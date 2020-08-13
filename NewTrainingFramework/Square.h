#ifndef SQUARE_H
#define SQUARE_H

#include "Vertex.h"
#include "Shaders.h"

class Square
{
private:
	int quad_indices[6];
	Vertex quad_vertices[4];
	GLuint m_hVertexBuffer;
	GLuint m_hIndexBuffer;


public:
	void Init();
	void draw(Shaders * shader);
	void setShaderValues(Shaders * shader);

	Square();
	~Square();


};

#endif