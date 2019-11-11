#pragma once

#include "Config.h"

struct DirectionalLight
{
	Eigen::Vector3f direction = Eigen::Vector3f(0.0f, -2.0f, -2.0f);
	Eigen::Vector3f ambient = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
	Eigen::Vector3f diffuse = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
	Eigen::Vector3f specular = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
};

struct PointLight
{
	Eigen::Vector3f position = Eigen::Vector3f(-2.0f, 4.0f, -1.0f);
	Eigen::Vector3f ambient = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
	Eigen::Vector3f diffuse = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
	Eigen::Vector3f specular = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
	float constant = 1.0f;
	float linear = 0.022f;
	float quadratic = 0.0019f;
};