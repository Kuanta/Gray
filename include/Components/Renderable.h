#include "Components/Component.h"
#include "Object.h"
#include "Shader.h"

namespace gray
{
    class Renderable:public Component
    {
        Renderable();
        Renderable(GameManager* gm, Object* object, Shader* shader);

        GameManager* gm;
        Object* object;
        Shader* shader;

        void start() override;
        void update(float deltaTime) override;
        void draw() override;
        void draw(Shader* shader) override;
        void cleanup() override;
        Component* clone() override;

    };
}