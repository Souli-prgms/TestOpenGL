#pragma once

#include "Shape.h"
#include "Material.h"

class Sphere : public Shape {

public:
	Sphere(float radius = 1.f, int nU = 40, int nV = 40);
	virtual ~Sphere();
	void init();
	void display(Shader *shader);
	float radius() const { return m_radius; }

private:
	GLuint m_vao;
	GLuint m_vbo[9];

	std::vector<int>                m_indices;   /** vertex indices */
	std::vector<Eigen::Vector3f>	m_vertices;  /** 3D positions */
	std::vector<Eigen::Vector3f>	m_kas;
	std::vector<Eigen::Vector3f>	m_kds;
	std::vector<Eigen::Vector3f>	m_kss;
	std::vector<Eigen::Vector3f>	m_normals;   /** 3D normals */
	std::vector<Eigen::Vector3f>	m_tangents;  /** 3D tangent to surface */
	std::vector<Eigen::Vector3f>    m_bitangents;
	std::vector<Eigen::Vector2f>	m_texCoords; /** 2D texture coordinates */

	float m_radius;
	Material* m_material;
};
