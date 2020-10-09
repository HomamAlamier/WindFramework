#ifndef POINT_H
#define POINT_H



#include <Core/Drawing/Color.h>
namespace drawing {
	struct Point {
		float x = 0, y = 0, z = 0;
		Point() {
			color = Color::Black();
		}
		Point(float x, float y, float z = 0, Color color = Color::Black()) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->color = Color(color);
		}
		Point operator-(Point& r){
			return Point(x - r.x, y - r.y);
		}
		Point operator+(Point& r){
			return Point(x + r.x, y + r.y);
		}
		void set_color(Color c) { color = Color(c); }
		Color color;
	};
	struct PointD {
		double x = 0, y = 0, z = 0;
		PointD() {
			color = Color::Black();
		}
		PointD(double x, double y, double z = 0, Color color = Color::Black()) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->color = color;
		}
		PointD operator-(PointD& r){
			return PointD(x - r.x, y - r.y);
		}
		PointD operator+(PointD& r){
			return PointD(x + r.x, y + r.y);
		}
		PointD offset(double x, double y){
			return PointD(this->x + x, this->y + y);
		}
		Color color;
	};
}
#endif // !POINT_H