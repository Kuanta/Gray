#include "Plane.h"

Plane::Plane(float width, float height, float uvScale)
{
	this->width = width;
	this->height = height;
	float vertexPoints[] = {
		-0.5f * width, -0.5 * height, 0,
		0.5f * width, -0.5 * height, 0,
		-0.5 * width, 0.5 * height, 0,
		0.5 * width, 0.5 * height, 0
	};
	float uvs[] = {
		0.0f, 0.0f,0.0f,
		1.0f*uvScale, 0.0f,0.0f,
		0.0f, 1.0f * uvScale,0.0f,
		1.0f*uvScale, 1.0f * uvScale,0.0f
	};
	float normals[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};
	float tangents[] = 
	{
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};
	unsigned int indices[] = {
		0, 1, 2, 1, 2, 3
	};
	int vertexCount = 12;
	int indexCount = 6;
	for (int i = 0; i < vertexCount - 2; i += 3)
	{
		Vertex vert;
		vert.Position = glm::vec3(vertexPoints[i], vertexPoints[i + 1], vertexPoints[i + 2]);
		vert.UVs = glm::vec2(uvs[i], uvs[i + 1]);
		vert.Normal = glm::vec3(normals[i], normals[i + 1], normals[i + 2]);
		vert.Tangent = glm::vec3(tangents[i], tangents[i+1], tangents[i+2]);
		this->vertices.push_back(vert);
	}
	this->setIndices(indices, indexCount);
	this->initBuffers();
}
