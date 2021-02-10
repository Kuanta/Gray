#include <iostream>
#include "glad/glad.h"
#include "Scene.h"
#include "Lights/grLight.h"

namespace gray
{
    class Shadows
    {
        public:
            Shadows(int width, int height, Scene* scene, GrLight* light);
            void initShadows();
            void draw();
            int width;
            int height;
            Scene* scene;
            GrLight* light;
        private:
            unsigned int depthMap;
            unsigned int frameBuffer;
            float nearPlane = 1.0f;
            float farPlane = 10.0f;
    };
}
