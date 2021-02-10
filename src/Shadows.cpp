#include "Shadows.h"

gray::Shadows::Shadows(int width, int height, Scene* scene, GrLight* light)
{
    this->width = width;
    this->height = height;
    this->scene;
    this->light = light;
}

void gray::Shadows::initShadows()
{
    //İnit framebuffer
    glGenFramebuffers(1, &frameBuffer);

    //Init texture
    glGenTextures(1, &this->depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    //Create only depth component
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 

    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthMap, 0);
    glDrawBuffer(GL_NONE);  //No color data is needed
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void gray::Shadows::draw()
{
    glViewport(0,0,this->width, this->height);
    glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);
    glClear(GL_DEPTH_BUFFER_BIT);

    //Shaders and Matrices
    this->scene->matricesBuffer->bindBuffer();
    glm::mat4 lightViewMatrix = glm::lookAt(this->light->getPosition(), this->light->direction,glm::vec3(0.0f,1.0f,0.0f));
    glm::mat4 lightProjMatrix = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,this->nearPlane, this->farPlane);
    //Render Scene
    this->scene->draw(0.0f);
}