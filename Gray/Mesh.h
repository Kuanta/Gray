#ifndef MESH_H
#define MESH_H

#include <iostream>
#include "Object.h"
#include "Geometry.h"
#include "Material.h"
#include "Scene.h"

class Mesh : public Object
{
public:
	Mesh(Geometry* geometry,Material* material);
	~Mesh();
	Geometry* geometry;
	Material* material;
	void update(float deltaTime) override;
	void draw() override;
	void cleanup();

	//Transformation Matrices
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
private :
	unsigned int VAO, VBO, EBO; //These buffers will be created in Geometry
	
};

#endif
