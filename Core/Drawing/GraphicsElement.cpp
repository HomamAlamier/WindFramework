#include <Core/Drawing/GraphicsElement.h>
#include <Core/Drawing/Shaders.h>
#include <Core/Drawing/Point.h>
#include <Core/Drawing/Rectangle.h>
namespace drawing {
	GraphicsElement::GraphicsElement(Rectangle rect) {
		this->rect = Rectangle(rect);
	}
	void gelementToFloatVector(gelement* e, std::vector<float>* vec){
		for(auto point : e->points){
			vec->push_back(point.x);
			vec->push_back(point.y);
			vec->push_back(point.z);
			vec->push_back(point.color.R);
			vec->push_back(point.color.G);
			vec->push_back(point.color.B);
			vec->push_back(point.color.A);
			vec->push_back(0.0f);
		}
	}
	size_t GraphicsElement::AddLine(Point p1, Point p2) {
		gelement e;
		e.GLMODE = GL_LINES;
		e.points.push_back(Point(p1.x, p1.y, 0, p1.color));
		e.points.push_back(Point(p2.x, p2.y, 0, p2.color));
		e.count = 2;
		gelements.push_back(e);
		edited = true;
		return gelements.size() - 1;
	}
	bool GraphicsElement::Disable(size_t i){
		if (gelements.size() <= i) return false;
		else{
			gelements[i].enabled = false;
			return true;
		}
	}
	bool GraphicsElement::Enable(size_t i){
		if (gelements.size() <= i) return false;
		else{
			gelements[i].enabled = true;
			return true;
		}
	}
	void GraphicsElement::DisableAll(){
		for (size_t i = 0; i < gelements.size(); i++)
		{
			auto p = &gelements[i];
			p->enabled = false;
		}
	}
	void GraphicsElement::EnableAll(){
		for (size_t i = 0; i < gelements.size(); i++)
		{
			auto p = &gelements[i];
			p->enabled = true;
		}
	}
	bool GraphicsElement::ModifyLine(size_t index, Point p1, Point p2, Color color) {
		if (index >= gelements.size() ||
			gelements[index].GLMODE != GL_LINES) return false;
		gelement* e = &gelements[index];
		e->points.clear();
		e->points.push_back(Point(p1.x, p1.y, 0, color));
		e->points.push_back(Point(p2.x, p2.y, 0, color));
		e->count = 2;
		edited = true;
		return true;
	}
	bool GraphicsElement::ModifyRectangle(size_t index, Rectangle rect, Color color, bool fill) {
		if (index >= gelements.size() ||
			(gelements[index].GLMODE != GL_LINE_STRIP && gelements[index].GLMODE != GL_TRIANGLES) )
			return false;
		gelement* e = &gelements[index];
		e->points.clear();
		if (!fill)
		{
			e->GLMODE = GL_LINE_STRIP;
			e->points.push_back(Point(rect.x, rect.y, 0, color));
			e->points.push_back(Point(rect.Right(), rect.y, 0, color));
			e->points.push_back(Point(rect.Right(), rect.y, 0, color));
			e->points.push_back(Point(rect.Right(), rect.Bottom(), 0, color));
			e->points.push_back(Point(rect.Right(), rect.Bottom(), 0, color));
			e->points.push_back(Point(rect.x, rect.Bottom(), 0, color));
			e->points.push_back(Point(rect.x, rect.Bottom(), 0, color));
			e->points.push_back(Point(rect.x, rect.y, 0, color));
			e->count = 8;
			edited = true;
			return true;
		}
		else {
			e->GLMODE = GL_TRIANGLES;
			e->points.push_back(Point(rect.x, rect.y, 0, color));
			e->points.push_back(Point(rect.Right(), rect.y, 0, color));
			e->points.push_back(Point(rect.Right(), rect.Bottom(), 0, color));
			e->points.push_back(Point(rect.Right(), rect.Bottom(), 0, color));
			e->points.push_back(Point(rect.x, rect.Bottom(), 0, color));
			e->points.push_back(Point(rect.x, rect.y, 0, color));
			e->count = 6;

			if (inited) {
					
				std::vector<float> tmp;
				gelementToFloatVector(e, &tmp);

				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferSubData(GL_ARRAY_BUFFER, e->startIndex, sizeof(float) * tmp.size(), tmp.data());
				glBindVertexArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			else edited = true;
			return true;
		}
	}
	size_t GraphicsElement::AddLine(Point p1, Point p2, Color color) {
		gelement e;
		e.GLMODE = GL_LINES;
		e.points.push_back(Point(p1.x, p1.y, 0, color));
		e.points.push_back(Point(p2.x, p2.y, 0, color));
		e.count = 2;
		gelements.push_back(e);
		edited = true;
		return gelements.size() - 1;
	}
	size_t GraphicsElement::AddArc(Point center, float radius, float startAngle, float endAngle, Color color) {
		gelement e;
		e.GLMODE = GL_TRIANGLES;
		float sa = startAngle;
		Point old(-1, 0);
		while (sa < startAngle + endAngle)
		{
			float x = center.x + radius * sin(sa);
			float y = center.y + radius * cos(sa);
			if (old.x > -1) {
				e.points.push_back(old);
				e.points.push_back(Point(center.x, center.y, 0, color));
				e.points.push_back(Point(x, y, 0, color));
			}
			old = Point(x, y, 0, color);
			sa += 0.1;
		}
		e.count = (int)e.points.size();
		gelements.push_back(e);
		return gelements.size() - 1;
	}
	size_t GraphicsElement::AddArc(Point center, float radius, float startAngle, float endAngle, Color color1, Color color2) {
		gelement e;
		e.GLMODE = GL_LINES;
		float sa = startAngle;
		Point old(-1, 0);
		Color c = color1;
		while (sa < startAngle + endAngle)
		{
			float x = center.x + radius * sin(sa);
			float y = center.y + radius * cos(sa);
			if (old.x > -1) {
				e.points.push_back(old);
				e.points.push_back(Point(x, y));
			}
			old = Point(x, y);
			sa += 0.01f;
		}
		float cc = 1.0f / e.points.size();
		for (size_t i = 0; i < e.points.size(); i++)
		{
			e.points[i].set_color(c);
			c = Color::Translate(c, color2, cc);
		}
		e.count = (int)e.points.size();
		gelements.push_back(e);
		return gelements.size() - 1;
	}
	size_t GraphicsElement::AddImage(Rectangle rect, Image* image){
		gelement e;
		e.GLMODE = GL_IMAGE_MODE;
		e.img = image;
		e.points.push_back(Point(rect.x, rect.y, 0,       		 drawing::Color(0,   255, 0, 0)));
		e.points.push_back(Point(rect.Right(), rect.y, 0, 		 drawing::Color(255, 255, 0, 0)));
		e.points.push_back(Point(rect.Right(), rect.Bottom(), 0, drawing::Color(255, 0,   0, 0)));
		e.points.push_back(Point(rect.Right(), rect.Bottom(), 0, drawing::Color(255, 0,   0, 0)));
		e.points.push_back(Point(rect.x, rect.Bottom(), 0,		 drawing::Color(0,   0,   0, 0)));
		e.points.push_back(Point(rect.x, rect.y, 0, 			 drawing::Color(0,   255, 0, 0)));
		e.count = 6;
		gelements.push_back(e);
		edited = true;
	}
	size_t GraphicsElement::AddRectangle(Rectangle rect, Color color, bool fill) {
		if (!fill)
		{
			gelement e;
			e.GLMODE = GL_LINE_STRIP;
			e.points.push_back(Point(rect.x, rect.y, 0, color));
			e.points.push_back(Point(rect.Right(), rect.y, 0, color));
			e.points.push_back(Point(rect.Right(), rect.y, 0, color));
			e.points.push_back(Point(rect.Right(), rect.Bottom(), 0, color));
			e.points.push_back(Point(rect.Right(), rect.Bottom(), 0, color));
			e.points.push_back(Point(rect.x, rect.Bottom(), 0, color));
			e.points.push_back(Point(rect.x, rect.Bottom(), 0, color));
			e.points.push_back(Point(rect.x, rect.y, 0, color));
			e.count = 8;
			gelements.push_back(e);
			edited = true;
			return gelements.size() - 1;
		}
		else {
			gelement e;
			e.GLMODE = GL_TRIANGLES;
			e.points.push_back(Point(rect.x, rect.y, 0, color));
			e.points.push_back(Point(rect.Right(), rect.y, 0, color));
			e.points.push_back(Point(rect.Right(), rect.Bottom(), 0, color));
			e.points.push_back(Point(rect.Right(), rect.Bottom(), 0, color));
			e.points.push_back(Point(rect.x, rect.Bottom(), 0, color));
			e.points.push_back(Point(rect.x, rect.y, 0, color));
			e.count = 6;
			gelements.push_back(e);
			edited = true;
			return gelements.size() - 1;
		}
	}
	size_t GraphicsElement::AddGradiantRectangle(Rectangle rect, Color color1, Color color2, RectangleGradiantType rgType) {
		gelement e;
		e.GLMODE = GL_TRIANGLES;
		Color p1c, p2c, p3c, p4c;
		switch (rgType)
		{
		case RectangleGradiantType::Vertical:
			p1c = Color(color1);
			p2c = Color(color1);
			p3c = Color(color2);
			p4c = Color(color2);
			break;
		case RectangleGradiantType::Horizontal:
			p1c = Color(color1);
			p2c = Color(color2);
			p3c = Color(color2);
			p4c = Color(color1);
			break;
		case RectangleGradiantType::Inverted_Vertical:
			p1c = Color(color2);
			p2c = Color(color2);
			p3c = Color(color1);
			p4c = Color(color1);
			break;
		case RectangleGradiantType::Inverted_Horizontal:
			p1c = Color(color2);
			p2c = Color(color1);
			p3c = Color(color1);
			p4c = Color(color2);
			break;
		}
		e.points.push_back(Point(rect.x, rect.y, 0, p1c));
		e.points.push_back(Point(rect.Right(), rect.y, 0, p2c));
		e.points.push_back(Point(rect.Right(), rect.Bottom(), 0, p3c));
		e.points.push_back(Point(rect.Right(), rect.Bottom(), 0, p3c));
		e.points.push_back(Point(rect.x, rect.Bottom(), 0, p4c));
		e.points.push_back(Point(rect.x, rect.y, 0, p1c));
		e.count = 6;
		gelements.push_back(e);
		edited = true;
		return gelements.size() - 1;
	}
	bool GraphicsElement::ModifyGradiantRectangle(size_t index, Rectangle rect, Color color1, Color color2, RectangleGradiantType rgType) {
		if (index >= gelements.size() ||
			(gelements[index].GLMODE != GL_TRIANGLES) )
			return false;
		gelement* e = &gelements[index];
		e->GLMODE = GL_TRIANGLES;
		Color p1c, p2c, p3c, p4c;
		switch (rgType)
		{
		case RectangleGradiantType::Vertical:
			p1c = Color(color1);
			p2c = Color(color1);
			p3c = Color(color2);
			p4c = Color(color2);
			break;
		case RectangleGradiantType::Horizontal:
			p1c = Color(color1);
			p2c = Color(color2);
			p3c = Color(color2);
			p4c = Color(color1);
			break;
		case RectangleGradiantType::Inverted_Vertical:
			p1c = Color(color2);
			p2c = Color(color2);
			p3c = Color(color1);
			p4c = Color(color1);
			break;
		case RectangleGradiantType::Inverted_Horizontal:
			p1c = Color(color2);
			p2c = Color(color1);
			p3c = Color(color1);
			p4c = Color(color2);
			break;
		}
		e->points.clear();
		e->points.push_back(Point(rect.x, rect.y, 0, p1c));
		e->points.push_back(Point(rect.Right(), rect.y, 0, p2c));
		e->points.push_back(Point(rect.Right(), rect.Bottom(), 0, p3c));
		e->points.push_back(Point(rect.Right(), rect.Bottom(), 0, p3c));
		e->points.push_back(Point(rect.x, rect.Bottom(), 0, p4c));
		e->points.push_back(Point(rect.x, rect.y, 0, p1c));
		e->count = 6;
	
		/*if (inited) {
				
			std::vector<float> tmp;
			gelementToFloatVector(e, &tmp);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, e->startIndex, sizeof(float) * tmp.size(), tmp.data());
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		else */
		edited = true;
		return true;
	}
	size_t GraphicsElement::AddGradiantCenterRectangle(Rectangle rect, Color center, Color o_color){
		gelement e;
		e.GLMODE = GL_TRIANGLES;
		Point c = Point(rect.x + rect.width / 2, rect.y + rect.height / 2);
		e.points.push_back(Point(rect.x, rect.y, 0, o_color));
		e.points.push_back(Point(rect.Right(), rect.y, 0, o_color));
		e.points.push_back(Point(c.x, c.y, 0, center));


		e.points.push_back(Point(c.x, c.y, 0, center));
		e.points.push_back(Point(rect.x, rect.y, 0, o_color));
		e.points.push_back(Point(rect.x, rect.Bottom(), 0, o_color));	


		e.points.push_back(Point(rect.x, rect.Bottom(), 0, o_color));
		e.points.push_back(Point(rect.Right(), rect.Bottom(), 0, o_color));
		e.points.push_back(Point(c.x, c.y, 0, center));

		
		e.points.push_back(Point(c.x, c.y, 0, center));
		e.points.push_back(Point(rect.Right(), rect.Bottom(), 0, o_color));
		e.points.push_back(Point(rect.Right(), rect.y, 0, o_color));
		e.count = 12;
		edited = true;
		gelements.push_back(e);
		return gelements.size() - 1;
	}
	bool GraphicsElement::ModifyGradiantCenterRectangle(size_t index, Rectangle rect, Color center, Color o_color){
		if (index >= gelements.size() ||
			(gelements[index].GLMODE != GL_TRIANGLES) )
			return false;
		gelement* e = &gelements[index];
		e->points.clear();
		e->GLMODE = GL_TRIANGLES;
		float off = 20;
		Point c = Point(rect.x + rect.width / 2, rect.y + rect.height / 2);
		e->points.push_back(Point(rect.x, rect.y, 0, o_color));
		e->points.push_back(Point(rect.Right(), rect.y, 0, o_color));
		e->points.push_back(Point(c.x, c.y, 0, center));


		e->points.push_back(Point(c.x, c.y, 0, center));
		e->points.push_back(Point(rect.x, rect.y, 0, o_color));
		e->points.push_back(Point(rect.x, rect.Bottom(), 0, o_color));	


		e->points.push_back(Point(rect.x, rect.Bottom(), 0, o_color));
		e->points.push_back(Point(rect.Right(), rect.Bottom(), 0, o_color));
		e->points.push_back(Point(c.x, c.y, 0, center));

		
		e->points.push_back(Point(c.x, c.y, 0, center));
		e->points.push_back(Point(rect.Right(), rect.Bottom(), 0, o_color));
		e->points.push_back(Point(rect.Right(), rect.y, 0, o_color));
		e->count = 12;
		edited = true;
		return true;
	}
	void GraphicsElement::ChangeColorForAll(Color c) {
		for (auto& e : gelements) {
			for (auto& p : e.points) {
				p.color = Color(c);
			}
		}
		edited = true;
	}
	void GraphicsElement::Draw(int vpWidth, int vpHeight) {
		auto cs = shaders::ColorShader();
		cs.Use();
		cs.UniformFloat("vpWidth", vpWidth);
		cs.UniformFloat("vpHeight", vpHeight);
		cs.UniformInt("img", 0);
		if (!inited) {
			std::vector<float> a;
			for (auto& e : gelements) {
				e.startIndex = a.size();
				gelementToFloatVector(&e, &a);
				e.endIndex = a.size();
			}
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * a.size(), a.data(), GL_DYNAMIC_DRAW);
			inited = true;
		}
		if (edited) {
			std::vector<float> a;
			for (auto& e : gelements) {
				e.startIndex = a.size();
				gelementToFloatVector(&e, &a);
				e.endIndex = a.size();
			}
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * a.size(), a.data(), GL_DYNAMIC_DRAW);
			edited = false;
		}
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		size_t cc = 0;
		std::vector<size_t> offsets;
		for (auto& e : gelements) {	
			if (e.enabled) {
				if (e.GLMODE == GL_IMAGE_MODE) {
					cs.UniformInt("dmode", 1);
					glActiveTexture(GL_TEXTURE0);
					e.img->glBind();
					glDrawArrays(GL_TRIANGLES, cc, e.count);
				}
				else {
					cs.UniformInt("dmode", 0);
					glDrawArrays(e.GLMODE, cc, e.count);
				}
			}
			offsets.push_back(cc);
			cc += e.count;
		}
		for(auto ind : ontop){
			gelement e = gelements[ind];
			if (e.enabled) {
				if (e.GLMODE == GL_IMAGE_MODE) {
					cs.UniformInt("dmode", 1);
					glActiveTexture(GL_TEXTURE0);
					e.img->glBind();
					glDrawArrays(GL_TRIANGLES, offsets[ind], e.count);
				}
				else {
					cs.UniformInt("dmode", 0);
					glDrawArrays(e.GLMODE, offsets[ind], e.count);
				}
			}
		}	
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void GraphicsElement::Clear() {
		gelements.clear();
	}
	void GraphicsElement::ClearData() {
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
		inited = false;
		edited = true;
		VBO = VAO = 0;
	}
	bool GraphicsElement::Disable(size_t index, size_t count){
		if (index > gelements.size() || index + count > gelements.size()) return false;
		for (size_t i = index; i < index + count; i++)
		{
			gelements[i].enabled = false;
		}
		return true;
	}
	bool GraphicsElement::Enable(size_t index, size_t count){
		if (index > gelements.size() || index + count > gelements.size()) return false;
		for (size_t i = index; i < index + count; i++)
		{
			gelements[i].enabled = false;
		}
		return true;
	}
	void GraphicsElement::DrawOnTop(size_t index){
		if (index >= gelements.size() || index < 0) return;
		ontop.push_back(index);
	}
	void GraphicsElement::ClearColor(Color color) { cColor = Color(color); }
}