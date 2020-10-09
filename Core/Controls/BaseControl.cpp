#include <Core/Controls/BaseControl.h>
#include <Core/OSWindow/OSWindow.h>
//build
namespace controls {
	void BaseControl_HandleCMove(void* ptr, windowsys::OsWindow_EventArgs* ea) {
		windowsys::MouseArgs e = *((windowsys::MouseArgs*)ea);
		BaseControl* bc = ((BaseControl*)ptr);
		if (!bc->Enabled() || !bc->Visable()) return;
		double x = e.x(), y = e.y();
		if (bc->Rectangle().Contains(drawing::PointD(x, y))) {
			if (bc->mEntered()) bc->MouseMove(e);
			else {
			#ifdef GLFW_LIB
				((windowsys::oswindow*)bc->get_Parent())->setStandardCursor(bc->get_Cursor());
			#endif
				bc->setEntered(true);
				bc->MouseEnter();
			}
		}
		else if (bc->mEntered()) {
			bc->setEntered(false);
			bc->MouseLeave();
			#ifdef GLFW_LIB
				((windowsys::oswindow*)bc->get_Parent())->setStandardCursor(((windowsys::oswindow*)bc->get_Parent())->getCursor());
			#endif
		}
	}
	void BaseControl_HandleMButton(void* ptr, windowsys::OsWindow_EventArgs* ea) {
		windowsys::MouseArgs e = *((windowsys::MouseArgs*)ea);
		BaseControl* bc = ((BaseControl*)ptr);
		if (!bc->Enabled() || !bc->Visable()) return;
		double x = e.x(), y = e.y();
		bool down = e.action() == windowsys::mouseKeyAction::press;
		if (bc->Rectangle().Contains(drawing::PointD(x, y))) {
			((windowsys::oswindow*)bc->get_Parent())->set_UnfocusAllControls();
			if (!bc->Focused()){
				bc->setFocus(true);
				bc->GotFocus();
			}
			if (down) bc->MouseDown(e);
			else bc->MouseUp(e);
		}
	}
	void BaseControl_HandleKeyPress(void* ptr, windowsys::OsWindow_EventArgs* ea) {
		windowsys::KeyArgs e = *((windowsys::KeyArgs*)ea);
		BaseControl* bc = ((BaseControl*)ptr);
		if (!bc->Enabled() || !bc->Visable() || !bc->Focused()) return;
		bc->KeyCharPress({ e._char(), e.control(), e.shift(), e.alt() });
	}
	void BaseControl_HandleKeyChange(void* ptr, windowsys::OsWindow_EventArgs* ea) {
		windowsys::KeyArgs e = *((windowsys::KeyArgs*)ea);
		BaseControl* bc = ((BaseControl*)ptr);
		if (!bc->Enabled() || !bc->Visable() || !bc->Focused()) return;
		if (e.action() == windowsys::keyAction::repeat || e.action() == windowsys::keyAction::press) 
			bc->KeyPress({ 0, e.control(), e.shift(), e.alt(), e.key() });
		//else if (actio//n == GLFW_RELEASE)
	}
	BaseControl::BaseControl(drawing::Rectangle rect, void* parent) {
		ge = new drawing::GraphicsElement(rect);
		this->rect = rect;
		this->parent = parent;
		((windowsys::oswindow*)parent)->bindEvent(OSWINDOW_MOUSE_MOVE_EVENT, this, BaseControl_HandleCMove);
		((windowsys::oswindow*)parent)->bindEvent(OSWINDOW_MOUSE_BUTTON_EVENT, this, BaseControl_HandleMButton);
		((windowsys::oswindow*)parent)->bindEvent(OSWINDOW_INPUT_CHAR_EVENT, this, BaseControl_HandleKeyPress);
		((windowsys::oswindow*)parent)->bindEvent(OSWINDOW_KEY_EVENT, this, BaseControl_HandleKeyChange);
		DrawingSetup();
	}
	void BaseControl::Paint(int vpWidth, int vpHeight) {
		if (ge != NULL) ge->Draw(vpWidth, vpHeight);
	}
	void BaseControl::DrawingSetup() {
		ge->AddRectangle(drawing::Rectangle(0, 0, rect.width, rect.height), drawing::Color::Red(), false);
	}
	void BaseControl::Invalidate() {
    #ifdef GLFW_LIB
        ((windowsys::oswindow*)get_Parent())->request_redraw();
    #endif
	}
	void BaseControl::MouseMove(windowsys::MouseArgs e) { if (fn_mousemove != NULL) fn_mousemove(e); }
	void BaseControl::MouseDown(windowsys::MouseArgs e) { if (fn_mousedown != NULL) fn_mousedown(e); }
	void BaseControl::MouseUp(windowsys::MouseArgs e) { if (fn_mouseup != NULL) fn_mouseup(e); }
	void BaseControl::MouseScroll(windowsys::MouseScrollArgs e) { if (fn_mousescroll != NULL) fn_mousescroll(e); }
	void BaseControl::MouseLeave() {}
	void BaseControl::MouseEnter() {}
	void BaseControl::LostFocus() {}
	void BaseControl::GotFocus() {}
	void BaseControl::KeyCharPress(KeyArgs ka) {}
	void BaseControl::KeyPress(KeyArgs e) {}
	void BaseControl::SetBackgroundColor(drawing::Color color) { bg = color; }
	drawing::Color BaseControl::BackgroundColor() { return bg; }
	void BaseControl::set_Position(float x, float y) 
	{ 
		rect.x = x;
		rect.y = y;
	}
	void BaseControl::set_Size(float width, float height){
		rect.width = width;
		rect.height = height;
	}
}