#pragma once
#include "Math.h"
#include "../Utilities/utilities.h"

struct Light
{
	int			m_id;
	int			m_lightType;
	int			m_moveType;
	float		m_radius;
	float		m_speed;
	Vector3		m_posDir;
	Vector3		m_color;
};