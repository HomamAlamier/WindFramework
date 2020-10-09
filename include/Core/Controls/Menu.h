#ifndef MENU_H
#define MENU_H
#include <Core/Controls/BaseControl.h>
#include <Core/Drawing/Animation/easeAnimation.h>
namespace controls{\
    struct Menu_Item {
        std::u32string text;
        drawing::Rectangle rect;
        int** shortcut = NULL;
        std::function<void()> cb;
        drawing::TextBlock* tb;
    };
    class Menu : public BaseControl {
        std::vector<Menu_Item> items;
        float wid, height, sx, sy;
        size_t main_rect;
        size_t hover_index;
        drawing::animation::EasingAnimation *ea_w = NULL, *ea_h = NULL;
        bool opened = false;
        drawing::GraphicsElement* shadows;
    public:
        Menu(drawing::Rectangle rect, void* window);
        void MouseMove(windowsys::MouseArgs e);
		//void MouseDown(windowsys::MouseArgs e);
		//void MouseUp(windowsys::MouseArgs e);
		void Paint(int vpWidth, int vpHeight);
       // void MouseEnter();
        void SetBackgroundColor(::drawing::Color color);
		void MouseLeave();
        void Show(float x, float y);
        void Hide();
        void AddItem(std::u32string name, std::function<void()> cb, int** shortcut = NULL);
        void LostFocus();
        void ReGen();
    };
}

#endif // !MENU_H