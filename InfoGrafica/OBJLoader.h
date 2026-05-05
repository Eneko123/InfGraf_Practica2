#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
typedef struct {
	glm::vec4 vPos;
	glm::vec4 vNorm;
	glm::vec4 vTangent;
	glm::vec4 vBoneWeights;
	glm::vec4 vBoneIdxs;
	glm::vec2 vTextCoords;
} vertex_t;
bool LoadOBJ(
	const std::string& path,
	std::vector<vertex_t>& outVertices,
	std::vector<GLuint>& outIndices,
	bool recalculateNormalsIfMissing = true
);