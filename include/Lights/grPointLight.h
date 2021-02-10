#pragma once
#include "Lights/grLight.h"
#include "Shader.h"

class grPointLight : public GrLight
{
public:
	grPointLight();
	grPointLight(glm::vec3 color, float intensity, float constant, float linear, float quadratic);
	~grPointLight();
	virtual void useLight(Shader* shader) override;
	virtual glm::mat4 getTransformMat() override;

	//Params
	float constant;
	float linear;
	float quadratic;
};

