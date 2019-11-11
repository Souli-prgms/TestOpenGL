#include "Ground.h"


using namespace std;

Ground::Ground()
{
	setTexCoord(0, 0.f, 1.f);
	setPos(0, -1.f, -1.f);

	setTexCoord(1, 1.f, 1.f);
	setPos(1, 1.f, -1.f);

	setTexCoord(2, 1.f, 0.f);
	setPos(2, 1.f, 1.f);

	setTexCoord(3, 0.f, 0.f);
	setPos(3, -1.f, 1.f);

	// specification of the triangles indices
	m_indices[0] = 0;
	m_indices[1] = 1;
	m_indices[2] = 3;

	m_indices[3] = 1;
	m_indices[4] = 2;
	m_indices[5] = 3;

	m_bbox.extend(Eigen::Vector3f(-1, -1, 0));
	m_bbox.extend(Eigen::Vector3f(1, 1, 0));

	for (int i = 0; i < 12; ++i) {
		m_normal[i] = 0.0f;
		m_tangent[i] = 0.0f;
		m_bitangent[i] = 0.0f;
		
		if (i % 3 == 0)
			m_tangent[i] = 1.f;

		if (i + 1 % 3 == 0)
			m_normal[i] = 1.f;

		if (i + 2 % 3 == 0)
			m_bitangent[i] = 1.f;
	}

	this->setTransformationMatrix(this->getTransformationMatrix() * Eigen::AngleAxis<float>(M_PI_2, Eigen::Vector3f(1.0f, 0.0f, 0.0f)));
}

Ground::~Ground()
{
	if (m_ready) {
		glDeleteBuffers(6, m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}
}

void Ground::init()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(6, m_vbo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, m_indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, m_pos, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, m_texCoord, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, m_normal, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, m_tangent, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, m_bitangent, GL_STATIC_DRAW);

	glBindVertexArray(0);

	m_ready = true;
}


void Ground::setTexCoord(unsigned int id, float x, float y)
{
	if (id > 3)
		cerr << "Warning: Invalid texture coordinates" << endl;

	m_texCoord[2 * id] = x;
	m_texCoord[2 * id + 1] = y;
}


void Ground::setPos(unsigned int id, float x, float y)
{
	if (id > 3)
		cerr << "Warning: Invalid vertex coordinates" << endl;

	m_pos[2 * id] = x;
	m_pos[2 * id + 1] = y;
}


void Ground::display(Shader *shader)
{
	if (!m_ready)
		init();

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[0]);

	int vertex_loc = shader->getAttribLocation("vtx_position");
	if (vertex_loc >= 0) {
		glEnableVertexAttribArray(vertex_loc);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
		glVertexAttribPointer(vertex_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	int texCoord_loc = shader->getAttribLocation("vtx_texcoord");
	if (texCoord_loc >= 0) {
		glEnableVertexAttribArray(texCoord_loc);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
		glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	int normal_loc = shader->getAttribLocation("vtx_normal");
	if (normal_loc >= 0) {
		glEnableVertexAttribArray(normal_loc);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[3]);
		glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	int tangent_loc = shader->getAttribLocation("vtx_tangent");
	if (tangent_loc >= 0) {
		glEnableVertexAttribArray(tangent_loc);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[4]);
		glVertexAttribPointer(tangent_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	int bitangent_loc = shader->getAttribLocation("vtx_bitangent");
	if (bitangent_loc >= 0) {
		glEnableVertexAttribArray(bitangent_loc);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[5]);
		glVertexAttribPointer(bitangent_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	if (vertex_loc >= 0)
		glDisableVertexAttribArray(vertex_loc);
	if (texCoord_loc >= 0)
		glDisableVertexAttribArray(texCoord_loc);
	if (tangent_loc >= 0)
		glDisableVertexAttribArray(tangent_loc);
	if (normal_loc >= 0)
		glDisableVertexAttribArray(normal_loc);
	if (bitangent_loc >= 0)
		glDisableVertexAttribArray(bitangent_loc);

	glBindVertexArray(0);
}

void Ground::setWorldPos(int x, int y)
{
	this->setTransformationMatrix(this->getTransformationMatrix() * Eigen::Translation3f(float(x) * 2, float(y) * 2, 0.0));
}
