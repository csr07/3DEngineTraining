#include "stdafx.h"
#include "SceneManager.h"

#include "Object.h"
#include "Camera.h"
#include "Light.h"

enum SMType
{
	NONE,
	OBJECT,
	LIGHT,
	CAMERA,
	FOG,
	END
};

SceneManager * SceneManager::ms_pSManager = NULL;

SceneManager::~SceneManager()
{
	if(m_pCamera != NULL)
	{
		delete m_pCamera;
		m_pCamera = NULL;
	}
	if(m_pObjects != NULL)
	{
		delete[] m_pObjects;
		m_pObjects = NULL;
	}
	if(m_pLights != NULL)
	{
		delete[] m_pLights;
		m_pLights = NULL;
	}
}

bool SceneManager::Init(char * filename)
{
	//esLogMessage("Loading Scene File %s....\n");		
	m_numObjects = 0;
	m_numLights = 0;
	m_totalTime = 0.0f;
		
	int tNum2DTextures = 0;
	int tNumCTextures = 0;
	int tNumLights = 0;

	int tObjectId; //Id del objeto
	int tModelId;
	int t2DTextureId;
	int tCTextureId;
	int tShadersId;	
	int tLightId;
	int tObjLightId;

	Vector3 tPosition;
	Vector3 tRotation;
	Vector3 tScale;

	float tTilingFactor = 0.0f;

	float tNear;
	float tFar;
	float tFOV;
	float tSpeed;
	float tSpeedRot;	

	/////Light data
	Vector3 tPosDir;
	int		tType;
	Vector3 tColor;
	int		tMoving;
	float	tRadius;
	float   tLightSpeed;

	int		tNumSpecParam;
	float	tSpecularPow;

	FILE * pFile;
	errno_t errorCode;
	SMType type = NONE;

	if((errorCode = fopen_s(&pFile, filename, "r"))!=0)
    {
        esLogMessage("Impossible to open the file !\n");
        return false;
    }
    else
    {        
		type = OBJECT;
    }

	while(type < END)
	{
		if(type == OBJECT)
		{
			if(m_numObjects == 0)
			{
				fscanf_s(pFile, "#Objects: %d\n", &m_numObjects);
				m_pObjects = new Object[m_numObjects];
			}
			for(int j=0; j<m_numObjects; j++)
			{			
				/////////////////////////////////////////////////////////////
				fscanf_s(pFile, "ID %d\n MODEL %d\n NUM_TEXTURES %d\n", &tObjectId, &tModelId, &tNum2DTextures);			
				m_pObjects[j].m_id = tObjectId;
				m_pObjects[j].m_modelId = tModelId;
				m_pObjects[j].m_num2DTextures = tNum2DTextures;

				if(tNum2DTextures > 0)
				{
					m_pObjects[j].m_2DTexturesIds = new int[tNum2DTextures];
					for(int i=0; i<tNum2DTextures; i++)
					{
						fscanf_s(pFile,"TEXTURE %d\n", &t2DTextureId);
						m_pObjects[j].m_2DTexturesIds[i] = t2DTextureId;				
					}
				}
				/////////////////////////////////////////////////////////////
				fscanf_s(pFile, "NUM_CUBE_TEXTURES %d\n", &tNumCTextures);
				m_pObjects[j].m_numCTextures = tNumCTextures;

				if(tNumCTextures > 0)
				{
					m_pObjects[j].m_CTexturesIds = new int[tNumCTextures];
					for(int i=0; i<tNumCTextures; i++)
					{
						fscanf_s(pFile,"CUBETEX %d\n", &tCTextureId);
						m_pObjects[j].m_CTexturesIds[i] = tCTextureId;				
					}
				}
				/////////////////////////////////////////////////////////////
				fscanf_s(pFile, "SHADER %d\n", &tShadersId);
				m_pObjects[j].m_ShadersId = tShadersId;			
				/////////////////////////////////////////////////////////////
				fscanf_s(pFile, "NUM_LIGHTS %d\n", &tNumLights);
				m_pObjects[j].m_numObjLights = tNumLights;

				if(tNumLights > 0)
				{
					m_pObjects[j].m_objLightsIds = new int[tNumLights];
					for(int i=0; i<tNumLights; i++)
					{
						fscanf_s(pFile,"LIGHT %d\n", &tObjLightId);
						m_pObjects[j].m_objLightsIds[i] = tObjLightId;				
					}
					// read for specific parameters
					fscanf_s(pFile, "NUM_SPECIFIC_PARAMETER %d\n", &tNumSpecParam); // este numero no lo estoy pasando a otra variable en el objeto
					if(tNumSpecParam > 0)
					{
						fscanf_s(pFile, "SPECULAR_POWER %d\n", &tSpecularPow);// lo paso al objeto
						m_pObjects[j].m_specularPower = tSpecularPow;
					}
					else
					{
						m_pObjects[j].m_specularPower = 10.0f;
					}
				}
				/////////////////////////////////////////////////////////////
				fscanf_s(pFile, "POSITION %f, %f, %f\n ROTATION %f, %f, %f\n SCALE %f, %f, %f\n",
					&tPosition.x,&tPosition.y,&tPosition.z,
					&tRotation.x,&tRotation.y,&tRotation.z,
					&tScale.x,&tScale.y,&tScale.z);
				m_pObjects[j].m_position =	tPosition;
				m_pObjects[j].m_rotation =	tRotation;
				m_pObjects[j].m_scale    =	tScale;				
				/////////////////////////////////////////////////////////////
				fscanf_s(pFile, "TILING_FACTOR %f\n", &tTilingFactor);  // recordar poner el tiling factor para cada objeto en mi scene manager 64 para piso
				m_pObjects[j].m_tilingFactor = tTilingFactor;			
				/////////////////////////////////////////////////////////////				
			}
			type = LIGHT;
		}
		else if(type == LIGHT)
		{
			if(m_numLights == 0)
			{
				fscanf_s(pFile, "#LIGHTS: %d\n", &m_numLights);
				m_pLights = new Light[m_numLights];// hacer la estructura de luces? o guardar directamente en los miembros del objeto?				
			}
			if(m_numLights > 0)
			{
				fscanf_s(pFile, "AMBIENTCOLOR %f, %f, %f\n", &m_ambientColor.x, &m_ambientColor.y, &m_ambientColor.z);
				fscanf_s(pFile, "AMBIENTWEIGHT %f\n", &m_ambientW);
				for(int i=0; i<m_numLights; i++)
				{
					//leer los datos y guardarlos directamente en el array de Lights del Scene Manager..
					fscanf_s(pFile, "ID %d\n", &tLightId);
					fscanf_s(pFile, "POS/DIR %f, %f, %f\n", &tPosDir.x,&tPosDir.y,&tPosDir.z);
					fscanf_s(pFile, "TYPE %d\n", &tType);
					fscanf_s(pFile, "COLOR %f, %f, %f\n", &tColor.x,&tColor.y,&tColor.z);

					m_pLights[i].m_id			= tLightId;
					m_pLights[i].m_posDir		= tPosDir;  // revisar la asignacion correcta
					m_pLights[i].m_lightType	= tType;
					m_pLights[i].m_color		= tColor;	// revisar asignacion

					if(tType == 1)
					{
						fscanf_s(pFile, "MOVING %d\n", &tMoving);
						fscanf_s(pFile, "RADIUS %f\n", &tRadius);
						fscanf_s(pFile, "SPEED %f\n", &tLightSpeed);

						m_pLights[i].m_moveType = tMoving;
						m_pLights[i].m_radius	= tRadius;
						m_pLights[i].m_speed	= tLightSpeed;
					}

				}
			}
			type = CAMERA;
		}
		else if(type == CAMERA)
		{
			fscanf_s(pFile, "#CAMERA\n NEAR %f\n FAR %f\n FOV %f\n SPEED %f\n SPEED_ROT %f\n",
				&tNear, &tFar, &tFOV, &tSpeed, &tSpeedRot);				
			m_pCamera = new Camera();
			m_pCamera->Init(tNear, tFar, tFOV, tSpeed, tSpeedRot);
			type = FOG;
		}
		else if(type == FOG)
		{
			fscanf_s(pFile, "#FOG\n FOG_COLOR %f, %f, %f\n FOG_START %f\n FOG_RANGE %f\n",
				&m_fogColor.x, &m_fogColor.y, &m_fogColor.z, &m_fogStart, &m_fogRange);							
			type = END;
		}
		else
		{
			type = END;
		}
	}

	///fin de lectura del archivo, inicializar los objetos
	for(int i=0; i< m_numObjects; i++)
	{
		m_pObjects[i].Init();
	}
	return true;
}

void SceneManager::Draw()
{
	//draw each object
	for(int i=0; i<m_numObjects; i++)
	{
		m_pObjects[i].Draw();
	}
}

void SceneManager::Update(float dTime)
{
	m_totalTime += dTime;
	
	m_pCamera->Update(dTime);

	//update each object
	for(int i=0; i<m_numObjects; i++)
	{
		m_pObjects[i].Update(m_totalTime);
	}
}