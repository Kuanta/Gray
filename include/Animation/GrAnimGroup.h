#include "Animation/GrAnimation.h"

class GrAnimationGroup : public GrAnimation
{
    public:
    GrAnimationGroup();
    GrAnimationGroup(string name);
    ~GrAnimationGroup();
    void resetAnimation() override;
    vector<GrAnimation*> animations;
    void addAnimation(GrAnimation* animation, float weight);
    void update(float deltaTime, GrSkeleton *skeleton, bool transition, float transitionFactor) override;
    void removeAnimation(string animName);

};