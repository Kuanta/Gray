#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(unsigned int byteSize, const char* blockName, int bindingPoint)
{
	this->byteSize = byteSize;
	this->blockName = blockName;
	this->bindingPoint = bindingPoint;
	glGenBuffers(1, &this->bufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, this->bufferID);
	glBufferData(GL_UNIFORM_BUFFER, byteSize, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::setBindingPoint(Shader* shader)
{
	//For pre OpenGL 4.2
	glBindBuffer(GL_UNIFORM_BUFFER, this->bufferID);
	int index = glGetUniformBlockIndex(shader->ID, this->blockName);
	glUniformBlockBinding(shader->ID, index, bindingPoint);
	glBindBufferBase(GL_UNIFORM_BUFFER, this->bindingPoint, this->bufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::bindBuffer()
{
	glBindBuffer(GL_UNIFORM_BUFFER, this->bufferID);
}

void UniformBuffer::updateInt(int data, unsigned int offset)
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &data);
}

void UniformBuffer::updateFloat(float data, unsigned int offset)
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &data);
}

void UniformBuffer::updateVec2(glm::vec2 data, unsigned int offset)
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec2), glm::value_ptr(data));
}

void UniformBuffer::updateVec3(glm::vec3 data, unsigned int offset)
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(data));
}

void UniformBuffer::updateVec4(glm::vec4 data, unsigned int offset)
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec4), glm::value_ptr(data));
}

void UniformBuffer::updateMat2(glm::mat2 data, unsigned int offset)
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat2), glm::value_ptr(data));
}

void UniformBuffer::updateMat3(glm::mat3 data, unsigned int offset)
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat3), glm::value_ptr(data));
}

void UniformBuffer::updateMat4(glm::mat4 data, unsigned int offset)
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(data));
}

