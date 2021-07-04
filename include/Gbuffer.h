#include "glad/glad.h"
#include "Shader.h"
#include <iostream>
using namespace std;
class Gbuffer
{
    public: 
    Gbuffer(int width, int height);
    void setActive();
    unsigned int gBuffer;  //Framebuffer
    unsigned int gPosition; //Pos texture
    unsigned int gNormals; //Normals texture
    unsigned int gColors; // Colors (diffuse)
    unsigned int gPbr; //Roughness - Metalness
    unsigned int gUVs;
    unsigned int depthBuffer;
};