#include "Components/Renderable.h"

gray::Renderable::Renderable()
{
    this->gm = nullptr;
}
gray::Renderable::Renderable(GameManager* gm, Object* object, Shader* shader)
{
    this->gm = gm;
    this->object = object;
    this->shader = shader;
}
void gray::Renderable::start()
{

}
void gray::Renderable::update(float deltaTime)
{
    this->object->update(deltaTime);
}
void gray::Renderable::draw()
{
    this->object->draw();
}
void gray::Renderable::draw(Shader* shader)
{
    this->object->draw(shader);
}
void gray::Renderable::cleanup()
{
    this->object->cleanup();
}
Component* gray::Renderable::clone()
{
    Renderable* rb = new Renderable(this->gm, this->object->clone(), this->shader);
    return rb;
}
