#pragma once
#include "Geometry.h"

class Plane : public Geometry
{
public:
	Plane(float width, float height);
	float width;
	float height;
};

