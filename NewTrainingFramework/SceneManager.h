#pragma once 
#include "../Utilities/utilities.h"

class Object;
class Camera;
class Light;

class SceneManager
{
protected:
	SceneManager(){}
	~SceneManager();
	
	SceneManager(const SceneManager &){}
	SceneManager& operator =(const SceneManager &){}

public:
	static SceneManager * GetInstance()
	{
		if(ms_pSManager == NULL)
			ms_pSManager = new SceneManager;
		return ms_pSManager;
	}
	static void DestroyInstance()
	{
		if(ms_pSManager)
		{
			delete ms_pSManager;
			ms_pSManager = NULL;
		}
	}

	bool	Init(char * file);
	void	InitObjects();
	void	Draw();
	void	Update(float dTime);

protected:
	static SceneManager * ms_pSManager;

public:
	int			m_numObjects;	
	Object *	m_pObjects;
	Camera *	m_pCamera;

	Vector3		m_fogColor;
	float		m_fogStart;
	float		m_fogRange;	
	float		m_totalTime;

	////////////Light data
	Vector3		m_ambientColor;
	float		m_ambientW;

	int			m_numLights;
	Light *		m_pLights;
};