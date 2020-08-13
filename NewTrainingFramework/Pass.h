#pragma once
#include "../Utilities/utilities.h"

class Shaders;
class FBO;
//class Square;

class Pass
{
public:
	Pass();
	~Pass();

	void Init();
	void Draw();
	
	int GetPassId(){return m_id;}
	int getShaderId(){return m_shaderId;}         //id que viene del EM.txt
	int getTargetFBOId(){return m_frameBufferId;} //id que viene del EM.txt
	int * GetPassTextures() {return m_passTexturesColor;}
	int * GetPassColorDepthConf(){return m_passTexturesDepth;}
	float * GetOtherData(){return m_otherData;}

	void SetPassId(int id){m_id = id;}
	void SetPassName(char *n){strcpy(m_name,n);}
	void SetShaderId(int id){m_shaderId = id;}
	void SetTargetFBOId(int id){m_frameBufferId = id;}
	void SetTexturesColor(int * textures);
	void SetTexturesDepth(int * conf);
	void SetNumOtherData(int d){m_numOtherData = d;}
	void SetOtherData(float * other);

private:
	int			m_id;	
	int			m_numOtherData;
	char		m_name[100];
	int			m_shaderId;						// shader que va a utilizar	 Id del txt
	int			m_frameBufferId;					// target que vamos a dibujar Id del txt		
	int			m_passTexturesColor[3];			//
	int			m_passTexturesDepth[3];
	float 		m_otherData[5];

	Shaders *   theShader;

	/*int m_passTextures[MAX_TEXT_POST];			//
	int m_passColorDepthConf[MAX_TEXT_POST];
	float m_passOtherData[MAX_POST_PARAM];*/
};