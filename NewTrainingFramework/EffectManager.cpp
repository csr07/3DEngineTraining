#include "stdafx.h"
#include "EffectManager.h"

#include "Shaders.h"
#include "FBO.h"
#include "Effect.h"
#include "Pass.h"
#include "Square.h"

enum EMType{ //modificar los tipos
	NONE,
	SHADERS,
	FBOS,
	EFFECTS,
	END
};

enum {
	EFFECT_NONE,
	EFFECT_B_W,
	EFFECT_BLUR,
	EFFECT_BLOOM,
	EFFECT_DEPTH_OF_FIELD
};

EffectManager * EffectManager::ms_pEManager = NULL;

EffectManager::~EffectManager()
{
	if(m_hVertexBuffer != NULL)
	{
		glDeleteBuffers(1, &m_hVertexBuffer);
		m_hVertexBuffer = NULL;
	}
	if(m_hIndexBuffer != NULL)
	{
		glDeleteBuffers(1, &m_hIndexBuffer);
		m_hIndexBuffer = NULL;
	}
	if(m_pShaders != NULL)
	{
		delete[] m_pShaders;
		m_pShaders = NULL;
	}
	if(m_pEffects != NULL)
	{
		delete[] m_pEffects;
		m_pEffects = NULL;
	}
	if(m_pFBOs != NULL)
	{
		delete[] m_pFBOs;
		m_pFBOs = NULL;
	}
	if(m_pSquare != NULL)
	{
		delete m_pSquare;
		m_pSquare = NULL;
	}
}

