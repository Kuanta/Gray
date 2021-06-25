#include "BoxGeometry.h"


BoxGeometry::BoxGeometry(float width, float height, float depth)
{
	float vertexPoints[] = {
		//Back
		width*0.5f, height*0.5f, -depth*0.5f,
		width*0.5f, -height*0.5f, -depth*0.5f,
		-width*0.5f, -height*0.5f, -depth*0.5f,
		-width*0.5f, height*0.5f, -depth*0.5f,
		//Bottom
		-width*0.5f, -height*0.5f, depth*0.5f,
		-width*0.5f, -height*0.5f, -depth*0.5f,
		width*0.5f, -height*0.5f, -depth*0.5f,
		width*0.5f, -height*0.5f, depth*0.5f,
		//Front
		-width*0.5f, height*0.5f, depth*0.5f,
		-width*0.5f, -height*0.5f, depth*0.5f,
		width*0.5f, -height*0.5f, depth*0.5f,
		width*0.5f, height*0.5f, depth*0.5f,
		//Left
		-width*0.5f, height*0.5f, -depth*0.5f,
		-width*0.5f, -height*0.5f, -depth*0.5f,
		-width*0.5f, -height*0.5f, depth*0.5f,
		-width*0.5f, height*0.5f, depth*0.5f,
		//Right
		width*0.5f, height*0.5f, depth*0.5f,
		width*0.5f, -height*0.5f, depth*0.5f,
		width*0.5f, -height*0.5f, -depth*0.5f,
		width*0.5f, height*0.5f, -depth*0.5f,
		//Top
		width*0.5f, height*0.5f, -depth*0.5f,
		width*0.5f, height*0.5f, depth*0.5f,
		-width*0.5f, height*0.5f, depth*0.5f,
		-width*0.5f, height*0.5f, -depth*0.5f,
	};
	float uvs[] = {
		0.0f,1.0f,0.0f, 0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,1.0f,0.0f, //Back
		0.0f,1.0f,0.0f, 0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,1.0f,0.0f, //Bottom
		0.0f,1.0f,0.0f, 0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,1.0f,0.0f, //Front
		0.0f,1.0f,0.0f, 0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,1.0f,0.0f, //Left
		0.0f,1.0f,0.0f, 0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,1.0f,0.0f, //Right
		0.0f,1.0f,0.0f, 0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,1.0f,0.0f, //Top
		
	};
	float normals[] = {
		//Back
		0,0,-1, 0,0,-1, 0,0,-1, 0,0,-1,
		//Bottom
		0,-1,0, 0,-1,0, 0,-1,0, 0,-1,0,
		//Front
		0,0,1, 0,0,1, 0,0,1, 0,0,1,
		//Left
		-1,0,0, -1,0,0, -1,0,0, -1,0,0,
		//Right
		1,0,0, 1,0,0 ,1,0,0, 1,0,0,
		//Top
		0,1,0, 0,1,0, 0,1,0, 0,1,0,
	};
	float tangents[] = {
		//Back
		1,0,0, 1,0,0, 1,0,0, 1,0,0,
		//Bottom
		1,0,0, 1,0,0, 1,0,0, 1,0,0,
		//Front
		1,0,0, 1,0,0, 1,0,0, 1,0,0,
		//Left
		0,0,1, 0,0,1, 0,0,1, 0,0,1,
		//Right
		0,0,-1, 0,0,-1 ,0,0,-1, 0,0,-1,
		//Top
		1,0,0, 1,0,0, 1,0,0, 1,0,0,
	};
	float colors[] = {
		0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f, 0.5f,0.5f,0.5f,
		0.9f,0.1f,0.5f, 0.9f,0.1f,0.5f, 0.9f,0.1f,0.5f, 0.9f,0.1f,0.5f,
		0.3f,0.8f,0.5f, 0.3f,0.8f,0.5f, 0.3f,0.8f,0.5f, 0.3f,0.8f,0.5f,
		0.0f,0.0f,0.9f, 0.0f,0.0f,0.9f, 0.0f,0.0f,0.9f, 0.0f,0.0f,0.9f,
		0.9f,0.9f,0.9f, 0.9f,0.9f,0.9f, 0.9f,0.9f,0.9f, 0.9f,0.9f,0.9f,
		0.0f,0.9f,0.0f, 0.0f,0.9f,0.0f, 0.0f,0.9f,0.0f, 0.0f,0.9f,0.0f,
	};
	//float vertexPoints[] = {
	//	0.5f,  0.5f, 0.0f,  // top right
	//	0.5f, -0.5f, 0.0f,  // bottom right
	//	-0.5f, -0.5f, 0.0f,  // bottom left
	//	-0.5f,  0.5f, 0.0f   // top left 
	//};
	unsigned int _indices[] = {
		0,1,3,1,2,3,
		4,5,7,5,6,7,
		8,9,11,9,10,11,
		12,13,15,13,14,15,
		16,17,19,17,18,19,
		20,21,23,21,22,23
	};
	//unsigned int _indices[] = { 0, 1, 2, 3, 0 , 2};
	int vertexCount = sizeof(vertexPoints) / sizeof(float);
	int indexCount = sizeof(_indices) / sizeof(unsigned int);

	for (int i = 0; i < vertexCount - 2; i += 3) {
		Vertex vert;
		vert.Position = glm::vec3(vertexPoints[i], vertexPoints[i + 1], vertexPoints[i + 2]);
		vert.UVs = glm::vec2(uvs[i], uvs[i+1]);
		vert.Normal = glm::vec3(normals[i], normals[i+1], normals[i+2]);
		vert.Color = glm::vec3(colors[i], colors[i+1], colors[i+2]);
		vert.Tangent = glm::vec3(tangents[i], tangents[i+1], tangents[i+2]);
		this->vertices.push_back(vert);
	}
	this->setIndices(_indices, indexCount);

	this->initBuffers();
}

BoxGeometry::~BoxGeometry()
{
}
