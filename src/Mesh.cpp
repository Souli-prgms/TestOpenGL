#include "Mesh.h"

using namespace std;
using namespace Eigen;

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
	
}


void Mesh::load(const string& filename)
{
	vector<Vector3f> positionTmp;
	vector<Vector3i> faceId;
	vector<Vector3f> normalTmp;
	vector<Vector3i> normalId;
	vector<Vector2f> texcoordTmp;
	vector<Vector3i> texId;
	vector<Vector4f> colorTmp;
	vector<Vector3f> Kas;
	vector<Vector3f> Kds;
	vector<Vector3f> Kss;

	cout << "Load " << filename << endl;

	try {
		std::string ext = filename.substr(filename.size() - 3, 3);
		if (ext == "obj" || ext == "OBJ")
			MeshLoader::loadObj(filename, positionTmp, faceId, normalTmp, normalId, texcoordTmp, texId, Kas, Kds, Kss);
		else
			cerr << "Mesh: extension \'" << ext << "\' not supported." << endl;
	}
	catch (MeshLoaderException e) {
		cerr << e.getMessage();
		return;
	}

	if (normalId.size() == 0) // vertex normals not available, compute them from the faces normal
	{
		// pass 1: set the normal to 0
		normalTmp.resize(positionTmp.size(), Vector3f(0.f, 0.f, 0.f));

		// pass 2: compute face normals and accumulate
		for (unsigned int i = 0; i < faceId.size(); i++)
		{
			Vector3f v0 = positionTmp[(faceId[i])[0]];
			Vector3f v1 = positionTmp[(faceId[i])[1]];
			Vector3f v2 = positionTmp[(faceId[i])[2]];

			Vector3f n = (v1 - v0).cross(v2 - v0);

			normalTmp[(faceId[i])[0]] += n;
			normalTmp[(faceId[i])[1]] += n;
			normalTmp[(faceId[i])[2]] += n;
		}

		// pass 3: normalize
		for (vector<Vector3f>::iterator n_iter = normalTmp.begin(); n_iter != normalTmp.end(); ++n_iter)
			(*n_iter).normalize();

		normalId = faceId;
	}

	Vector3f pos;
	Vector2f tex;
	Vector3f norm;
	unsigned int size = faceId.size();
	m_vertices.reserve(3 * size);

	for (unsigned int i = 0; i < size; i++) {

		for (unsigned int j = 0; j < 3; j++) {
			pos = positionTmp[(faceId[i])[j]];
			if (i < texId.size())
				tex = texcoordTmp[(texId[i])[j]];
			if (i < normalId.size())
				norm = normalTmp[(normalId[i])[j]];
			
	
			m_vertices.push_back(Vertex(pos, Kas[i], Kds[i], Kss[i], norm, tex));
			m_indices.push_back(3 * i + j);
			m_bbox.extend(pos);
		}
	}
}

void Mesh::init()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(2, m_vbo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*m_vertices.size(), m_vertices[0].m_position.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	m_ready = true;
}

void Mesh::display(Shader *shader)
{
	if (!m_ready)
		init();

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);

	std::size_t index = 0;

	int vertex_loc = shader->getAttribLocation("vtx_position");
	if (vertex_loc >= 0) {
		glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)index);
		glEnableVertexAttribArray(vertex_loc);
	}

	index += sizeof(Vector3f);

	int ka_loc = shader->getAttribLocation("vtx_ka");
	if (ka_loc >= 0) {
		glVertexAttribPointer(ka_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)index);
		glEnableVertexAttribArray(ka_loc);
	}

	index += sizeof(Vector3f);

	int kd_loc = shader->getAttribLocation("vtx_kd");
	if (kd_loc >= 0) {
		glVertexAttribPointer(kd_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)index);
		glEnableVertexAttribArray(kd_loc);
	}

	index += sizeof(Vector3f);

	int ks_loc = shader->getAttribLocation("vtx_ks");
	if (ks_loc >= 0) {
		glVertexAttribPointer(ks_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)index);
		glEnableVertexAttribArray(ks_loc);
	}

	index += sizeof(Vector3f);

	int normal_loc = shader->getAttribLocation("vtx_normal");
	if (normal_loc >= 0) {
		glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)index);
		glEnableVertexAttribArray(normal_loc);
	}

	index += sizeof(Vector3f);

	int tangent_loc = shader->getAttribLocation("vtx_tangent");
	if (tangent_loc >= 0) {
		glVertexAttribPointer(tangent_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)index);
		glEnableVertexAttribArray(tangent_loc);
	}

	index += sizeof(Vector3f);

	int bitangent_loc = shader->getAttribLocation("vtx_bitangent");
	if (bitangent_loc >= 0) {
		glVertexAttribPointer(bitangent_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)index);
		glEnableVertexAttribArray(bitangent_loc);
	}

	index += sizeof(Vector3f);

	int texCoord_loc = shader->getAttribLocation("vtx_texcoord");
	if (texCoord_loc >= 0) {
		glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)index);
		glEnableVertexAttribArray(texCoord_loc);
	}

	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

	if (vertex_loc >= 0)
		glDisableVertexAttribArray(vertex_loc);
	if (ka_loc >= 0)
		glDisableVertexAttribArray(ka_loc);
	if (kd_loc >= 0)
		glDisableVertexAttribArray(kd_loc);
	if (ks_loc >= 0)
		glDisableVertexAttribArray(ks_loc);
	if (normal_loc >= 0)
		glDisableVertexAttribArray(normal_loc);
	if (tangent_loc >= 0)
		glDisableVertexAttribArray(tangent_loc);
	if (bitangent_loc >= 0)
		glDisableVertexAttribArray(bitangent_loc);
	if (texCoord_loc >= 0)
		glDisableVertexAttribArray(texCoord_loc);

	glBindVertexArray(0);
}
