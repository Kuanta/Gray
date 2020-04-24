#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <GLFW\glfw3.h>
#include <math.h>
#include "GameScene.h"
#include "GameManager.h"
#include "BoxGeometry.h"
#include "Material.h"
#include "grMesh.h"
#include "OrbitCamera.h"
#include "grLight.h"
#include "Model.h"

using namespace std;

int main()
{
	GameManager* gm = new GameManager(1024, 800);
	GameScene* scene = new GameScene(gm);
	gm->currentScene = scene;

	////Brick
	//Object* box = new Object();
	//BoxGeometry* bg = new BoxGeometry(3.0f, 7.0f, 3.0f);
	//Material* mat = new Material(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 16);
	//mat->setTexture("assets/brick.png", DIFFUSE_TEXTURE);
	//GrMesh* mesh = new GrMesh(bg, mat);
	//box->name = string("Brick");
	//box->gm = gm;
	//mesh->material->gm = gm;
	//box->position.x = -25;
	//mesh->addToObject(box);

	////Wooden Box
	//Object* woodBox = new Object();
	//BoxGeometry* woodBoxGeo = new BoxGeometry(5, 5, 5);
	//Material* woodBoxMat = new Material(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 4);
	//woodBoxMat->setTexture("assets/wood.png", DIFFUSE_TEXTURE);
	//GrMesh* woodBoxMesh = new GrMesh(woodBoxGeo, woodBoxMat);
	//woodBox->name = "Wood";
	//woodBox->gm = gm;
	//woodBoxMesh->material->gm = gm;
	//woodBox->position.x = 5;
	//woodBoxMesh->addToObject(woodBox);

	////Sci-fi
	//BoxGeometry* sfBoxGeo = new BoxGeometry(5, 5, 5);
	//Material* sfBoxMat = new Material(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 4);
	//sfBoxMat->setTexture("assets/metal.png", DIFFUSE_TEXTURE);
	//GrMesh* sfBoxMesh = new GrMesh(sfBoxGeo, sfBoxMat);
	//Object* sfBox = new Object();
	//sfBoxMesh->addToObject(sfBox);
	//sfBox->name = "SciFi";
	//sfBox->gm = gm;
	//sfBox->position.x =-5;
	//sfBox->position.z = 5;
	
	
	Model* model = new Model();
	//Object* scene2 = model->loadModel(gm, "assets/knight_d_pelegrini.fbx");
	Object* scene2 = model->loadModel(gm, "assets/paladin_j_nordstrom.fbx");
	//Object* scene2 = model->loadModel(gm, "assets/walking.fbx");
	//Object* scene2 = model->loadModel(gm, "assets/Sylvanas/Sylvanas.fbx");
	
	if (scene2 != nullptr)
	{
		scene2->scale = glm::vec3(0.5,0.5,0.5);
		scene2->position.y = -60;
		scene2->position.z = -30;
		scene->em.addElement(scene2);
		scene2->gm = gm;
	}
	//scene->em.addElement(box);
	//scene->em.addElement(woodBox);
	//scene->em.addElement(sfBox);
	
	
	OrbitCamera* oc = new OrbitCamera(&scene->camera);
	scene->camera.controls = oc;
	/*Object* parentMesh = new Object();
	Object* parentMesh2 = new Object();
	parentMesh->add(parentMesh2);
	parentMesh2->add(mesh);
	
	parentMesh->position.z = -15;
	parentMesh->position.x = 0;
	parentMesh->position.y = 0;
	parentMesh->scale.x = 2;*/
	
	scene->camera.setTarget(glm::vec3(0, 0, -10));

	while (!glfwWindowShouldClose(gm->window))
	{
		//box->position.x = cos(glfwGetTime()) * 5;
		gm->update();
	}
}