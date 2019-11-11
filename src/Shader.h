#pragma once

#include "Config.h"

class Shader
{
public:
	Shader(const std::string& filepath);
	~ Shader();

	void bind();
	void unbind();

	int getAttribLocation(const char* name) const;

	void setFloat(const char* name, float f) const;
	void setInt(const char* name, unsigned int i) const;
	void setVec3(const char* name, Eigen::Vector3f vector) const;
	void setVec4(const char* name, Eigen::Vector4f vector) const;
	void setMat3(const char* name, Eigen::Matrix3f mat) const;
	void setMat4(const char* name, Eigen::Matrix4f mat) const;


private:
	void parseShader(const std::string& filepath);
	unsigned int compileShader(unsigned int typeconst, const std::string& source);
	void createShader();

	std::string m_vertexSource;
	std::string m_fragmentSource;
	unsigned int m_id;
};








