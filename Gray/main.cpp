#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <GLFW\glfw3.h>
#include <math.h>
#include "GameScene.h"
#include "GameManager.h"
#include "BoxGeometry.h"
#include "Plane.h"
#include "Material.h"
#include "grMesh.h"
#include "OrbitCamera.h"
#include "grLight.h"

#include "containers.h"

using namespace std;

int main()
{
	GameManager* gm = new GameManager(1024, 800);
	GameScene* scene = new GameScene(gm);
	gm->changeScene(scene);
	GrLight* ambient = createAmbientLight(glm::vec3(0.8, 0.6, 0.1), 0.2);
	GrLight* point = createPointLight(glm::vec3(0.8, 0.7,0.9), 0.8, glm::vec3(2, 10, 20), 1.0f, 0.014f * 0.001, 0.07f * 0.01);
	scene->add(ambient);
	scene->add(point);
	////Brick
	Object* box = new Object();
	BoxGeometry* bg = new BoxGeometry(3.0f, 7.0f, 3.0f);
	Material* mat = new Material(glm::vec3(0.5f, 0.5f, 0.5f), 1, true,1);
	mat->setTexture("assets/brick.png", DIFFUSE_TEXTURE);
	GrMesh* mesh = new GrMesh(bg, mat);
	box->name = string("Brick");
	box->gm = gm;
	mesh->material->gm = gm;
	box->setPosition(-25, 0, 0);
	mesh->addToObject(box);
	box->setShader(gm->getShader(SHADER_TYPE::DEFAULT_SHADER));

	//Plane
	Object* plane = new Object();
	Plane* pg = new Plane(10.0f, 10.0f);
	Material* mat_plane = new Material(glm::vec3(0.5f, 0.5f, 0.5f), 1, true, 1);
	mat_plane->setTexture("assets/wood.png", DIFFUSE_TEXTURE);
	mat_plane->shininess = 32.0f;
	mat_plane->rougness = 20;
	mat_plane->metalness = true;
	GrMesh* plane_mesh = new GrMesh(pg, mat_plane);
	plane->setRotation(glm::vec3(-90,0,0));
	plane->setPosition(glm::vec3(0, 0, 20));
	plane->setScale(5,5,5);
	plane->setShader(gm->getShader(SHADER_TYPE::PBR_SHADER));
	plane->addComponent(plane_mesh);
	
	Model* model = new Model();
	Object* knight = model->loadModel(gm, "assets/knight_d_pelegrini.fbx");

	scene->add(box);
	scene->add(plane);
	scene->add(knight);

	
	OrbitCamera* oc = new OrbitCamera(&scene->camera);
	scene->camera.controls = oc;
	
	scene->camera.setTarget(glm::vec3(0, 0, -10));

	while (!glfwWindowShouldClose(gm->window))
	{
		point->setPositionX(20*cos(glfwGetTime() * 5));
		gm->update();
	}
}