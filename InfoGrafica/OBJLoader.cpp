#include "OBJLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
struct OBJIndex
{
	int v = -1;
	int vt = -1;
	int vn = -1;
	bool operator==(const OBJIndex& other) const
	{
		return v == other.v && vt == other.vt && vn == other.vn;
	}
};
struct OBJIndexHasher
{
	std::size_t operator()(const OBJIndex& k) const
	{
		std::size_t h1 = std::hash<int>()(k.v);
		std::size_t h2 = std::hash<int>()(k.vt);
		std::size_t h3 = std::hash<int>()(k.vn);
		return h1 ^ (h2 << 1) ^ (h3 << 2);
	}
};
static bool ParseFaceVertex(const std::string& token, OBJIndex& out)
{
	out = {};
	size_t firstSlash = token.find('/');
	if (firstSlash == std::string::npos)
	{
		out.v = std::stoi(token);
		return true;
	}
	size_t secondSlash = token.find('/', firstSlash + 1);
	if (secondSlash == std::string::npos)
	{
		std::string sV = token.substr(0, firstSlash);
		std::string sVT = token.substr(firstSlash + 1);
		if (!sV.empty()) out.v = std::stoi(sV);
		if (!sVT.empty()) out.vt = std::stoi(sVT);
		return true;
	}
	std::string sV = token.substr(0, firstSlash);
	std::string sVT = token.substr(firstSlash + 1, secondSlash - firstSlash - 1);
	std::string sVN = token.substr(secondSlash + 1);
	if (!sV.empty()) out.v = std::stoi(sV);
	if (!sVT.empty()) out.vt = std::stoi(sVT);
	if (!sVN.empty()) out.vn = std::stoi(sVN);
	return true;
}
static int FixOBJIndex(int idx, int size)
{
	if (idx > 0) return idx - 1;
	if (idx < 0) return size + idx;
	return -1;
}
static void RecalculateNormals(
	std::vector<vertex_t>& vertices,
	const std::vector<GLuint>& indices
)
{
	for (size_t i = 0; i < vertices.size(); ++i)
		vertices[i].vNorm = glm::vec4(0.0f);
	for (size_t i = 0; i + 2 < indices.size(); i += 3)
	{
		GLuint ia = indices[i];
		GLuint ib = indices[i + 1];
		GLuint ic = indices[i + 2];
		glm::vec3 a = glm::vec3(vertices[ia].vPos);
		glm::vec3 b = glm::vec3(vertices[ib].vPos);
		glm::vec3 c = glm::vec3(vertices[ic].vPos);
		glm::vec3 edge1 = b - a;
		glm::vec3 edge2 = c - a;
		glm::vec3 n = glm::normalize(glm::cross(edge1, edge2));
		vertices[ia].vNorm += glm::vec4(n, 0.0f);
		vertices[ib].vNorm += glm::vec4(n, 0.0f);
		vertices[ic].vNorm += glm::vec4(n, 0.0f);
	}
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		glm::vec3 n = glm::normalize(glm::vec3(vertices[i].vNorm));
		vertices[i].vNorm = glm::vec4(n, 0.0f);
	}
}
bool LoadOBJ(
	const std::string& path,
	std::vector<vertex_t>& outVertices,
	std::vector<GLuint>& outIndices,
	bool recalculateNormalsIfMissing
)
{
	outVertices.clear();
	outIndices.clear();
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cout << "No se pudo abrir el OBJ: " << path << std::endl;
		return false;
	}
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec3> normals;
	std::unordered_map<OBJIndex, GLuint, OBJIndexHasher> uniqueVertices;
	bool hasNormals = false;
	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty() || line[0] == '#')
			continue;
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;
		if (prefix == "v")
		{
			glm::vec3 p;
			iss >> p.x >> p.y >> p.z;
			positions.push_back(p);
		}
		else if (prefix == "vt")
		{
			glm::vec2 uv;
			iss >> uv.x >> uv.y;
			uv.y = 1.0f - uv.y;
			texcoords.push_back(uv);
		}
		else if (prefix == "vn")
		{
			glm::vec3 n;
			iss >> n.x >> n.y >> n.z;
			normals.push_back(glm::normalize(n));
			hasNormals = true;
		}
		else if (prefix == "f")
		{
			std::vector<OBJIndex> face;
			std::string token;
			while (iss >> token)
			{
				OBJIndex idx;
				ParseFaceVertex(token, idx);
				face.push_back(idx);
			}
			if (face.size() < 3)
				continue;
			for (size_t i = 1; i + 1 < face.size(); ++i)
			{
				OBJIndex tri[3] = { face[0], face[i], face[i + 1] };
				for (int k = 0; k < 3; ++k)
				{
					int posIndex = FixOBJIndex(tri[k].v, (int)positions.size());
					int uvIndex = FixOBJIndex(tri[k].vt, (int)texcoords.size());
					int nrmIndex = FixOBJIndex(tri[k].vn, (int)normals.size());
					if (posIndex < 0 || posIndex >= (int)positions.size())
						return false;
					OBJIndex fixedKey;
					fixedKey.v = posIndex;
					fixedKey.vt = uvIndex;
					fixedKey.vn = nrmIndex;
					auto it = uniqueVertices.find(fixedKey);
					if (it != uniqueVertices.end())
					{
						outIndices.push_back(it->second);
					}
					else
					{
						vertex_t vert{};
						glm::vec3 pos = positions[posIndex];
						glm::vec2 uv(0.0f, 0.0f);
						glm::vec3 norm(0.0f, 0.0f, 0.0f);
						if (uvIndex >= 0 && uvIndex < (int)texcoords.size())
							uv = texcoords[uvIndex];
						if (nrmIndex >= 0 && nrmIndex < (int)normals.size())
							norm = normals[nrmIndex];
						vert.vPos = glm::vec4(pos, 1.0f);
						vert.vNorm = glm::vec4(norm, 0.0f);
						vert.vTangent = glm::vec4(0.0f);
						vert.vBoneWeights = glm::vec4(0.0f);
						vert.vBoneIdxs = glm::vec4(0.0f);
						vert.vTextCoords = uv;
						GLuint newIndex = (GLuint)outVertices.size();
						outVertices.push_back(vert);
						outIndices.push_back(newIndex);
						uniqueVertices[fixedKey] = newIndex;
					}
				}
			}
		}
	}
	if (!hasNormals && recalculateNormalsIfMissing)
		RecalculateNormals(outVertices, outIndices);
	return true;
}