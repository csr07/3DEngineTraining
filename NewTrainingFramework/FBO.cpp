#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "FBO.h"
#include "Globals.h"

bool FBO::Init(bool isDepth)
{
	bool result = true;

	glGenFramebuffers(1,&m_hFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER,m_hFrameBuffer);

	if(isDepth)
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,createTexture(isDepth),0);

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,createTexture(false),0);	
	
	int i=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(i!=GL_FRAMEBUFFER_COMPLETE)
	{
		result = false;
		esLogMessage("Framebuffer FAIL!!!, CODE= %d",i);
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	return result;
}

unsigned int FBO::createTexture(bool isDepth)
{	
	if(isDepth)
	{
		glGenTextures(1,&m_hTextureDepth);
		glBindTexture(GL_TEXTURE_2D,m_hTextureDepth);
	}
	else
	{
		glGenTextures(1,&m_hTextureColor);
		glBindTexture(GL_TEXTURE_2D,m_hTextureColor);
	}
	glTexImage2D(GL_TEXTURE_2D,0,(isDepth ?  GL_DEPTH_COMPONENT:GL_RGBA), Globals::screenWidth , Globals::screenHeight,0,isDepth ? GL_DEPTH_COMPONENT : GL_RGBA, isDepth ? GL_UNSIGNED_SHORT:GL_UNSIGNED_BYTE ,NULL);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	
	int i;
	i=glGetError();
	if(i!=0)
	{
		esLogMessage( "Error happened while loading the texture for FBO: %d", i);
	}
	glBindTexture(GL_TEXTURE_2D,0);

	if(isDepth)
		return m_hTextureDepth;
	else
		return m_hTextureColor;
}

FBO::FBO()
{
	
}

FBO::~FBO()
{
if(m_hTextureColor)
		glDeleteTextures(1,&m_hTextureColor);

if(m_hTextureDepth)
		glDeleteTextures(1,&m_hTextureDepth);

if(m_hFrameBuffer)
		glDeleteFramebuffers(1,&m_hFrameBuffer);


}