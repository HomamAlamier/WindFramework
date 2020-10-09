#pragma once
#ifndef GRAPHICSELEMENT_H
#define GRAPHICSELEMENT_H

#include <vector>
#include <GL.h>
#include <Core/Drawing/Rectangle.h>
#include <Core/Drawing/Point.h>
#include <Core/Drawing/Image.h>
constexpr float MATH_PI = 3.14159265359f;
#define GL_IMAGE_MODE 0x0f01ab
enum class RectangleGradiantType {
	Vertical, Horizontal,
	Inverted_Vertical, Inverted_Horizontal
};
namespace drawing {
	struct gelement
	{
		bool enabled = true;
		int GLMODE = 0;
		GLsizei count = 0;
		std::vector<Point> points;
		size_t startIndex = 0;
		size_t endIndex = 0;
		Image* img;
	};
	class GraphicsElement
	{
		Rectangle rect;
		Color cColor;
		GLuint VAO = 0, VBO = 0;
		bool inited = false;
		bool edited = false;
		std::vector<gelement> gelements;
		std::vector<size_t> ontop;
	public:
		GraphicsElement(Rectangle rect);
		bool ModifyLine(size_t index, Point pnt1, Point p2, Color color);
		bool ModifyRectangle(size_t index, Rectangle rect, Color color, bool fill = false);		
		bool ModifyGradiantRectangle(size_t index, Rectangle rect, Color color1, Color color2, RectangleGradiantType rgType);	
		bool ModifyGradiantCenterRectangle(size_t index, Rectangle rect, Color center, Color o_color);
		size_t AddLine(Point pnt1, Point p2);
		size_t AddLine(Point pnt1, Point pnt2, Color color);
		size_t AddRectangle(Rectangle rect, Color color, bool fill = false);
		size_t AddImage(Rectangle rect, Image* image);
		size_t AddArc(Point center, float radius, float startAngle, float endAngle, Color color);
		size_t AddArc(Point center, float radius, float startAngle, float endAngle, Color color1, Color color2);
		size_t AddGradiantRectangle(Rectangle rect, Color color1, Color color2, RectangleGradiantType rgType);
		size_t AddGradiantCenterRectangle(Rectangle rect, Color center, Color o_color);
		bool Disable(size_t index);
		bool Disable(size_t index, size_t count);
		bool Enable(size_t index);
		bool Enable(size_t index, size_t count);
		void DrawOnTop(size_t index);
		void DisableAll();
		void EnableAll();
		void ClearColor(Color color);
		void Draw(int vpWidth, int vpHeight);
		void ChangeColorForAll(Color color);
		void Clear();
		void ClearData();
	};
}
#endif // !GRAPHICSELEMENT_H
