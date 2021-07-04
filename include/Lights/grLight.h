#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Object.h"
#include "Shader.h"

enum class LIGHT_TYPES {
	AMBIENT = 0, DIRECTIONAL = 1, POINT = 2
};
class GrLight : public Object
{
public:
	GrLight();
	GrLight(LIGHT_TYPES type);
	GrLight(LIGHT_TYPES type, glm::vec3 color, float intensity);
	GrLight(LIGHT_TYPES type, glm::vec3 color, float intensity, float constant, float linear, float quadratic);
	~GrLight();
	virtual void useLight(Shader* shader);
	virtual vector<glm::mat4> getTransformMat(int width, int height);  //For shadow mapping
	virtual void createDepthMap(unsigned int frameBuffer);

	//Properties
	LIGHT_TYPES type;
	glm::vec3 color;
	glm::vec3 direction;
	float intensity;
	float constant;
	float linear;
	float quadratic;
	bool castShadow=false;
	unsigned int depthMap;
	float farPlane = 1.0f; //For shadow maps
	bool staticLight = false; //Static lights won't update their shadowmaps no matter what
	bool requiresUpdate = true; //If this light moves, update its static depth maps
	float attenuationFactor = 2.0f;
};

//Generator methods for various light types
GrLight* createAmbientLight(glm::vec3 color, float intensity);
GrLight* createDirectionalLight(glm::vec3 color, float intensity, glm::vec3 direction);
GrLight* createPointLight(glm::vec3 color, float intensity, glm::vec3 position, float cosntant, float linear, float quadratic);
