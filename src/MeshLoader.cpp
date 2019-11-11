#include "MeshLoader.h"

using namespace std;
using namespace Eigen;

void MeshLoader::loadObj(const string&  filename,
	vector<Vector3f>& position,
	vector<Vector3i>& faceId,
	vector<Vector3f>& normal,
	vector<Vector3i>& normalId,
	vector<Vector2f>& texcoord,
	vector<Vector3i>& texId,
	vector<Eigen::Vector3f>& Kas,
	vector<Eigen::Vector3f>& Kds,
	vector<Eigen::Vector3f>& Kss)
{
	map<string, Material> materials = loadMtl("res/models/sportsCar.mtl");
	string currentMaterial;
	ifstream input(filename.c_str());

	if (!input)
		throw MeshLoaderException("loadObj : cannot load file :" + filename);

	char line[1024];

	while (!input.eof()) {

		if (!input.getline(line, 1024, '\n').good())
			continue;

		istringstream line_input(line);
		string keyword;

		line_input >> keyword;

		if (keyword == "v") { // vertex position
			Vector3f p;
			line_input >> p[0] >> p[1] >> p[2];
			position.push_back(p);
		}

		else if (keyword == "usemtl")
		{
			line_input >> currentMaterial;
		}

		else if (keyword == "vt") { // vertex texture coordinate
			Vector2f p;
			line_input >> p[0] >> p[1];
			texcoord.push_back(p);
		}
		else if (keyword == "vn") { // vertex normal
			Vector3f n;
			line_input >> n[0] >> n[1] >> n[2];
			normal.push_back(n);
		}
		else if (keyword == "f") { // face
			// vertex_id / vertex texcoord / vertex normal
			Vector3i fId;
			Vector3i tId;
			Vector3i nId;

			Vector4i fIdt;
			Vector4i nIdt;

			if (sscanf(line, "f %d//%d %d//%d %d//%d %d//%d", &fIdt[0], &nIdt[0], &fIdt[1], &nIdt[1], &fIdt[2], &nIdt[2], &fIdt[3], &nIdt[3]) == 8)
			{
				for (int i = 0; i < 4; ++i) {
					fIdt[i] -= 1;
					nIdt[i] -= 1;
				}

				faceId.push_back(Vector3i(fIdt(0), fIdt(1), fIdt(2)));
				faceId.push_back(Vector3i(fIdt(0), fIdt(2), fIdt(3)));
				normalId.push_back(Vector3i(nIdt(0), nIdt(1), nIdt(2)));
				normalId.push_back(Vector3i(nIdt(0), nIdt(2), nIdt(3)));
				Kas.push_back(materials[currentMaterial].getAmbient());
				Kds.push_back(materials[currentMaterial].getDiffuse());
				Kss.push_back(materials[currentMaterial].getSpecular());
				Kas.push_back(materials[currentMaterial].getAmbient());
				Kds.push_back(materials[currentMaterial].getDiffuse());
				Kss.push_back(materials[currentMaterial].getSpecular());
			}


			else if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
				&fId[0], &tId[0], &nId[0],
				&fId[1], &tId[1], &nId[1],
				&fId[2], &tId[2], &nId[2]) == 9) {
				for (int i = 0; i < 3; ++i) {
					fId[i] -= 1;
					tId[i] -= 1;
					nId[i] -= 1;
				}
				faceId.push_back(fId);
				texId.push_back(tId);
				normalId.push_back(nId);
				Kas.push_back(materials[currentMaterial].getAmbient());
				Kds.push_back(materials[currentMaterial].getDiffuse());
				Kss.push_back(materials[currentMaterial].getSpecular());
			}
			else if (sscanf(line, "f %d//%d %d//%d %d//%d",
				&fId[0], &nId[0],
				&fId[1], &nId[1],
				&fId[2], &nId[2]) == 6) {
				for (int i = 0; i < 3; ++i) {
					fId[i] -= 1;
					nId[i] -= 1;
				}
				faceId.push_back(fId);
				normalId.push_back(nId);
				Kas.push_back(materials[currentMaterial].getAmbient());
				Kds.push_back(materials[currentMaterial].getDiffuse());
				Kss.push_back(materials[currentMaterial].getSpecular());
			}
			else if (sscanf(line, "f %d// %d// %d//",
				&fId[0],
				&fId[1],
				&fId[2]) == 3) {
				for (int i = 0; i < 3; ++i) {
					fId[i] -= 1;
				}
				faceId.push_back(fId);
				Kas.push_back(materials[currentMaterial].getAmbient());
				Kds.push_back(materials[currentMaterial].getDiffuse());
				Kss.push_back(materials[currentMaterial].getSpecular());
			}
			else if (sscanf(line, "f %d %d %d",
				&fId[0],
				&fId[1],
				&fId[2]) == 3) {
				for (int i = 0; i < 3; ++i) {
					fId[i] -= 1;
				}
				faceId.push_back(fId);
				Kas.push_back(materials[currentMaterial].getAmbient());
				Kds.push_back(materials[currentMaterial].getDiffuse());
				Kss.push_back(materials[currentMaterial].getSpecular());
			}
			else { cerr << "error reading line -> \"" << line << "\"" << endl; }
		}
	}

	input.close();

}

map<string, Material> MeshLoader::loadMtl(const string& filename)
{
	map<string, Material> materials;

	ifstream input(filename.c_str());

	if (!input)
		throw MeshLoaderException("loadMtl : cannot load file :" + filename);

	char line[1024];
	string currentPartName;
	Material currentMaterial;
	currentMaterial.setShininess(32.0f);

	while (!input.eof()) {

		if (!input.getline(line, 1024, '\n').good())
			continue;

		istringstream line_input(line);
		string keyword;

		line_input >> keyword;

		if (keyword == "newmtl") {
			line_input >> currentPartName;
		}

		else if (keyword == "Ka")
		{
			Vector3f Ka;
			line_input >> Ka[0] >> Ka[1] >> Ka[2];
			currentMaterial.setAmbient(Ka);
		}

		else if (keyword == "Kd")
		{
			Vector3f Kd;
			line_input >> Kd[0] >> Kd[1] >> Kd[2];
			currentMaterial.setDiffuse(Kd);
		}

		else if (keyword == "Ks")
		{
			Vector3f Ks;
			line_input >> Ks[0] >> Ks[1] >> Ks[2];
			currentMaterial.setSpecular(Ks);
			materials[currentPartName] = currentMaterial;
		}
	}

	return materials;
}


