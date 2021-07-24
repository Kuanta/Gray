#include "GrAnimGroup.h"

GrAnimationGroup::GrAnimationGroup()
{
    this->name = "";
}
GrAnimationGroup::GrAnimationGroup(string name)
{
    this->name = name;
}
void GrAnimationGroup::resetAnimation()
{
    for(int i=0;i<this->animations.size();i++)
    {
        this->animations.at(i)->resetAnimation();
    }
}
void GrAnimationGroup::addAnimation(GrAnimation *animation, float weight)
{
    this->animations.push_back(animation);
    animation->weight = weight;
}
void GrAnimationGroup::update(float deltaTime, GrSkeleton *skeleton, bool transition, float transitionFactor = 0.0f)
{
    for(int i=0;i<this->animations.size();i++)
    {
        this->animations.at(i)->update(deltaTime, skeleton, transition, transitionFactor);
    }
}