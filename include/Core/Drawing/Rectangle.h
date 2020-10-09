#pragma once
#include <Core/Drawing/Point.h>
namespace drawing {
	struct Rectangle
	{
	public:
		Rectangle() {}
		Rectangle(float x, float y, float width, float height) {
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}
		float x = -1, y = -1, width = -1, height = -1;
		float Right() { return x + width; }
		float Bottom() { return y + height; }
		bool Contains(Point p) {
			return p.x >= x &&
				p.x <= Right() &&
				p.y >= y &&
				p.y <= Bottom();
		}
		bool Contains(PointD p) {
			return p.x >= x &&
				p.x <= Right() &&
				p.y >= y &&
				p.y <= Bottom();
		}
		
	};
}