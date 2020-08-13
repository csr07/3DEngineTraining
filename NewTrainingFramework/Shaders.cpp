#include "stdafx.h"
#include "Shaders.h"

int Shaders::Init(char * fileVertexShader, char * fileFragmentShader)
{
	/*in Shader::Init(), you will interrogate to find the location of uniforms 
	(using glGetUniformLocation() – the function returns -1 when the uniform 
	is not an active uniform inside the program) and you keep the location as 
	data members in the Shader class;*/	
	
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);

	if ( vertexShader == 0 )
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes	

	a_position			= glGetAttribLocation(program, "a_pos");
	a_color				= glGetAttribLocation(program,"a_color");
	a_uv				= glGetAttribLocation(program,"a_uv");	
	a_normal            = glGetAttribLocation(program,"a_normal");
	a_binormal          = glGetAttribLocation(program,"a_binormal");
	a_tgt				= glGetAttribLocation(program,"a_tgt");

	uWorldMatrixLoc		= glGetUniformLocation(program, "u_WMatrix");
	uWLPLoc				= glGetUniformLocation(program, "u_WVPMatrix");
	
	uTextureLoc[0]	= glGetUniformLocation(program, "u_s_texture[0]");	
	uTextureLoc[1]	= glGetUniformLocation(program, "u_s_texture[1]");	
	uTextureLoc[2]	= glGetUniformLocation(program, "u_s_texture[2]");	
	uTextureLoc[3]	= glGetUniformLocation(program, "u_s_texture[3]");	

	uCubeTextureLoc = glGetUniformLocation(program, "u_s_cubeTexture");

	uTilingFactorLoc= glGetUniformLocation(program, "u_tilingFactor");
	uCameraPosLoc	= glGetUniformLocation(program, "u_cameraPos");
	uFogColorLoc    = glGetUniformLocation(program, "u_fogColor");
	uFogStartLoc	= glGetUniformLocation(program, "u_fogStart");
	uFogRangeLoc	= glGetUniformLocation(program, "u_fogRange");
	uTimeLoc        = glGetUniformLocation(program, "u_time");

	//LightStuff
	uAmbientColorLoc	= glGetUniformLocation(program, "u_ambientColor");
	uAmbientWeightLoc	= glGetUniformLocation(program, "u_ambientWeight");
	uSpecularPowerLoc	= glGetUniformLocation(program, "u_specularPower");
	uNumObjLightsLoc	= glGetUniformLocation(program, "u_numObjLights");
	
	uLightTypeLoc[0]	= glGetUniformLocation(program, "u_lightType[0]");
	uLightTypeLoc[1]	= glGetUniformLocation(program, "u_lightType[1]");
	uLightTypeLoc[2]	= glGetUniformLocation(program, "u_lightType[2]");
	uLightTypeLoc[3]	= glGetUniformLocation(program, "u_lightType[3]");
	uLightTypeLoc[4]	= glGetUniformLocation(program, "u_lightType[4]");
	
	uLightPosDirLoc[0]	= glGetUniformLocation(program, "u_lightPosDir[0]");
	uLightPosDirLoc[1]	= glGetUniformLocation(program, "u_lightPosDir[1]");
	uLightPosDirLoc[2]	= glGetUniformLocation(program, "u_lightPosDir[2]");
	uLightPosDirLoc[3]	= glGetUniformLocation(program, "u_lightPosDir[3]");
	uLightPosDirLoc[4]	= glGetUniformLocation(program, "u_lightPosDir[4]");

	uLightColorLoc[0]	= glGetUniformLocation(program, "u_lightColor[0]");
	uLightColorLoc[1]	= glGetUniformLocation(program, "u_lightColor[1]");
	uLightColorLoc[2]	= glGetUniformLocation(program, "u_lightColor[2]");
	uLightColorLoc[3]	= glGetUniformLocation(program, "u_lightColor[3]");
	uLightColorLoc[4]	= glGetUniformLocation(program, "u_lightColor[4]");

	uBlurStepLoc		= glGetUniformLocation(program, "u_blurStep");
	uTexturePostLoc[0]  = glGetUniformLocation(program, "u_texturePost[0]");
	uTexturePostLoc[1]  = glGetUniformLocation(program, "u_texturePost[1]");
	uTexturePostLoc[2]  = glGetUniformLocation(program, "u_texturePost[2]");
	uBloomLimitLoc      = glGetUniformLocation(program, "u_bloomLimit"); 

	uNearLoc			= glGetUniformLocation(program, "u_near");
	uFarLoc				= glGetUniformLocation(program, "u_far");
	uFadeLoc			= glGetUniformLocation(program, "u_fade");
	uClarityLoc			= glGetUniformLocation(program, "u_clarity");	

	return 0;
}

Shaders::~Shaders()
{
	if(m_numStates>0 && stateId != NULL)
	{
		delete[] stateId;
		stateId = NULL;
	}
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}