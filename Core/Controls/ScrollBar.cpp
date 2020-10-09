#include <Core/Controls/ScrollBar.h>
#define LOG_FN "log"
#include <Core/Services/LogService.h>

namespace controls {
    ScrollBar::ScrollBar(drawing::Rectangle rect, void* window) : BaseControl(rect, window) {
        ge->Clear();
        ge->AddRectangle(drawing::Rectangle(0, 0, rect.width, rect.height), bg, true);
        float half = max / factor;
        ge->AddRectangle(drawing::Rectangle(0, 0, rect.width, rect.height - half), drawing::Color::Red(), true);
        scrollArea = drawing::Rectangle(0, rect.height - half, rect.width, half);
    }
    void ScrollBar::Paint(int vpWidth, int vpHeight){
        if (current != ocurrent){
            ge->ModifyRectangle(1, drawing::Rectangle(0, current / factor, rect.width, rect.height - (max / factor)), drawing::Color::Red(), true);
            ge->ClearData();
            ocurrent = current;
        }
        glViewport(rect.x, (vpHeight - rect.height) - rect.y, rect.width, rect.height);
        ge->Draw(rect.width, rect.height);
        glViewport(0, 0, vpWidth, vpHeight);
    }
    void ScrollBar::MouseMove(windowsys::MouseArgs e){
        if (e.key() == windowsys::mouseKey::left && e.action() == windowsys::mouseKeyAction::press){
            auto delta = e.get_PointD().offset(-rect.x, -rect.y) - mdown;
            if (delta.y > 0 && current < max) {
                current += delta.y * factor;
                if (current > max) current = max;
            } 
            else if (delta.y < 0 && current > 0) {
                current += delta.y * factor;
                if (current < 0) current = 0;
            }
            mdown = e.get_PointD().offset(-rect.x, -rect.y);
            Invalidate();
        }
    }
    void ScrollBar::MouseDown(windowsys::MouseArgs e){
        mdown = e.get_PointD().offset(-rect.x, -rect.y);
    }
}