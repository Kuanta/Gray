#include "Components/Skybox.h"

Skybox::Skybox(GameManager* gm, vector<string> texturePaths)
{
    this->gm = gm;
    this->texturePaths = texturePaths;
    this->loadSkybox();
}

void Skybox::start()
{

}
void Skybox::update(float deltaTime)
{

}
void Skybox::draw()
{

    this->shader->use();
    glDepthMask(GL_FALSE);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);
    this->bg->draw(this->shader);
    glDepthMask(GL_TRUE);
}
void Skybox::draw(Shader* shader)
{
    //If a special shader was fed, don't draw skybox
    if(shader == nullptr)
    {
        this->draw();
    }
}
void Skybox::loadSkybox()
{
    this->shader = this->gm->getShader(SHADER_TYPE::GBUFFER_SKY_SHADER);
    //Generate texture
    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);

    int width, height, nrChannels;
    
    for(unsigned int i = 0; i < this->texturePaths.size(); i++)
    {
        cout<<"Loading: "<<texturePaths.at(i)<<endl;
        unsigned char *data = stbi_load(texturePaths.at(i).c_str(), &width, &height, &nrChannels, 0);
        if (!data)
        {
            printf("Failed to find :%s\n", texturePaths.at(i));
            return;
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    //Inity box geometry
    this->bg = new BoxGeometry(500.0f, 500.0f, 500.0f);  
}
void Skybox::cleanup()
{

}
Component* Skybox::clone()
{
    return this;
}