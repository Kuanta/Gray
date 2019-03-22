#ifndef Geometry_H
#define Geometry_H

#define NUM_BONES_PER_VERTEX 4

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GLFW/glfw3.h>
#include <glad\glad.h>
#include "Shader.h"

using namespace std;


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
	int IDs[NUM_BONES_PER_VERTEX];
	//Bone Weights
	float Weights[NUM_BONES_PER_VERTEX];
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

class Geometry
{
public:
	Geometry(); // For derived classes
	Geometry(vector<Vertex> vertices, vector<unsigned int> indices); // For indexed geometries
	Geometry(vector<Vertex> vertices, unsigned int indices[]);
	Geometry(vector<Vertex> vertices); // For non indexed geometries
	~Geometry();
	unsigned int VAO, VBO, EBO;
	void setIndices(unsigned int indices[], int indicesSize);
	void initBuffers();
	void draw();
	void cleanup();
	
protected:
	bool indexed;
	vector<Vertex> vertices;
	vector<unsigned int> indices;

};

#endif

