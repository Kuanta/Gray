#ifndef Geometry_H
#define Geometry_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "GrBone.h"
#include "GrSkeleton.h"

using namespace std;

struct Vertex;

class Geometry
{
public:
	Geometry(); // For derived classes
	Geometry(vector<Vertex> vertices, vector<unsigned int> indices, bool initBuffers = true); // For indexed geometries
	Geometry(vector<Vertex> vertices, unsigned int indices[], bool initBuffers = true);
	Geometry(vector<Vertex> vertices, bool initBuffers = true); // For non indexed geometries
	~Geometry();
	bool hasBones = false;
	vector<Vertex> vertices;
	unsigned int VAO, VBO, EBO;
	void setIndices(unsigned int indices[], int indicesSize);
	void initBuffers();
	void draw(Shader* shader);
	Geometry* clone();
	void cleanup();
	const static int MAX_BONE = 100;
	const static int NUM_BONES_PER_VERTEX = 4;
protected:
	bool indexed;
	vector<unsigned int> indices;
	void initGeometry(vector<Vertex> vertices, bool initBuffers);

};

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 UVs;
	glm::vec2 UV2s;
	// Color
	glm::vec3 Color;
	//Bone IDs
	int IDs[Geometry::NUM_BONES_PER_VERTEX];
	//Bone Weights
	float Weights[Geometry::NUM_BONES_PER_VERTEX];
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};
#endif

