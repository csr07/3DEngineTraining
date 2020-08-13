#pragma once
#include "../Utilities/utilities.h"

class Model;
class Texture;
class Shaders;

class ResourceManager
{
protected:
	ResourceManager() {}
	~ResourceManager();
	/*implicit methods exist for the copy constructor and operator= and we want to forbid calling them.*/
	ResourceManager(const ResourceManager &){} 
	ResourceManager& operator =(const ResourceManager &){} 

public:
	static ResourceManager * GetInstance() {
		if ( ms_pRManager == NULL )
			ms_pRManager = new ResourceManager;
		return ms_pRManager;
	}
	static void DestroyInstance() {
		if ( ms_pRManager )
		{
			delete ms_pRManager;
			ms_pRManager = NULL;
		}
	}

	bool		Init(char * file);
	void		InitResources();
	Model	*	getModelById(int id);
	Texture *	getTextureById(int id);
	Texture *	getCubeTextureById(int id);
	Shaders *	getShadersById(int id);
	void		cleanPathString(char * thePath);

	void		PrintData();


protected:
	static ResourceManager * ms_pRManager;

public:
	int						m_numModels;
	int						m_num2DTextures;
	int						m_numCTextures;
	int						m_numShaders;
	int						m_numStates;
	
	Model	*				m_pModels;
	Texture *				m_p2DTextures;
	Texture *				m_pCTextures;
	Shaders *				m_pShaders;	
};

