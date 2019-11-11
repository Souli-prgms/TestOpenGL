#pragma once

#include "Config.h"
#include "Shader.h"

class Material
{
public:
	Material(Eigen::Vector3f ambient, Eigen::Vector3f diffuse, Eigen::Vector3f specular, float shininess) :
		m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess) {};

	Material(){}


	void sendToShader(Shader* shader)
	{
		shader->setVec3("material.ambient", m_ambient);
		shader->setVec3("material.diffuse", m_diffuse);
		shader->setVec3("material.specular", m_specular);
		shader->setFloat("material.shininess", m_shininess);
	}

	void setAmbient(Eigen::Vector3f ambient) { m_ambient = ambient; }
	void setDiffuse(Eigen::Vector3f diffuse) { m_diffuse = diffuse; }
	void setSpecular(Eigen::Vector3f specular) { m_specular = specular; }
	void setShininess(float shininess) { m_shininess = shininess; }

	Eigen::Vector3f getAmbient() { return m_ambient; }
	Eigen::Vector3f getDiffuse() { return m_diffuse; }
	Eigen::Vector3f getSpecular() { return m_specular; }

private:
	Eigen::Vector3f m_ambient;
	Eigen::Vector3f m_diffuse;
	Eigen::Vector3f m_specular;
	float m_shininess;
};

