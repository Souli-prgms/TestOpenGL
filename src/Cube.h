#pragma once

#include "Shape.h"
#include "Material.h"

class Cube : public Shape
{
public:
	Cube();
	virtual ~Cube();

	void init();
	void display(Shader *shader);

private:
	int m_vbo[3];
	int m_vao;
	Material* m_material;
};