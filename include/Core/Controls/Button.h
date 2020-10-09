#ifndef BUTTON_H
#define BUTTON_H

class BaseControl;
#include <Core/Controls/BaseControl.h>
#include <Core/Drawing/Font.h>
#include <Core/Drawing/Animation/ColorAnimation.h>
namespace controls {
	class Button : public BaseControl {
		std::function<void()> click_fn;
		std::u32string text = U"Button";
		drawing::TextBlock* tb;
		drawing::animation::ColorAnimation* ca;
		drawing::Color cbg;
	public:
		Button(drawing::Rectangle rect, void* window);
		void DrawingSetup();
		void Paint(int vpWidth, int vpHeight);
		void MouseMove(windowsys::MouseArgs e);
		void MouseDown(windowsys::MouseArgs e);
		void MouseUp(windowsys::MouseArgs e);
		void MouseEnter();
		void MouseLeave();
		void set_Font(drawing::Font* font);
		void set_Text(std::u32string text);
		void set_Click(std::function<void()> func);
	};
}
#endif // !BUTTON_H

