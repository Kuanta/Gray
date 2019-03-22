#pragma once
#include "Geometry.h"
#include <iostream>

using namespace std;

class BoxGeometry : public Geometry
{
public:
	BoxGeometry(float width, float height, float depth);
	~BoxGeometry();
};

