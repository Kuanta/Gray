#include "Scene.h"
#include "GameManager.h"


Scene::Scene(GameManager * gm)
{
	this->camera = Camera();
	this->gm = gm;
	this->window = gm->window;
}

Scene::~Scene()
{

}