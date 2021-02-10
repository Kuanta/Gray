#define GLFW_DLL
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <math.h>
#include "GameScene.h"
#include "GameManager.h"
#include "BoxGeometry.h"
#include "Plane.h"
#include "Material.h"
#include "grMesh.h"
#include "OrbitCamera.h"
#include "Lights/grLight.h"
#include "Components/Skybox.h"
#include "containers.h"

using namespace std;

int main()
{
	cout<<"Starting THe Engine"<<endl;
	GameManager* gm = new GameManager(1920.0f, 1080.0f);
	GameScene* scene = new GameScene(gm);
	gm->changeScene(scene);
	GrLight* ambient = createAmbientLight(glm::vec3(0.8, 0.6, 0.1), 0.1);
	GrLight* point = createPointLight(glm::vec3(1.0, 1.0,0.9), 0.9, glm::vec3(2, 10, 30), 1.0f, 0.014f * 0.001, 0.07f * 0.01);
	GrLight* directional = createDirectionalLight(glm::vec3(0.5, 0.5,0.5), 0.1f, glm::vec3(0.5f,-1.0f,1.0f));
	directional->castShadow = 1;
	directional->setPosition(-10.0f, 50.0f,-20.0f);
	scene->add(ambient);
	scene->add(point);
	scene->add(directional);

	//Brick
	Object* box = new Object();
	BoxGeometry* bg = new BoxGeometry(3.0f, 7.0f, 3.0f);
	Material* mat = new Material(glm::vec3(0.5f, 0.5f, 0.5f), 1, true,1);
	mat->rougness = 5;
	mat->metalness = true;
	mat->setTexture("assets/brick.png", DIFFUSE_TEXTURE);
	GrMesh* mesh = new GrMesh(bg, mat);
	box->name = string("Brick");
	box->gm = gm;
	mesh->material->gm = gm;
	box->setPosition(-5, 0, 20);
	mesh->addToObject(box);
	box->setShader(gm->getShader(SHADER_TYPE::PBR_SHADER));
	//Plane
	Object* plane = new Object();
	Plane* pg = new Plane(10.0f, 10.0f, 3.5f);
	Material* mat_plane = new Material(glm::vec3(0.5f, 0.5f, 0.5f), 1, true, 1);
	mat_plane->setTexture("./assets/wood.png", DIFFUSE_TEXTURE);
	mat_plane->shininess = 32.0f;
	mat_plane->rougness = 20;
	mat_plane->metalness = true;
	GrMesh* plane_mesh = new GrMesh(pg, mat_plane);
	plane->setRotation(glm::vec3(-90,0,0));
	plane->setPosition(glm::vec3(0, 0, 20));
	plane->setScale(5,5,5);
	plane->setShader(gm->getShader(SHADER_TYPE::PBR_SHADER));
	plane->addComponent(plane_mesh);

	//Skybox
	Object* skybox = new Object();
	//back, bottom, front, left, right
	vector<string> filepaths = {"./assets/Skyboxes/skybox/right.jpg", 
	"./assets/Skyboxes/skybox/left.jpg",
	"./assets/Skyboxes/skybox/top.jpg",
	"./assets/Skyboxes/skybox/bottom.jpg",
	"./assets/Skyboxes/skybox/front.jpg",
	"./assets/Skyboxes/skybox/back.jpg"};
	Skybox* skyboxComp = new Skybox(gm,filepaths);
	skybox->addComponent(skyboxComp);

	scene->add(box);
	scene->add(plane);
	scene->add(skybox);
	
	OrbitCamera* oc = new OrbitCamera(&scene->camera);
	scene->camera.controls = oc;
	scene->camera.setTarget(glm::vec3(0, 0, -10));
	while (!glfwWindowShouldClose(gm->window))
	{
		point->setPositionX(20*cos(glfwGetTime() * 0.4));
		gm->update();
	}
}