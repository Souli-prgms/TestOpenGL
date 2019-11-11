#pragma once

#include "Shape.h"
#include "Material.h"

class Ground: public Shape
{
public:
	Ground();
	~Ground();

	void init();
	void display(Shader *shader);
	void setWorldPos(int x, int y);

private:
	float m_texCoord[8];
	float m_pos[8];
	float m_normal[12];
	float m_tangent[12];
	float m_bitangent[12];
	unsigned int m_indices[6];

	GLuint m_vao;
	GLuint m_vbo[5];

	void setTexCoord(unsigned int id, float x, float y);
	void setPos(unsigned int id, float x, float y);
	void computeTangents();
};

