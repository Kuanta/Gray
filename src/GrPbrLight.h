#pragma once
#include "grLight.h"

class GrPbrLight : public GrLight
{
	GrPbrLight(glm::vec3 color, float intensity);
	virtual void useLight(Shader* shader) override;
};

