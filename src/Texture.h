#pragma once

#include "Config.h"
#include "SOIL.h"

class Texture
{
public:
	Texture(const std::string& filename);
	~Texture();

	unsigned int getId() { return m_id; }
	void setParameters();


private:
	int m_width, m_height, m_nbChannels;
	unsigned char* m_data;
	unsigned int m_id;
};