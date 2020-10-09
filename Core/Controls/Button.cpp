#include <Core/Controls/Button.h>
#include <Core/OSWindow/OSWindow.h>
#include <iostream>
namespace controls {
	Button::Button(drawing::Rectangle rect, void* window) : BaseControl(rect, window) {
		DrawingSetup();
		cbg = bg;
		ca = new drawing::animation::ColorAnimation(200, bg, drawing::Color::Red());
		ca->set_ChangeCallback([&](drawing::Color color) {
			ge->ChangeColorForAll(color);
			cbg = color;
			Invalidate();
		});
		((windowsys::oswindow*)window)->addTimer(ca->InternalTimer());
	}
	void Button::DrawingSetup() {
		ge->Clear();
		ge->AddRectangle(drawing::Rectangle(0, 0, rect.width, rect.height), bg, true);
	}
	void Button::Paint(int vpWidth, int vpHeight){
		glViewport(rect.x, (vpHeight - rect.height) - rect.y, rect.width, rect.height);
		glClearColor(1, 1, 1, 1);
		ge->Draw(rect.width, rect.height);
		if (tb != NULL) 
			tb->Render((rect.width / 2) - (tb->Size().x / 2), (rect.height / 2) - ((tb->Size().y / 2)), rect.width, rect.height, fg);
		glViewport(0, 0, vpWidth, vpHeight);
	}
	void Button::MouseEnter() {
		ca->Start();
	}
	void Button::MouseLeave() {
		ca->set_StartColor(bg);
		ca->set_EndColor(drawing::Color::Red());
		ca->Reverse();
	}
	void Button::MouseMove(windowsys::MouseArgs e) {
		if (fn_mousemove != NULL) fn_mousemove(e);
	}
	void Button::MouseDown(windowsys::MouseArgs e) {
		std::cout << "MouseDown !" << std::endl;
		ca->set_StartColor(cbg);
		ca->set_EndColor(drawing::Color::Green());
		ca->Start();
		if (fn_mousedown != NULL) fn_mousedown(e);
	}
	void Button::MouseUp(windowsys::MouseArgs e) {
		std::cout << "MouseUp !" << std::endl;
		ca->Reverse();
		if (click_fn != NULL) click_fn();
		if (fn_mouseup != NULL) fn_mouseup(e);
	}
	void Button::set_Font(drawing::Font* font){
		this->font = font;
		tb = new drawing::TextBlock(font->RenderTextBlock(text));
		Invalidate();
	}
	void Button::set_Text(std::u32string text){
		this->text = text;
		tb->~TextBlock();
		tb = new drawing::TextBlock(font->RenderTextBlock(text));
		Invalidate();
	}
	void Button::set_Click(std::function<void()> func){
		click_fn = func;
	}
}