#pragma once
#include <map>
#include "Shader.h"

enum class SHADER_TYPE { DEFAULT_SHADER = 0, PBR_SHADER, OUTLINE_FRAGMENT , FBO_SHADER, SKYBOX_SHADER, DEPTH_SHADER};

class GameManager;
class ShaderManager
{
public:
	ShaderManager();
	GameManager* gm;
	std::map<SHADER_TYPE, Shader*> shaders;
	
	//Methods
	Shader* getShaderByType(SHADER_TYPE shaderType);
	void initShader(SHADER_TYPE shaderType);
	void deleteShader(SHADER_TYPE shaderType);

private:
	Shader* createShader(SHADER_TYPE shaderType);
};

