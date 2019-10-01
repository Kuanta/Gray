#ifndef MESH_H
#define MESH_H

#include <iostream>
#include "Component.h"
#include "Geometry.h"
#include "Material.h"
#include "Scene.h"

class Mesh : public Component
{
public:
	Mesh(Geometry* geometry,Material* material);
	~Mesh();
	Geometry* geometry;
	Material* material;
	void start() override;
	void update(float deltaTime) override;
	void draw() override;
	void cleanup() override;

private :
	Shader* shader;
	unsigned int VAO, VBO, EBO; //These buffers will be created in Geometry
	
};

#endif