bool EffectManager::Init(char * filename)
{
	esLogMessage("Loading Effects file %s...\n", filename);

	m_numShaders = 0;
	m_numFBOS = 0;
	m_numEffects = 0;

	int tId;
	int tPassId;
	int tPassShaderId;
	int tNumPasses;
	int tTarget;
	int tNumOtherData;	
	char tVSPath[100];
	char tFSPath[100];
	char tName[100];
	char tPassName[100];
	int tColor[3];
	int tDepth[3];
	float tOtherData[5];

	FILE * pFile;
    errno_t errorCode;
	EMType type = NONE;

	if((errorCode = fopen_s(&pFile, filename, "r"))!=0)
    {
        esLogMessage("Impossible to open the file !\n");
        return false;
    }
    else
    {        
		type = SHADERS;
    }

	while(type < END)
	{
		if(type == SHADERS)
		{
			if(m_numShaders == 0)
			{
				fscanf_s( pFile, "#Shaders: %d\n",&m_numShaders);	
				m_pShaders = new Shaders[m_numShaders];
			}

			for(int i=0; i<m_numShaders; i++)
			{
				fscanf_s(pFile, "ID %d\n", &tId);
				fscanf_s(pFile, "VSFile: \"%s\n", tVSPath, sizeof(tVSPath));
				fscanf_s(pFile, "FSFile: \"%s\n", tFSPath, sizeof(tFSPath));
				CleanPathString(&tVSPath[0]);
				CleanPathString(&tFSPath[0]);

				m_pShaders[i].m_id = tId;
				strcpy(m_pShaders[i].m_VSPath, tVSPath);
				strcpy(m_pShaders[i].m_FSPath, tFSPath);

				m_pShaders[i].Init(m_pShaders[i].m_VSPath,m_pShaders[i].m_FSPath);
			}
			type = FBOS;
		}
		else if (type == FBOS)
		{
			if(m_numFBOS == 0)
			{
				fscanf_s( pFile, "#FBO\nNoFBO %d\n",&m_numFBOS);					
			}

			m_pFBOs = new FBO[m_numFBOS];

			//for(int i=0; i<m_numFBOS; i++)
			//{
				m_pFBOs[0].Init(true);	// Solo el primer FBO tiene depth texture, si todos tenian o ninguno, daba problema
				m_pFBOs[1].Init(false);	// Sin depth texture
				m_pFBOs[2].Init(false);	// Sin depth texture
			//}

			type = EFFECTS;
		}
		else if(type == EFFECTS)
		{
			if(m_numEffects == 0)
			{
				fscanf_s( pFile, "#Effects: %d\n",&m_numEffects);					
			}

			m_pEffects = new Effect[m_numEffects];

			for(int i=0; i<m_numEffects; i++)
			{
				fscanf_s(pFile, "ID %d\n", &tId);
				fscanf_s(pFile, "Name %s\n", tName, sizeof(tName));
				fscanf_s(pFile, "NoPasses: %d\n", &tNumPasses);

				m_pEffects[i].m_id = tId;
				strcpy(m_pEffects[i].m_name,tName);
				m_pEffects[i].m_numPass = tNumPasses;

				m_pEffects[i].m_pPass = new Pass[tNumPasses];
				for(int j=0; j<tNumPasses; j++)
				{
					fscanf_s(pFile, "PassID %d\n", &tPassId);
					fscanf_s(pFile, "PassName %s\n", tPassName, sizeof(tPassName));
					fscanf_s(pFile, "   ShaderID %d\n", &tPassShaderId);
					fscanf_s(pFile, "   TexturesCOLOR: %d, %d, %d\n", &tColor[0],&tColor[1], &tColor[2]);
					fscanf_s(pFile, "   TexturesDEPTH: %d, %d, %d\n", &tDepth[0],&tDepth[1], &tDepth[2]);
					fscanf_s(pFile, "   Target: %d\n", &tTarget);
					fscanf_s(pFile, "   NumOtherData: %d\n", &tNumOtherData);					

					

					for(int k=0; k<tNumOtherData; k++)
					{
						fscanf_s(pFile, "   %f\n", &tOtherData[k]);
					}

					m_pEffects[i].m_pPass[j].SetPassId(tPassId);
					m_pEffects[i].m_pPass[j].SetPassName(tPassName);
					m_pEffects[i].m_pPass[j].SetShaderId(tPassShaderId);
					m_pEffects[i].m_pPass[j].SetTexturesColor(tColor);
					m_pEffects[i].m_pPass[j].SetTexturesDepth(tDepth);
					m_pEffects[i].m_pPass[j].SetTargetFBOId(tTarget);
					m_pEffects[i].m_pPass[j].SetNumOtherData(tNumOtherData);
					m_pEffects[i].m_pPass[j].SetOtherData(tOtherData);						
				}
			}

			type = END;
		}
	}

	m_pSquare = new Square;
	m_pSquare->Init();
}

void EffectManager::CleanPathString(char * thePath)
{
	//int tam = sizeof(thePath);
	for(int i=0; i<100; i++)  //hardcoding 100
	{			
		if(thePath[i] == '\0')
		{
			thePath[i-1] = '\0';						
		}
	}	
}

void EffectManager::Draw(int key)
{
	int effectActivated = 0;

	switch(key)
	{		
		case Globals::KEY_NUMPAD_0:
			effectActivated = EFFECT_NONE;
		break;

		case Globals::KEY_NUMPAD_1:
			effectActivated = EFFECT_B_W;
		break;
				
		case Globals::KEY_NUMPAD_2:
			effectActivated = EFFECT_BLUR;
		break;
				
		case Globals::KEY_NUMPAD_3:
			effectActivated = EFFECT_BLOOM;
		break;
				
		case Globals::KEY_NUMPAD_4:
			effectActivated = EFFECT_DEPTH_OF_FIELD;
		break;
	}

	//Llamar al draw() del efecto activado
	if(m_numEffects > (effectActivated-1))
	{
		m_pEffects[effectActivated-1].Draw();  
	}	
}

void EffectManager::PreDraw(bool activate)
{
	/*Hace un glBindFrameBuffer al primer FBO si el booleando esta activado, de lo contrario hace el bind a cero*/

	if(activate)
	{
		glBindFramebuffer(GL_FRAMEBUFFER,m_pFBOs[0].get_hFrameBuffer());
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}
}