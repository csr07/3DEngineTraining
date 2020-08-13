#pragma once
#include "../Utilities/utilities.h"

class Texture
{
public:
	Texture();
	~Texture();
	void InitTexture(char* file);
	void InitCubeTexture(char* file);
	char * ExtractFace(char * bufferSource, char * bufferDest, int widthB, int wB, int bpp, int hOffset, int vOffset);
	//char * ExtractFace(char * bufferTGA, char * temp, int width, int bpp, int offsetX, int offsetY); 
	
	GLuint	m_hTexture; // handle de la textura
		
	int		m_id;
	char 	m_path[100];
	int 	m_mode;
};