#ifndef EVENTARGS_H
#define EVENTARGS_H

namespace windowsys{
    enum class keyAction { none, press, release, repeat };
    enum class mouseKey { none, left, right, middle };
    enum class mouseKeyAction { none, release, press };
    enum class mouseScrollDir { up, down, left, right };


    class oswindow;
    class OsWindow_EventArgs { 
        oswindow* window_ptr = 0;
    public:
        OsWindow_EventArgs(oswindow* windowPtr) { window_ptr = windowPtr; }
        oswindow* get_WindowPointer() { return window_ptr; }
    };
    class MouseArgs : public OsWindow_EventArgs{
        double _x = 0, _y = 0;
        mouseKey _key = mouseKey::none;
        mouseKeyAction _action = mouseKeyAction::none;
    public:
        MouseArgs(oswindow* windowPtr, double x, double y, mouseKey key, mouseKeyAction action) : OsWindow_EventArgs(windowPtr) {
            _x = x;
            _y = y;
            _key = key;
            _action = action;
        }
        double x() { return _x; }
        double y() { return _y; }
        drawing::PointD get_PointD() { return drawing::PointD(_x, _y); }
        mouseKey key() { return _key; }
        mouseKeyAction action() { return _action; }
    };
    class KeyArgs : public OsWindow_EventArgs{
        bool b_shift = false, b_ctrl = false, b_alt = false;
        int _key;
        char32_t ch;
        keyAction e;
    public:
        KeyArgs(oswindow* windowPtr, bool b_shift, bool b_ctrl, bool b_alt, int key, char32_t ch, keyAction e) : OsWindow_EventArgs(windowPtr) {
            this->b_shift = b_shift;
            this->b_ctrl = b_ctrl;
            this->b_alt = b_alt;
            _key = key;
            this->ch = ch;
            this->e = e;
        }
        bool shift() { return b_shift; }
        bool alt() { return b_alt; }
        bool control() { return b_ctrl; }
        int key() { return _key; }
        char32_t _char() { return ch; }
        keyAction action() { return e; }
    };
    class MouseScrollArgs : public OsWindow_EventArgs {
        mouseScrollDir ms;
    public:
        MouseScrollArgs(oswindow* windowPtr, mouseScrollDir ms) : OsWindow_EventArgs(windowPtr) { this->ms = ms; }
        mouseScrollDir ScrollDir() { return ms; }
    };
    typedef void (*OSWINDOW_EVENT)(void* ptr, OsWindow_EventArgs* e);
}
#endif // !EVENTARGS_H