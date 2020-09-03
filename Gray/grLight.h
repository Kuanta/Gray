#pragma once
#include <glm\glm.hpp>
#include <glad\glad.h>
#include "Object.h"
#include "Shader.h"

class GrLight : public Object
{
public:
	GrLight();
	GrLight(glm::vec3 color, float intensity);
	~GrLight();
	glm::vec3 color;
	float intensity;
	virtual void useLight(Shader* shader);
	
};

