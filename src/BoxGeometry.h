#ifndef BOX_GEOMETRY
#define BOX_GEOMETRY

#include "Geometry.h"
#include <iostream>

using namespace std;

class BoxGeometry : public Geometry
{
public:
	BoxGeometry(float width, float height, float depth);
	~BoxGeometry();
};

#endif