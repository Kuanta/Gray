#ifndef MESH_H
#define MESH_H

#include <iostream>
#include "Components/Component.h"
#include "Geometry.h"
#include "Material.h"
#include "Scene.h"


class GrMesh : public Component
{
public:
	GrMesh(Geometry* geometry,Material* material);
	GrMesh(Geometry* geometry, Material* material, string name);
	~GrMesh();
	string name;
	Geometry* geometry;
	Material* material;
	void start() override;
	void update(float deltaTime) override;
	void draw(Shader* shader) override;
	void cleanup() override;
	virtual Component* clone() override;

private :
	Shader* shader;
	unsigned int VAO, VBO, EBO; //These buffers will be created in Geometry
};

#endif
