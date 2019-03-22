#include "Geometry.h"



Geometry::Geometry()
{
}

Geometry::Geometry(vector<Vertex> vertices, vector<unsigned int> indices)
{
	this->indexed = true;
	this->vertices = vertices;
	this->indices = indices;
	this->initBuffers();
}

Geometry::Geometry(vector<Vertex> vertices, unsigned int indices[])
{
	int indicesSize = sizeof(indices) / sizeof(indices[0]);
	this->setIndices(indices, indicesSize);
	this->indexed = true;
	this->vertices = vertices;
	this->initBuffers();
}

Geometry::Geometry(vector<Vertex> vertices)
{
	this->indexed = false;
	this->vertices = vertices;
	this->initBuffers();
}

Geometry::~Geometry()
{
}

void Geometry::setIndices(unsigned int _indices[], int indicesSize)
{
	this->indexed = true;
	cout << sizeof(_indices) << endl;
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
	glVertexAttribIPointer(4, NUM_BONES_PER_VERTEX, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, IDs));
	//Weights
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, NUM_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Weights));
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

void Geometry::draw()
{
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

void Geometry::cleanup()
{
}
