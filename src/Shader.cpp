#include "Shader.h"

Shader::Shader(const std::string& filepath)
{
	parseShader(filepath);
	createShader();
}

Shader::~Shader()
{
	glDeleteProgram(m_id);
}

void Shader::parseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}

			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}

	m_vertexSource = ss[0].str();
	m_fragmentSource = ss[1].str();
}

unsigned int Shader::compileShader(unsigned int typeconst, const std::string& source) {
	unsigned int id = glCreateShader(typeconst);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int lenght;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
		char* message = (char*)alloca(lenght * sizeof(char));
		glGetShaderInfoLog(id, lenght, &lenght, message);
		std::cout << "Failed to compile " << (typeconst == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

void Shader::createShader() {
	m_id = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, m_vertexSource);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, m_fragmentSource);

	glAttachShader(m_id, vs);
	glAttachShader(m_id, fs);
	glLinkProgram(m_id);
	glValidateProgram(m_id);

	glDeleteShader(fs);
	glDeleteShader(vs);
}

void Shader::bind()
{
	glUseProgram(m_id);
}

void Shader::unbind()
{
	glUseProgram(0);
}

int Shader::getAttribLocation(const char* name) const
{
	return glGetAttribLocation(m_id, name);
}

void Shader::setVec3(const char* name, Eigen::Vector3f vector) const
{
	glUniform3f(glGetUniformLocation(m_id, name), vector(0), vector(1), vector(2));
}

void Shader::setVec4(const char* name, Eigen::Vector4f vector) const
{
	glUniform4f(glGetUniformLocation(m_id, name), vector(0), vector(1), vector(2), vector(3));
}


void Shader::setFloat(const char* name, float f) const
{
	glUniform1f(glGetUniformLocation(m_id, name), f);
}

void Shader::setMat3(const char* name, Eigen::Matrix3f mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, mat.data());
}

void Shader::setMat4(const char* name, Eigen::Matrix4f mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, mat.data());
}

void Shader::setInt(const char* name, unsigned int i) const
{
	glUniform1i(glGetUniformLocation(m_id, name), i);
}