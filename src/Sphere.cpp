#include "Sphere.h"

using namespace Eigen;

Sphere::Sphere(float radius, int nU, int nV) :
	m_radius(radius)
{
									
	m_material = new Material(Eigen::Vector3f(0.0215f, 0.1745f, 0.0215f), Eigen::Vector3f(0.07568f, 0.61424f, 0.07568f), Eigen::Vector3f(0.633f, 0.727811f, 0.633f), 10.0f);
	int nVertices = (nU + 1) * (nV + 1);
	int nTriangles = nU * nV * 2;

	m_vertices.resize(nVertices);
	m_normals.resize(nVertices);
	m_bitangents.resize(nVertices);
	m_tangents.resize(nVertices);
	m_texCoords.resize(nVertices);
	m_kas.resize(nVertices);
	m_kds.resize(nVertices);
	m_kss.resize(nVertices);
	m_indices.resize(3 * nTriangles);

	for (int v = 0; v <= nV; ++v)
	{
		for (int u = 0; u <= nU; ++u)
		{
			float theta = u / float(nU) * M_PI;
			float phi = v / float(nV) * M_PI * 2;

			int index = u + (nU + 1)*v;

			Vector3f vertex, tangent, normal, bitangent;
			Vector2f texCoord;

			// normal
			normal[0] = sin(theta) * cos(phi);
			normal[1] = sin(theta) * sin(phi);
			normal[2] = cos(theta);
			normal.normalize();

			// position
			vertex = normal * m_radius;

			// tangent
			theta += M_PI;
			tangent[0] = sin(theta) * cos(phi);
			tangent[1] = sin(theta) * sin(phi);
			tangent[2] = cos(theta);
			tangent.normalize();

			//bitangent
			bitangent = normal.cross(tangent);
			bitangent.normalize();

			// texture coordinates
			texCoord[1] = u / float(nU);
			texCoord[0] = v / float(nV);

			m_vertices[index] = vertex;
			m_normals[index] = normal;
			m_tangents[index] = tangent;
			m_bitangents[index] = bitangent;
			m_texCoords[index] = texCoord;
			m_kas[index] = m_material->getAmbient();
			m_kds[index] = m_material->getDiffuse();
			m_kss[index] = m_material->getSpecular();
			m_bbox.extend(vertex);
		}
	}

	int index = 0;
	for (int v = 0; v < nV; ++v)
	{
		for (int u = 0; u < nU; ++u)
		{
			int vindex = u + (nU + 1)*v;

			m_indices[index + 0] = vindex;
			m_indices[index + 1] = vindex + 1;
			m_indices[index + 2] = vindex + 1 + (nU + 1);

			m_indices[index + 3] = vindex;
			m_indices[index + 4] = vindex + 1 + (nU + 1);
			m_indices[index + 5] = vindex + (nU + 1);

			index += 6;
		}
	}
}

Sphere::~Sphere()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(9, m_vbo);
}

void Sphere::init()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(9, m_vbo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * m_kas.size(), m_kas.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * m_kds.size(), m_kds.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * m_kss.size(), m_kss.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * m_normals.size(), m_normals.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * m_tangents.size(), m_tangents.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3f) * m_bitangents.size(), m_bitangents.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[8]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2f) * m_texCoords.size(), m_texCoords.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	m_ready = true;
}

void Sphere::display(Shader *shader)
{
	if (!m_ready)
		init();

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[0]);

	int vertex_loc = shader->getAttribLocation("vtx_position");
	glEnableVertexAttribArray(vertex_loc);
	if (vertex_loc >= 0) {
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
		glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	int ka_loc = shader->getAttribLocation("vtx_ka");
	if (ka_loc >= 0) {
		glEnableVertexAttribArray(ka_loc);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
		glVertexAttribPointer(ka_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	int kd_loc = shader->getAttribLocation("vtx_kd");
	if (kd_loc >= 0) {
		glEnableVertexAttribArray(kd_loc);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[3]);
		glVertexAttribPointer(kd_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	int ks_loc = shader->getAttribLocation("vtx_ks");
	if (ks_loc >= 0) {
		glEnableVertexAttribArray(ks_loc);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[4]);
		glVertexAttribPointer(ks_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}


	int normal_loc = shader->getAttribLocation("vtx_normal");
	if (normal_loc >= 0) {
		glEnableVertexAttribArray(normal_loc);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[5]);
		glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	int tangent_loc = shader->getAttribLocation("vtx_tangent");
	if (tangent_loc >= 0) {
		glEnableVertexAttribArray(tangent_loc);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[6]);
		glVertexAttribPointer(tangent_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	int bitangent_loc = shader->getAttribLocation("vtx_bitangent");
	if (bitangent_loc >= 0) {
		glEnableVertexAttribArray(bitangent_loc);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[7]);
		glVertexAttribPointer(bitangent_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	int texCoord_loc = shader->getAttribLocation("vtx_texcoord");
	if (texCoord_loc >= 0) {
		glEnableVertexAttribArray(texCoord_loc);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[8]);
		glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

	if (vertex_loc >= 0)
		glDisableVertexAttribArray(vertex_loc);
	if (ka_loc >= 0)
		glDisableVertexAttribArray(ka_loc);
	if (kd_loc >= 0)
		glDisableVertexAttribArray(kd_loc);
	if (ks_loc >= 0)
		glDisableVertexAttribArray(ks_loc);
	if (normal_loc >= 0)
		glDisableVertexAttribArray(normal_loc);
	if (tangent_loc >= 0)
		glDisableVertexAttribArray(tangent_loc);
	if (bitangent_loc >= 0)
		glDisableVertexAttribArray(bitangent_loc);
	if (texCoord_loc >= 0)
		glDisableVertexAttribArray(texCoord_loc);

	glBindVertexArray(0);
}
