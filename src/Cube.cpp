#include "cube.h"

// cube ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3

// vertex coords array for glDrawArrays() =====================================
// A cube has 6 sides and each side has 2 triangles, therefore, a cube consists
// of 36 vertices (6 sides * 2 tris * 3 vertices = 36 vertices). And, each
// vertex is 3 components (x,y,z) of floats, therefore, the size of vertex
// array is 108 floats (36 * 3 = 108).
GLfloat vertices[] = { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,      // v0-v1-v2 (front)
					   -1,-1, 1,   1,-1, 1,   1, 1, 1,      // v2-v3-v0

						1, 1, 1,   1,-1, 1,   1,-1,-1,      // v0-v3-v4 (right)
						1,-1,-1,   1, 1,-1,   1, 1, 1,      // v4-v5-v0

						1, 1, 1,   1, 1,-1,  -1, 1,-1,      // v0-v5-v6 (top)
					   -1, 1,-1,  -1, 1, 1,   1, 1, 1,      // v6-v1-v0

					   -1, 1, 1,  -1, 1,-1,  -1,-1,-1,      // v1-v6-v7 (left)
					   -1,-1,-1,  -1,-1, 1,  -1, 1, 1,      // v7-v2-v1

					   -1,-1,-1,   1,-1,-1,   1,-1, 1,      // v7-v4-v3 (bottom)
						1,-1, 1,  -1,-1, 1,  -1,-1,-1,      // v3-v2-v7

						1,-1,-1,  -1,-1,-1,  -1, 1,-1,      // v4-v7-v6 (back)
					   -1, 1,-1,   1, 1,-1,   1,-1,-1 };    // v6-v5-v4

// normal array
GLfloat normals[] = { 0, 0, 1,   0, 0, 1,   0, 0, 1,      // v0-v1-v2 (front)
						0, 0, 1,   0, 0, 1,   0, 0, 1,      // v2-v3-v0

						1, 0, 0,   1, 0, 0,   1, 0, 0,      // v0-v3-v4 (right)
						1, 0, 0,   1, 0, 0,   1, 0, 0,      // v4-v5-v0

						0, 1, 0,   0, 1, 0,   0, 1, 0,      // v0-v5-v6 (top)
						0, 1, 0,   0, 1, 0,   0, 1, 0,      // v6-v1-v0

					   -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v1-v6-v7 (left)
					   -1, 0, 0,  -1, 0, 0,  -1, 0, 0,      // v7-v2-v1

						0,-1, 0,   0,-1, 0,   0,-1, 0,      // v7-v4-v3 (bottom)
						0,-1, 0,   0,-1, 0,   0,-1, 0,      // v3-v2-v7

						0, 0,-1,   0, 0,-1,   0, 0,-1,      // v4-v7-v6 (back)
						0, 0,-1,   0, 0,-1,   0, 0,-1 };    // v6-v5-v4

// color array
GLfloat colors[] = { 1, 1, 1,   1, 1, 0,   1, 0, 0,      // v0-v1-v2 (front)
						1, 0, 0,   1, 0, 1,   1, 1, 1,      // v2-v3-v0

						1, 1, 1,   1, 0, 1,   0, 0, 1,      // v0-v3-v4 (right)
						0, 0, 1,   0, 1, 1,   1, 1, 1,      // v4-v5-v0

						1, 1, 1,   0, 1, 1,   0, 1, 0,      // v0-v5-v6 (top)
						0, 1, 0,   1, 1, 0,   1, 1, 1,      // v6-v1-v0

						1, 1, 0,   0, 1, 0,   0, 0, 0,      // v1-v6-v7 (left)
						0, 0, 0,   1, 0, 0,   1, 1, 0,      // v7-v2-v1

						0, 0, 0,   0, 0, 1,   1, 0, 1,      // v7-v4-v3 (bottom)
						1, 0, 1,   1, 0, 0,   0, 0, 0,      // v3-v2-v7

						0, 0, 1,   0, 0, 0,   0, 1, 0,      // v4-v7-v6 (back)
						0, 1, 0,   0, 1, 1,   0, 0, 1 };    // v6-v5-v4

Cube::Cube()
{
	glGenBuffers(3, (GLuint *)m_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_bbox.extend(Eigen::Vector3f(-1, -1, -1));
	m_bbox.extend(Eigen::Vector3f(1, 1, 1));

	m_ready = false;

	m_material = new Material(Eigen::Vector3f(0.2f, 0.2f, 0.2f), Eigen::Vector3f(1.0f, 1.0f, 1.0f), Eigen::Vector3f(0.4f, 0.4f, 0.4f), 10.0f);
}

Cube::~Cube()
{
	//glDeleteBuffers(3,(const GLuint *)_vbo);
}

void Cube::init()
{
	glGenVertexArrays(1, (GLuint *)&m_vao);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);

	glBindVertexArray(0);

	m_ready = true;
}

void Cube::display(Shader *shader)
{
	if (!m_ready)
		init();

	glBindVertexArray(m_vao);

	// Recuperation des emplacements des deux attributs dans le shader
	int vertexPositionIndex = shader->getAttribLocation("vtx_position");
	int vertexColorIndex = shader->getAttribLocation("vtx_color");
	int vertexNormalIndex = shader->getAttribLocation("vtx_normal");

	if (vertexPositionIndex >= 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
		glVertexAttribPointer(vertexPositionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(vertexPositionIndex);
	}

	if (vertexColorIndex >= 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
		glVertexAttribPointer(vertexColorIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(vertexColorIndex);
	}
	if (vertexNormalIndex >= 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
		glVertexAttribPointer(vertexNormalIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(vertexNormalIndex);
	}

	glDrawArrays(GL_TRIANGLES, 0, 36);

	if (vertexPositionIndex >= 0)
		glDisableVertexAttribArray(vertexPositionIndex);
	if (vertexColorIndex >= 0)
		glDisableVertexAttribArray(vertexColorIndex);
	if (vertexNormalIndex >= 0)
		glDisableVertexAttribArray(vertexNormalIndex);

	glBindVertexArray(0);

	m_material->sendToShader(shader);
}
