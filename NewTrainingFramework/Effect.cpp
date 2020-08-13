#include "stdafx.h"
#include "Effect.h"

#include "Pass.h"

Effect::~Effect()
{
	if(m_pPass != NULL)
	{
		delete[] m_pPass;
		m_pPass = NULL;
	}
}

void Effect::Init()
{

}

void Effect::Draw()
{		
	for(int i=0; i< m_numPass; i++)
		m_pPass[i].Draw();
}