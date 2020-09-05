#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	//this->initShader(SHADER_TYPE::DEFAULT_SHADER);
}

Shader* ShaderManager::getShaderByType(SHADER_TYPE shaderType)
{
	Shader* shader = nullptr;
	if (this->shaders.find(shaderType) == this->shaders.end())
	{
		shader = this->createShader(shaderType);
		this->shaders.insert(std::pair<SHADER_TYPE, Shader*>(shaderType, shader));
	}
	else {
		shader = this->shaders.at(shaderType);
		if (shader == nullptr)
		{
			shader = this->createShader(shaderType);
		}
	}
	return shader;
}

void ShaderManager::initShader(SHADER_TYPE shaderType)
{
	if (this->shaders.find(shaderType) == this->shaders.end())
	{
		//No such key
		Shader* shader = this->createShader(shaderType);
		this->shaders.insert(std::pair<SHADER_TYPE, Shader*>(shaderType, shader));
	}
}

void ShaderManager::deleteShader(SHADER_TYPE shaderType)
{
	Shader* shader = nullptr;
	if (this->shaders.find(shaderType) != this->shaders.end())
	{
		//No such key
		shader = this->shaders.at(shaderType);
		if (shader != nullptr)
		{
			shader->clean();
			this->shaders.at(shaderType) = nullptr;
		}
	}
}

Shader* ShaderManager::createShader(SHADER_TYPE shaderType)
{
	Shader* shader = nullptr;
	switch (shaderType)
	{
	case SHADER_TYPE::DEFAULT_SHADER:
		shader = new Shader("Shaders/simple_vertex.txt", "Shaders/simple_fragment.txt");
		break;
	case SHADER_TYPE::PBR_SHADER:
		shader = new Shader("Shaders/simple_vertex.txt", "Shaders/pbr_fragment.txt");
		break;
	default:
		break;
	}
	return shader;
}