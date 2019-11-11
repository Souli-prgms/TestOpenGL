#pragma once

#include "Config.h"
#include "Shape.h"
#include "Shader.h"
#include "MeshLoader.h"
#include "Material.h"

class Mesh: public Shape
{
public:
	Mesh();
	~Mesh();

	void load(const std::string& filename);
	void init();
	void display(Shader *shader);

private:
	struct Vertex
	{
		Vertex(const Eigen::Vector3f& pos, const Eigen::Vector3f& ka, const Eigen::Vector3f& kd, const Eigen::Vector3f& ks,
			const Eigen::Vector3f& n, const Eigen::Vector2f& tex) : m_position(pos), m_ka(ka), m_kd(kd), m_ks(ks),
			m_normal(n), m_texcoord(tex)
		{}

		Eigen::Vector3f m_position;
		Eigen::Vector3f m_ka;
		Eigen::Vector3f m_kd;
		Eigen::Vector3f m_ks;
		Eigen::Vector3f m_normal;
		Eigen::Vector3f m_tangent;
		Eigen::Vector3f m_bitangent;
		Eigen::Vector2f m_texcoord;
	};


	GLuint m_vao;
	GLuint m_vbo[2];

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
};
