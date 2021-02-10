#pragma once
#include "Shader.h"
#include "Lights/grLight.h"
#include <glm/gtx/string_cast.hpp>
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
	virtual glm::mat4 getTransformMat() override;
	
private:
	
};

