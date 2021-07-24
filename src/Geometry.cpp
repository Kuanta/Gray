#include "Geometry.h"
#include <glm/gtx/string_cast.hpp>



Geometry::Geometry()
{
}

Geometry::Geometry(vector<Vertex> vertices, vector<unsigned int> indices, bool initBuffers)
{
	this->indexed = true;
	this->indices = indices;
	this->initGeometry(vertices, initBuffers);
}

Geometry::Geometry(vector<Vertex> vertices, unsigned int indices[], bool initBuffers)
{
	int indicesSize = sizeof(indices) / sizeof(indices[0]);
	this->setIndices(indices, indicesSize);
	this->indexed = true;
	this->initGeometry(vertices, initBuffers);
}

Geometry::Geometry(vector<Vertex> vertices, bool initBuffers)
{
	this->indexed = false;
	this->initGeometry(vertices, initBuffers);
}

Geometry::~Geometry()
{
}

void Geometry::setIndices(unsigned int _indices[], int indicesSize)
{
	this->indexed = true;
	this->indices.clear();
	for (int i = 0; i < indicesSize; i++)
	{
		this->indices.push_back(_indices[i]);
	}
}

void Geometry::initBuffers()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	if (this->indices.size() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	}
	
	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UVs));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV2s));
	//Bone IDs
	glEnableVertexAttribArray(4);
	glVertexAttribIPointer(4, Geometry::NUM_BONES_PER_VERTEX, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, IDs));
	//Weights
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, Geometry::NUM_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Weights));
	// vertex tangent
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	// colors
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
	
	glBindVertexArray(0);
}

void Geometry::draw(Shader* shader)
{
	shader->use();
	if(this->hasBones)
	{
		shader->setInt("hasBones", 1);
	}else{
		shader->setInt("hasBones", 0);
	}
	glBindVertexArray(this->VAO);
	if (this->indexed)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	}
	glBindVertexArray(0);
}

Geometry* Geometry::clone()
{
	return this;
}

void Geometry::cleanup()
{
}

void Geometry::initGeometry(vector<Vertex> vertices, bool initBuffers)
{
	this->vertices = vertices;
	//Set weights and boneIDs to default values
	for (int i = 0; i < this->vertices.size(); i++)
	{
		for (int j = 0; j < Geometry::NUM_BONES_PER_VERTEX; j++)
		{
			this->vertices.at(i).IDs[j] = -1;
			this->vertices.at(i).Weights[j] = 0;
		}
	}
	if (initBuffers)
	{
		this->initBuffers();
	}

}
