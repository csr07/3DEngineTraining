#pragma once
#include "../Utilities/utilities.h"

//forward declarations...
class Shaders;
class FBO;
class Effect;
class Square;

class EffectManager
{
protected:
	EffectManager(){}
	~EffectManager();
	EffectManager(const EffectManager &){}
	EffectManager& operator = (const EffectManager &){}

public:
	static EffectManager * GetInstance(){
		if(ms_pEManager == NULL)
			ms_pEManager = new EffectManager;
		return ms_pEManager;
	}
	static void DestroyInstance(){
		if(ms_pEManager)
		{
			delete ms_pEManager;
			ms_pEManager = NULL;
		}
	}

	bool		Init(char * file);
	void		CleanPathString(char * thePath);
	void		Draw(int key); 
	void		PreDraw(bool activate);

protected:
	static EffectManager * ms_pEManager;

public:
	int				m_numShaders;
	int				m_numFBOS;
	int				m_numEffects;

	Shaders *		m_pShaders;
	FBO *			m_pFBOs;
	Effect *		m_pEffects;
	Square *		m_pSquare;

	GLuint			m_hVertexBuffer;
	GLuint			m_hIndexBuffer;


};