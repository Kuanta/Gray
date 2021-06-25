#pragma once
#ifndef TEST
#define TEST
#define GLFW_DLL
#include "Model.h"
#include <iostream>
#include <math.h>
#include "Scene.h"
#include "GameManager.h"
#include "BoxGeometry.h"
#include "Plane.h"
#include "Material.h"
#include "grMesh.h"
#include "OrbitCamera.h"
#include "Lights/grLight.h"
#include "Components/Skybox.h"
#include "Object.h"
#include "GrAnimManager.h"

using namespace std;

// define callbacks
void keyInput(int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow * window, double xpos, double ypos);
void cursor_enter_callback(GLFWwindow * window, int entered);
void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset);

static Scene* scene;
static Object* player;
static GrAnimManager* animMan;
static GrAnimation* walk;
static GrAnimation* stand;
void run();





#endif