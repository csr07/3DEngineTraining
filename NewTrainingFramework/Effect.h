#pragma once
#include "../Utilities/utilities.h"

class Pass;

class Effect
{
public:
	Effect(){}
	~Effect();

	void Init();
	void Draw();


	int			m_id;
	char		m_name[100];
	int			m_numPass;
	Pass *		m_pPass;

};