#ifndef SKYBOX_H
#define SKYBOX_H

#include "Component.h"
#include "BoxGeometry.h"
#include "Shader.h"
#include "GameManager.h"
#include "stb_image.h"


using namespace std;

class Skybox : public Component
{
    public:
    Skybox(GameManager* gm, vector<string> texturePaths);
    void start() override;
    void update(float deltaTime) override;
    void draw() override;
    void draw(Shader* shader) override;
    void cleanup() override;
    Component* clone() override;
    GameManager* gm;

    private:
    vector<string> texturePaths;
    unsigned int textureID;
    BoxGeometry* bg = nullptr;
    Shader* shader; //Skybox uses spesifically skybox shader

    void loadSkybox();

};

#endif