#pragma once
#include "Geometry.h"

class Plane : public Geometry
{
public:
	Plane(float width, float height, float uvScale=1.0f);
	float width;
	float height;
};

