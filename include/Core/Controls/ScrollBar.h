#ifndef SCROLL_BAR_H
#define SCROLL_BAR_H

#include <Core/Controls/BaseControl.h>

namespace controls {
    class ScrollBar : public BaseControl {
        float ocurrent = 0, current = 0, max = 100, factor = 2.0f;
        drawing::Rectangle scrollArea;
        drawing::PointD mdown;
    public:
        ScrollBar(drawing::Rectangle rect, void* parent);
        void Paint(int vpWidth, int vpHeight);
        void MouseMove(windowsys::MouseArgs e);
        void MouseDown(windowsys::MouseArgs e);
        //void set_Maximum(float value);
        //void set_Value(float value);
        float get_Value() { return current; }
    };
}


#endif // !SCROLL_BAR_H