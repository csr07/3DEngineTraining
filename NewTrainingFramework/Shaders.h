#pragma once
#include "../Utilities/utilities.h"

class Shaders 
{
public:
	GLuint program, vertexShader, fragmentShader;

	GLint	a_position;
	GLint	a_color;
	GLint	a_uv;
	GLint   a_normal;
	GLint   a_binormal;
	GLint   a_tgt;

	GLint	uWorldMatrixLoc;
	GLint	uWLPLoc;
	GLint	uTextureLoc[4];
	GLint	uCubeTextureLoc;
	GLint   uTilingFactorLoc;
	GLint   uCameraPosLoc;	
	GLint	uFogColorLoc;
	GLint   uFogStartLoc;
	GLint   uFogRangeLoc;
	GLint   uTimeLoc;	

	//Light stuff
	GLint	uAmbientColorLoc;
	GLint	uAmbientWeightLoc;
	GLint	uSpecularPowerLoc;
	GLint	uNumObjLightsLoc;
	GLint	uLightTypeLoc[5];
	GLint   uLightPosDirLoc[5];
	GLint   uLightColorLoc[5];

	//Effects stuff
	GLint   uBlurStepLoc;
	GLint   uTexturePostLoc[3];
	GLint   uBloomLimitLoc;

	GLint   uNearLoc;
	GLint	uFarLoc;
	GLint	uFadeLoc;
	GLint	uClarityLoc;


	int		m_id;
	char 	m_VSPath[100];
	char 	m_FSPath[100];
	int		m_numStates;
	int *	stateId; // para guardar los diferentes states que tenga el shader ejem. 0cull_face / 1deep_Test	

	int Init(char * fileVertexShader, char * fileFragmentShader);
	Shaders(){}
	~Shaders();
};