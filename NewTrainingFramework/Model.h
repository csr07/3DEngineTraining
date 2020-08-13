#pragma once

#include "../Utilities/utilities.h"
#include "Vertex.h"

#include <stdio.h>

class Model
{
public:
	Model();
	~Model();
	bool InitModel(char* filename);
		
	GLuint					m_hVertexBuffer;
	GLuint					m_hIndexBuffer;
	unsigned int			m_noIndices;

	int						m_id;
	char					m_path[100];
};
