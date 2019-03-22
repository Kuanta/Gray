#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <GLFW\glfw3.h>
#include "GameScene.h"
#include "GameManager.h"
#include "BoxGeometry.h"
#include "Material.h"
#include "Mesh.h"
#include "OrbitCamera.h"
#include "grLight.h"
#include "Model.h"

using namespace std;

int main()
{
	GameManager* gm = new GameManager(800, 600);
	GameScene* scene = new GameScene(gm);
	gm->currentScene = scene;
	BoxGeometry* bg = new BoxGeometry(5.0f, 5.0f, 5.0f);
	Material* mat = new Material(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 4);
	mat->setDiffuseTexture("assets/brick.png");
	Mesh* mesh = new Mesh(bg, mat);
	mesh->gm = gm;
	mesh->material->gm = gm;
	Model* model = new Model();
	Object* scene2 = model->loadModel(gm, "assets/pyramid.fbx");
	scene2->scale = glm::vec3(1,1,1);
	OrbitCamera* oc = new OrbitCamera(&scene->camera);
	Object* parentMesh = new Object();
	Object* parentMesh2 = new Object();
	parentMesh->add(parentMesh2);
	parentMesh2->add(mesh);
	scene->camera.controls = oc;
	parentMesh->position.z = -15;
	parentMesh->position.x = 0;
	parentMesh->position.y = 0;
	parentMesh->scale.x = 2;
	scene->camera.setTarget(glm::vec3(0, 0, -10));
	scene->em.addElement(parentMesh);

	while (!glfwWindowShouldClose(gm->window))
	{
		gm->update();
	}
}