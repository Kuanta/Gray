#include "Controls.h"



Controls::Controls()
{
}


Controls::~Controls()
{
}

void Controls::keyInput(int key, int scancode, int action, int mods)
{
}

void Controls::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
}

void Controls::cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (this->firstMove)
	{
		
		this->deltaX = 0;
		this->deltaY = 0;
	}
	else
	{
		this->deltaX = xpos - this->lastX;
		this->deltaY = ypos - this->lastY;
	}
	this->lastX = xpos;
	this->lastY = ypos;
}

void Controls::cursor_enter_callback(GLFWwindow * window, int entered)
{
}

void Controls::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
}
