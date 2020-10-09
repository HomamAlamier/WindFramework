#ifndef BASECONTROL_H
#define BASECONTROL_H

#include <Core/Drawing/Rectangle.h>
#include <Core/Drawing/GraphicsElement.h>
#include <GL.h>
#include <Core/Drawing/Font.h>
#include <Core/OSWindow/EventArgs.h>
#include <functional>
namespace controls {
	typedef struct {
		char32_t Char;
		bool Control;
		bool Shift;
		bool Alt;
		int Key;
	} KeyArgs;
	class BaseControl {
	protected:
		::drawing::GraphicsElement* ge = NULL;
		::drawing::Rectangle rect;
		drawing::Font* font = NULL;
		bool visable = true;
		bool enabled = true;
		void* parent = NULL;
		::drawing::Color bg, fg;
		int w, h;
		std::function<void(windowsys::MouseArgs)> fn_mousedown = NULL;
		std::function<void(windowsys::MouseArgs)> fn_mouseup = NULL;
		std::function<void(windowsys::MouseArgs)> fn_mousemove = NULL;
		std::function<void(windowsys::MouseScrollArgs)> fn_mousescroll = NULL;
#ifdef GLFW_LIB
		int cursor = GLFW_ARROW_CURSOR;
#endif
		bool mentered = false,
			focus = false;
	public:
		BaseControl(::drawing::Rectangle rect, void* parent);
		virtual void DrawingSetup();
		virtual void Invalidate();
		virtual void SetBackgroundColor(::drawing::Color color);
		virtual ::drawing::Color BackgroundColor();
		virtual void Paint(int vpWidth, int vpHeight);
		virtual void MouseMove(windowsys::MouseArgs e);
		virtual void MouseDown(windowsys::MouseArgs e);
		virtual void MouseUp(windowsys::MouseArgs e);
		virtual void MouseScroll(windowsys::MouseScrollArgs e);
		virtual void MouseEnter();
		virtual void MouseLeave();
		virtual void GotFocus();
		virtual void LostFocus();
		virtual void KeyCharPress(KeyArgs e);
		virtual void KeyPress(KeyArgs e);
		virtual void set_Font(drawing::Font* font) { this->font = font; }
		virtual void set_Position(float x, float y);
		virtual void set_Size(float width, float height);
		virtual drawing::Rectangle Rectangle() { return rect; }
		virtual drawing::Font* Font() { return font; }
		virtual void set_ForegroundColor(::drawing::Color color) { fg = color; }
		virtual ::drawing::Color ForegroundColor() { return fg; }
		virtual bool Visable() { return visable; }
		virtual void set_Visable(bool value) { visable = value; }
		virtual bool Enabled() { return enabled; }
		virtual void set_Enabled(bool value) { enabled = value; }
		virtual void set_MouseDown(std::function<void(windowsys::MouseArgs)> func) { fn_mousedown = func; }
		virtual void set_MouseUp(std::function<void(windowsys::MouseArgs)> func) { fn_mouseup = func; }
		virtual void set_MouseMove(std::function<void(windowsys::MouseArgs)> func) { fn_mousemove = func; }
		virtual void set_MouseScroll(std::function<void(windowsys::MouseScrollArgs)> func) { fn_mousescroll = func; }
		virtual void ReGen() { ge->ClearData(); }
#ifdef GLFW_LIB
		virtual void set_Cursor(int cursor) { this->cursor = cursor; }
		virtual int get_Cursor() { return cursor; }
#endif
		bool mEntered() { return mentered; }
		bool Focused() { return focus; }
		void setFocus(bool value) { focus = value; }
		void setEntered(bool value) { mentered = value; }
		void* get_Parent() { return parent; }
	};
}
#endif // !BASECONTROL_H
