#include "Texture.h"

Texture::Texture(const std::string& filename)
{
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	this->setParameters();
	m_data = SOIL_load_image(filename.c_str(), &m_width, &m_height, &m_nbChannels, 0);


	if (m_data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	else
	{
		std::cout << "Failed to load texture: " << filename << std::endl;
	}
	
	SOIL_free_image_data(m_data);
}

void Texture::setParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
}