#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef float real;

namespace Gr
{
	class Vec3
	{
	public:
		real getX()
		{
			return this->x;
		}
		real getY()
		{
			return this->y;
		}
		real getZ()
		{
			return this->z;
		}
		void setX(real x)
		{
			this->x = x;
		}
		void setY(real y)
		{
			this->y = y;
		}
		void setZ(real z)
		{
			this->z = z;
		}
	private:
		real x;
		real y;
		real z;
	};
}