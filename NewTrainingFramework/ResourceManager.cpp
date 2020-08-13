#include "stdafx.h"
#include "ResourceManager.h"

#include "Model.h"
#include "Texture.h"
#include "Shaders.h"

enum RMType{
	NONE,
	MODELS,
	TEXTURE,
	CTEXTURE,
	SHADERS,
	END
};

ResourceManager * ResourceManager::ms_pRManager = NULL;

ResourceManager::~ResourceManager() 
{		
	if(m_pModels)
	{		
		delete[] m_pModels;
		m_pModels = NULL;
	}
	if(m_p2DTextures)
	{
		delete[] m_p2DTextures;
		m_p2DTextures = NULL;
	}
	if(m_pCTextures)
	{
		delete[] m_pCTextures;
		m_pCTextures = NULL;
	}
	if(m_pShaders)
	{		
		delete[] m_pShaders;
		m_pShaders = NULL;
	}
}

bool ResourceManager::Init(char * filename)
{
	esLogMessage("Loading Resources file %s...\n", filename);

    m_numModels = 0;
	m_num2DTextures = 0;
	m_numCTextures = 0;
	m_numShaders = 0;	
	m_numStates = 0;
			
	int tId;
	const int tam = 100;
	char tPath[tam];
	int tMode;
	char tVSPath[tam];
	char tFSPath[tam];
	char tStateName[tam];
	int  tStateId;

	//Contador
	unsigned int cont = 0;
	unsigned int contStates = 0;

    FILE * pFile;
    errno_t errorCode;
    RMType type = MODELS;

    if((errorCode = fopen_s(&pFile, filename, "r"))!=0)
    {
        esLogMessage("Impossible to open the file !\n");
        return false;
    }
    else
    {        
		type = MODELS;
    }

    while( type < END ){

        if(type == MODELS)
        {   
			if(m_numModels == 0)
            {
				fscanf_s( pFile, "#Models: %d\n",&m_numModels);				
				m_pModels =  new Model[m_numModels];								
            }

            for(int i=0; i<m_numModels; i++)
			{
				m_pModels[i] = Model();

				fscanf_s( pFile, "ID %d\n FILE \"%s\n",&tId,tPath,sizeof(tPath));				           
				cleanPathString(&tPath[0]);
				printf("tId escaneado: %d\n",tId);
				printf("tPath escaneado: %s\n",tPath, strlen(tPath));
				printf("long de tPath escaneada: %d\n",strlen(tPath));
				m_pModels[i].m_id = tId;
				//m_pModels[i].m_path = new char[strlen(tPath)];
				strcpy(m_pModels[i].m_path,tPath);
				printf("El path del modelo es: %s\n",m_pModels[i].m_path,sizeof(m_pModels[i].m_path));							
			}
			type = TEXTURE;
        }
        else if(type == TEXTURE)
        {
            if(m_num2DTextures == 0)
            {
				fscanf_s( pFile, "#2D_Textures: %d\n",&m_num2DTextures);
                cont = 0;               
				m_p2DTextures = new Texture[m_num2DTextures];
            }

            if(fscanf_s( pFile, "ID %d\n FILE \"%s\n MODE %d\n", &tId, tPath,sizeof(tPath), &tMode) != EOF)
            {                
				cleanPathString(&tPath[0]);
				m_p2DTextures[cont].m_id = tId;
				strcpy(m_p2DTextures[cont].m_path,tPath);
				m_p2DTextures[cont].m_mode=tMode;
				printf("El path de la textura 2D es: %s\n",m_p2DTextures[cont].m_path,sizeof(m_p2DTextures[cont].m_path));
				printf("El modo de la textura es: %d\n",m_p2DTextures[cont].m_mode);

				cont++;
                if(cont>=m_num2DTextures)
                {                                
                    type = CTEXTURE;                                                                                           
                }
            }
            else
			{
				type = END;
			}
        }
		else if(type == CTEXTURE)
        {
            if(m_numCTextures == 0)
            {
				fscanf_s( pFile, "#Cube_Textures: %d\n",&m_numCTextures);
                cont = 0;               
				m_pCTextures = new Texture[m_numCTextures];
            }

            if(fscanf_s( pFile, "ID %d\n FILE \"%s\n", &tId, tPath, sizeof(tPath)) != EOF)
            {                
				cleanPathString(&tPath[0]);
				m_pCTextures[cont].m_id = tId;
				strcpy(m_pCTextures[cont].m_path,tPath);	
				printf("El path de la textura Cubica es: %s\n",m_pCTextures[cont].m_path,sizeof(m_pCTextures[cont].m_path));

				cont++;
                if(cont>=m_numCTextures)
                {                                
                    type = SHADERS;                                                                                           
                }
            }
            else
			{
				type = END;
			}
        }
		else if(type == SHADERS)
        {
            if(m_numShaders == 0)
            {
				fscanf_s( pFile, "#Shaders: %d\n",&m_numShaders);
                cont = 0;               
				m_pShaders = new Shaders[m_numShaders];
				printf("El numero de shaders es: %d\n", m_numShaders);
            }

			if((fscanf_s( pFile, "ID %d\n VS \"%s\n FS \"%s\n STATES %d\n", &tId, tVSPath, sizeof(tVSPath), tFSPath, sizeof(tFSPath), &m_numStates) != EOF))
            {                
				cleanPathString(&tVSPath[0]);
				cleanPathString(&tFSPath[0]);				
				m_pShaders[cont].m_id = tId;
				strcpy(m_pShaders[cont].m_VSPath,tVSPath);
				strcpy(m_pShaders[cont].m_FSPath,tFSPath);
				m_pShaders[cont].m_numStates = m_numStates;
				
				printf("El path del Vertex Shader es: %s\n", m_pShaders[cont].m_VSPath, sizeof(m_pShaders[cont].m_VSPath));
				printf("El path del Fragment Shader es: %s\n", m_pShaders[cont].m_FSPath,sizeof(m_pShaders[cont].m_FSPath));
				printf("El numero de states es: %d\n", m_numStates);
				
				m_pShaders[cont].stateId = new int[m_numStates];
			
				for(int i=0; i<m_numStates; i++)
				{
					if(fscanf_s(pFile, "%d\n", &tStateId) != EOF)
					{					
						m_pShaders[cont].stateId[i]=tStateId;
						printf("El numero id del state que se guardo es: %d\n", m_pShaders[cont].stateId[i]);						
					}					
				}				

				cont++;
                if(cont>=m_numShaders)
                {                                
                    type = END;                                                                                           
                }
            }
            else
			{
				type = END;
			}
        }
    }       
    fclose( pFile );
    pFile = NULL;

	return true;
}

