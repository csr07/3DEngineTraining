#ifndef FBO_H
#define FBO_H
#include "Globals.h"

class FBO
{
private:
	GLuint m_hFrameBuffer;
	GLuint m_hTextureDepth;
	GLuint m_hTextureColor;

	unsigned int createTexture(bool isDepth=false);

public:

	bool Init(bool isDepth);
	GLuint get_hFrameBuffer(){return m_hFrameBuffer;}
	GLuint get_hTextureDepth(){return m_hTextureDepth;}
	GLuint get_hTextureColor(){return m_hTextureColor;}

	//void BindFBO();
	//void UnBindFBO();

	FBO();
	~FBO();

};
#endif