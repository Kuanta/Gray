#pragma once
#include "grLight.h"
#include "Shader.h"

class grPointLight : public grLight
{
public:
	grPointLight();
	grPointLight(glm::vec3 color, float intensity, float constant, float linear, float quadratic);
	~grPointLight();
	virtual void useLight(Shader* shader) override;

	//Params
	float constant;
	float linear;
	float quadratic;
};