void ResourceManager::InitResources()
{
	//Init Models	
	for(int i=0; i<m_numModels; i++)
	{
		printf("RMan Model No.%d\n",(i+1));
		m_pModels[i].InitModel(m_pModels[i].m_path);
	}
	//Init 2D Textures
	for(int i=0; i<m_num2DTextures; i++)
	{
		printf("RMan Texture No.%d\n",(i+1));
		m_p2DTextures[i].InitTexture(m_p2DTextures[i].m_path);
	}
	//Init Cubic Textures
	for(int i=0; i<m_numCTextures; i++)
	{
		printf("RMan CTexture No.%d\n",(i+1));
		m_pCTextures[i].InitCubeTexture(m_pCTextures[i].m_path);
	}
	//Init Shaders
	for(int i=0; i<m_numShaders; i++)
	{
		printf("RMan Shader No.%d\n",(i+1));
		m_pShaders[i].Init(m_pShaders[i].m_VSPath,m_pShaders[i].m_FSPath);	
	}	
	printf("FIN de InitResources\n");
}

Model * ResourceManager::getModelById(int id)
{
	for(int i=0; i<m_numModels; i++)
	{
		if(m_pModels[i].m_id == id)
		{			
			Model * p = m_pModels+i; 
			return p;			
		}
	}
	return NULL;
}

Texture * ResourceManager::getTextureById(int id)
{
	for(int i=0; i<m_num2DTextures; i++)
	{
		if(m_p2DTextures[i].m_id == id)
		{			
			Texture * p = m_p2DTextures+i;
			return p;			
		}
	}	
}

Texture * ResourceManager::getCubeTextureById(int id)
{
	for(int i=0; i<m_numCTextures; i++)
	{
		if(m_pCTextures[i].m_id == id)
		{			
			Texture * p = m_pCTextures+i;
			return p;			
		}
	}	
}

Shaders * ResourceManager::getShadersById(int id)
{
	for(int i=0; i<m_numShaders; i++)
	{
		if(m_pShaders[i].m_id == id)
		{			
			Shaders * p = m_pShaders+i;
			return p;			
		}
	}	
}

void ResourceManager::cleanPathString(char * thePath)
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

void ResourceManager::PrintData()
{
	printf("\nPRINTING RESOURCE MANAGER DATA\n");
	//Num de objetos
	printf("Numero de modelos: %d\n",m_numModels);
	printf("Numero de 2DTexturas: %d\n",m_num2DTextures);
	printf("Numero de CTextures: %d\n",m_numCTextures);
	printf("Numero de shaders: %d\n",m_numShaders);
	printf("Numero de States: %d\n",m_numStates);

	for(int i=0; i<m_numModels; i++)
	{
		printf("Model Id: %d\n",m_pModels[i].m_id);	
		printf("Model Path: %s\n",m_pModels[i].m_path,sizeof(m_pModels[i].m_path));
	}
	
}