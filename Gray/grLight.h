#pragma once
#include <glm\glm.hpp>
#include <glad\glad.h>
#include "Object.h"
#include "Shader.h"

class grLight : public Object
{
public:
	grLight();
	grLight(glm::vec3 color, float intensity);
	~grLight();
	glm::vec3 color;
	float intensity;
	virtual void useLight(Shader* shader);
	
};

