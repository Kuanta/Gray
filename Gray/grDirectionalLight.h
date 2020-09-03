#pragma once
#include "Shader.h"
#include "grLight.h"
class grDirectionalLight : public GrLight
{
public:
	grDirectionalLight();
	grDirectionalLight(glm::vec3 color, float intensity, glm::vec3 direction);
	~grDirectionalLight();

	//Params
	glm::vec3 direction;

	//methods
	virtual void useLight(Shader* shader) override;
private:
	
};

