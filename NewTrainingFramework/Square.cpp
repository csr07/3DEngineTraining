#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Square.h"

void Square::Init()
{
	quad_vertices[0].pos.x = -1.0;	quad_vertices[0].pos.y = 1.0;	quad_vertices[0].pos.z = 0.0;
	quad_vertices[0].uvs.x = 0.0;	quad_vertices[0].uvs.y = 1.0;
	quad_vertices[1].pos.x = -1.0;	quad_vertices[1].pos.y = -1.0;	quad_vertices[1].pos.z = 0.0;
	quad_vertices[1].uvs.x = 0.0;	quad_vertices[1].uvs.y = 0.0;
	quad_vertices[2].pos.x = 1.0;	quad_vertices[2].pos.y = -1.0;	quad_vertices[2].pos.z = 0.0;
	quad_vertices[2].uvs.x = 1.0;	quad_vertices[2].uvs.y = 0.0;
	quad_vertices[3].pos.x = 1.0;	quad_vertices[3].pos.y = 1.0;	quad_vertices[3].pos.z = 0.0;
	quad_vertices[3].uvs.x = 1.0;	quad_vertices[3].uvs.y = 1.0;

	quad_indices[0]= 0;	quad_indices[1]= 1;	quad_indices[2]= 2;
	quad_indices[3]= 0;	quad_indices[4]= 2;	quad_indices[5]= 3;

	glGenBuffers(1, &m_hVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_hVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), quad_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_hIndexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_hIndexBuffer);
	glBufferData(GL_ARRAY_BUFFER, (6)*sizeof(int), quad_indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Square::setShaderValues(Shaders * shader)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_hVertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_hIndexBuffer);
	
	if(shader->a_position != -1)
	{
		glEnableVertexAttribArray(shader->a_position);
		glVertexAttribPointer(shader->a_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);		
	}

	if(shader->a_uv != -1)
	{
		glEnableVertexAttribArray(shader->a_uv);
		glVertexAttribPointer(shader->a_uv, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));
	}
}

void Square::draw(Shaders * shader)
{	
	setShaderValues(shader);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
	
	glDisableVertexAttribArray(shader->a_position);
	glDisableVertexAttribArray(shader->a_uv);
	//glBindBuffer(GL_ARRAY_BUFFER,0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);	
}

Square::Square()
{

}

Square::~Square()
{
	glDeleteBuffers(1,&m_hVertexBuffer);
	glDeleteBuffers(1,&m_hIndexBuffer);
}
