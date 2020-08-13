#include "stdafx.h"
#include "Texture.h"

Texture::Texture()
{
	m_hTexture = 0;	
	m_id = 0;
}

Texture::~Texture()
{
	if(m_hTexture)
	{
		glDeleteBuffers(1,&m_hTexture);
	}
}

void Texture::InitTexture(char* file)
{
	// create the OpenGL ES texture resource and get the handle
	glGenTextures(1, &m_hTexture);
 
	// bind the texture to the 2D texture type
	glBindTexture(GL_TEXTURE_2D, m_hTexture);
 
	// create CPU buffer and load it with the image data
	int width, height, bpp;
	char * bufferTGA = LoadTGA(file, &width, &height, &bpp );
 
	// load the image data into OpenGL ES texture resource
	if(bpp == 24)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bufferTGA);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bufferTGA);
	}
 
	// free the client memory
	delete [] bufferTGA;
 
	//set the filters for minification and magnification	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
	// generate the mipmap chain
	glGenerateMipmap(GL_TEXTURE_2D);
	
	//set the wrapping modes
	//0 - GL_CLAMP_TO_EDGE
	//1 - GL_REPEAT
	// no entraba a ninguno de los dos casos y ocasionaba el mapeo de la textura equivocada...
	if (m_mode == 0)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	if(m_mode == 1)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
}

void Texture::InitCubeTexture(char* file)
{
	glGenTextures(1, &m_hTexture); 	
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_hTexture); 

	int widthP, heightP,wP, bpp; // in pixels
	int widthB, heightB, wB; // in bytes

	GLenum rgb;

	char * bufferTGA = LoadTGA(file, &widthP, &heightP, &bpp );
	
	if(bpp == 24)
		rgb = GL_RGB;
	else
		rgb = GL_RGBA;

	widthB = widthP * (bpp/8);
	heightB = heightP * (bpp/8);
	wP= widthP / 4;
	wB = widthB /4;
		
	char * faceTGA[6];
	for(int i=0; i<6; i++)
	{
		faceTGA[i]= new char[wP*wP*bpp/8];
	}
		
	faceTGA[0]= ExtractFace(bufferTGA, faceTGA[0], widthP, wP, bpp, 2*wP, wP);	
	faceTGA[1]= ExtractFace(bufferTGA, faceTGA[1], widthP, wP, bpp, 0, wP);
	faceTGA[2]= ExtractFace(bufferTGA, faceTGA[2], widthP, wP, bpp, wP, 0);
	faceTGA[3]= ExtractFace(bufferTGA, faceTGA[3], widthP, wP, bpp, wP, 2*wP);
	faceTGA[4]= ExtractFace(bufferTGA, faceTGA[4], widthP, wP, bpp, wP, wP);
	faceTGA[5]= ExtractFace(bufferTGA, faceTGA[5], widthP, wP, bpp, 3*wP, wP);
	
	for(int i=0; i<6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, rgb, widthP/4, heightP/3, 0, rgb, GL_UNSIGNED_BYTE, faceTGA[i]);
		delete faceTGA[i];	
	}		
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	delete [] bufferTGA; 
	
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
}

char * Texture::ExtractFace(char * source, char * dest, int widthP, int wP, int bpp, int hOffset, int vOffset)
{	
	int startByte = vOffset*widthP*bpp/8  +  hOffset*bpp/8; 

	for(int i=0; i<wP; i++ )
	{		
		memcpy_s( dest + i*wP*bpp/8, wP*wP*bpp/8, source + startByte, wP*bpp/8);			          	
		
		startByte += widthP*bpp/8;
	}
	return dest;
}