#pragma once

#include "Config.h"
#include "Shader.h"

class Shape 
{
public:
	Shape() : m_ready(false), m_transformation(Eigen::Matrix4f::Identity()) {}
	virtual ~Shape() {}

	virtual void init() = 0;
	virtual void display(Shader *shader) = 0;

	const Eigen::AlignedBox3f& boundingBox() const { return m_bbox; }

	const Eigen::Affine3f& getTransformationMatrix() const { return m_transformation; }
	void setTransformationMatrix(const Eigen::Affine3f& transfo) { m_transformation = transfo; }

protected:
	bool m_ready;
	Eigen::AlignedBox3f m_bbox;
	Eigen::Affine3f m_transformation;
};