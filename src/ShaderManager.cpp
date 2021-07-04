#include "ShaderManager.h"
#include "GameManager.h"

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

	//This uniform buffers must be bind to this shader
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
	case SHADER_TYPE::OUTLINE_FRAGMENT:
		shader = new Shader("Shaders/simple_vertex.txt", "Shaders/outline_fragment.txt");
		break;
	case SHADER_TYPE::FBO_SHADER:
		shader = new Shader("Shaders/fbo_vertex.txt", "Shaders/fbo_fragment.txt");
		break;
	case SHADER_TYPE::SKYBOX_SHADER:
		shader = new Shader("Shaders/skybox_vertex.txt", "Shaders/skybox_fragment.txt");
		break;
	case SHADER_TYPE::DEPTH_SHADER:
		shader = new Shader("Shaders/depth/depth_vertex.txt","Shaders/depth/depth_fragment.txt");
		break;
	case SHADER_TYPE::DEPTH_POINT_SHADER:
		shader = new Shader("Shaders/depth/depth_point_vertex.txt", "Shaders/depth/depth_point_geometry.txt",
		 "Shaders/depth/depth_point_fragment.txt");
		break;
	case SHADER_TYPE::GBUFFER_SHADER:
		shader = new Shader("Shaders/gbuffer/gbuffer_vertex.txt", "Shaders/gbuffer/gbuffer_fragment.txt");
		break;
	case SHADER_TYPE::GBUFFER_SKY_SHADER:
		shader = new Shader("Shaders/gbuffer/gbuffer_skybox_vertex.txt", "Shaders/gbuffer/gbuffer_skybox_fragment.txt");
		break;
	case SHADER_TYPE::DEFERRED_SHADER:
		shader = new Shader("Shaders/fbo_vertex.txt", "Shaders/deferred_fragment.txt");
		break;
	default:
		break;
	}

	if (shader != nullptr)
	{
		//Whenever a new shader created, set up the binding points. UniformBuffers must be initialized before ShaderManager
		this->gm->matricesBuffer->setBindingPoint(shader);
		this->gm->lightsBuffer->setBindingPoint(shader);
	}
	return shader;
}
