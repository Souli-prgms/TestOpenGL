#pragma once

#include "Config.h"
#include "Material.h"

class MeshLoaderException
{
public:
	MeshLoaderException(const std::string & msg) : message(msg) {}
	virtual  ~MeshLoaderException() {}
	const std::string getMessage() const { return "[MeshLoaderException] " + message; }

private:
	std::string message;
};


class MeshLoader
{
public:

	static void loadObj(const std::string& filename,
		std::vector<Eigen::Vector3f>& position,
		std::vector<Eigen::Vector3i>& faceId,
		std::vector<Eigen::Vector3f>& normal,
		std::vector<Eigen::Vector3i>& normalId,
		std::vector<Eigen::Vector2f>& texcoord,
		std::vector<Eigen::Vector3i>& texId,
		std::vector<Eigen::Vector3f>& Kas,
		std::vector<Eigen::Vector3f>& Kds,
		std::vector<Eigen::Vector3f>& Kss
		);

	static std::map<std::string, Material> loadMtl(const std::string& filename);
};
