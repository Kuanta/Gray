#include "OrbitCamera.h"



OrbitCamera::OrbitCamera(Camera* camera)
{
	this->camera = camera;
	this->yaw = 90.0f;
}


OrbitCamera::~OrbitCamera()
{
}

void OrbitCamera::update(float deltaTime)
{
	//Reseting timers
	if (this->leftMousePressed) {
		this->orbitTimer += deltaTime;
		if (this->orbitTimer>this->maxDragTime) {
			this->orbitTimer = 0;
			this->yawAcc = 0;
			this->pitchAcc = 0;
		}
	}
	else {
		//Set Accelerations to 0 just in case
		this->yawAcc = 0;
		this->pitchAcc = 0;
	}

	if (this->rightMousePressed) {
		this->panTimer += deltaTime;
		if (this->panTimer>this->maxDragTime) {
			this->panTimer = 0;
			this->panFrontAcc = 0;
			this->panRightAcc = 0;
		}
	}
	else {
		//Set Accelerations to 0 just in case
		this->panFrontAcc = 0;
		this->panRightAcc = 0;
	}
	//Update angles
	this->yawVel += this->yawAcc * deltaTime;
	this->pitchVel += this->pitchAcc*deltaTime;
	this->yaw += this->yawVel*deltaTime;
	this->pitch += this->pitchVel*deltaTime;
	if (this->pitch>89) this->pitch = 89;
	if (this->pitch<-89) this->pitch = -89;

	//Update Radius
	this->radiusVel += this->radiusAcc*deltaTime;
	this->radius += this->radiusVel*deltaTime + this->radiusAcc*deltaTime*deltaTime*0.5;
	if (this->radius>this->maxRadius) { this->radius = this->maxRadius; this->radiusVel = 0; this->radiusAcc = 0; }
	if (this->radius<this->minRadius) { this->radius = this->minRadius; this->radiusVel = 0; this->radiusAcc = 0; }


	
	this->panRightVel += this->panRightAcc*deltaTime;
	this->panFrontVel += this->panFrontAcc*deltaTime;
	float deltaRight = this->panRightVel*deltaTime;
	float deltaFront = this->panFrontVel*deltaTime;
	this->camera->target = this->camera->target + (this->camera->getRightVector() * deltaRight);
	this->camera->target = this->camera->target + (this->camera->getFrontVector() * deltaFront);
	
	this->radiusVel += this->radiusAcc*deltaTime;
	this->radius = this->radius + this->radiusVel * deltaTime + this->radiusAcc*deltaTime*deltaTime*0.5;
	//Orbit
	this->camera->position.x = (cos((PI / 180)*this->pitch) * cos((PI / 180)*this->yaw) * this->radius + this->camera->target.x);
	this->camera->position.y = (sin((PI / 180)*this->pitch)*this->radius + this->camera->target.y);
	this->camera->position.z = (sin((PI / 180)*this->yaw)*cos((PI / 180)*this->pitch) * this->radius + this->camera->target.z);

	this->panRightVel *= this->damping;
	this->panFrontVel *= this->damping;
	this->radiusVel *= this->damping;
	this->yawVel *= damping;
	this->pitchVel *= damping;

	//Set accelerations to zero
	this->radiusAcc = 0;
}

void OrbitCamera::keyInput(int key, int scancode, int action, int mods)
{
}

void OrbitCamera::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	//Pressed
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
	{
		this->leftMousePressed = true;
		this->leftPressedPos = glm::vec2(xpos, ypos);
	}
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		this->rightMousePressed = true;
		this->rightPressedPos = glm::vec2(xpos, ypos);
		
	}

	//Release
	if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT)
	{
		this->leftMousePressed = false;
	}
	if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		this->rightMousePressed = false;
	}

}

void OrbitCamera::cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
{
	Controls::cursor_position_callback(window, xpos, ypos);
	if (this->leftMousePressed)
	{
		double deltaX = xpos - this->leftPressedPos.x;
		double deltaY = ypos - this->leftPressedPos.y;
		this->yawAcc = deltaX * this->orbitSens;
		this->pitchAcc = deltaY * this->orbitSens;
	}
	if (this->rightMousePressed)
	{
		double deltaX = xpos - this->rightPressedPos.x;
		double deltaY = ypos - this->rightPressedPos.y;
		this->panRightAcc = deltaX * this->panSens;
		this->panFrontAcc = deltaY * this->panSens;
	}
}

void OrbitCamera::cursor_enter_callback(GLFWwindow * window, int entered)
{
}

void OrbitCamera::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	this->radiusAcc = -1 * yoffset * this->radiusSens;
}
