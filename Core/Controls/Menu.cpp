#include <Core/Controls/Menu.h>
#include <Core/OSWindow/OSWindow.h>
#define GE_MENU_OFFSET 1
#define LOG_FN "log"

#include <Core/Services/LogService.h>
namespace controls {
    Menu::Menu(drawing::Rectangle rect, void* window) : BaseControl(rect, window){
        shadows = new drawing::GraphicsElement(rect);
        shadows->AddGradiantCenterRectangle(drawing::Rectangle(0, -5, rect.width, 5), drawing::Color(0, 0, 0), drawing::Color::Transparent());
        ge->Clear();
        wid = rect.width;
        height = rect.height;
        main_rect = ge->AddRectangle(rect, bg, true);
        visable = false;
        opened = false;
    }
    void Menu::Paint(int vpWidth, int vpHeight){
        float off = 7;
        shadows->ModifyGradiantCenterRectangle(0, drawing::Rectangle(rect.x - off, rect.y - off, rect.width + 8 + off, rect.height + 12 + off), drawing::Color::Black(), drawing::Color(0, 0, 0, 0));
        shadows->Draw(vpWidth, vpHeight);
        glViewport(rect.x, (vpHeight - (rect.height + 5) ) - rect.y, rect.width, rect.height + 5);
        ge->Draw(rect.width, rect.height);
        for(auto item : items){
            item.tb->Render(item.rect.x, item.rect.y + ((item.rect.height / 2) - (item.tb->Size().y / 2)), rect.width, rect.height, fg);
        }
        glViewport(0, 0, vpWidth, vpHeight);
    }
    void Menu::LostFocus() {
        Hide();
    }
    void Menu::Show(float x, float y){
        shadows->Enable(0);
        hover_index = -1;
        rect.x = x; rect.y = y;
        rect.width = 0; rect.height = 0;
        sx = x;
        sy = y;
        ge->ModifyRectangle(0, drawing::Rectangle(0, 0, wid, height), bg, true);
        bool isBig = x + wid > ((windowsys::oswindow*)parent)->getWidth();
        if (ea_w == NULL && ea_h == NULL){
            ea_w = new drawing::animation::EasingAnimation(250, 0, wid, drawing::animation::EasingFunction::easeInOutQuart);
            ea_h = new drawing::animation::EasingAnimation(500, 0, height, drawing::animation::EasingFunction::easeInOutQuart);
            ea_w->set_Callback([&](float x) {
                if (sx + wid < ((windowsys::oswindow*)get_Parent())->getWidth()) rect.width = x;
                else {
                    rect.x = (sx - x) + (((windowsys::oswindow*)get_Parent())->getWidth() - sx);
                    rect.width = x;
                }
                Invalidate();
            });
            ea_h->set_Callback([&](float x) {
                rect.height = x;
                Invalidate();
            });
            ((windowsys::oswindow*)get_Parent())->addTimer(ea_w->InternalTimer());
            ((windowsys::oswindow*)get_Parent())->addTimer(ea_h->InternalTimer());
        }
        else {
            ea_w->set_end(wid);
            ea_h->set_end(height);
        }
        visable = true;
        ea_w->set_duration(250);
        ea_h->set_duration(500);
        ea_w->Start();
        ea_h->Start();
        opened = true;
    }
    void Menu::Hide(){
        if (!opened) return;
        shadows->Disable(0);
        ea_w->set_duration(500);
        ea_h->set_duration(250);
        ea_w->Reverse();
        ea_h->Reverse();
        ea_w->InternalTimer()->set_endCallback([&]{
            this->set_Visable(false);
            ea_w->InternalTimer()->set_endCallback(NULL);
        });
        opened = false;
    }
    void Menu::MouseLeave(){
        Hide();
        hover_index = -1;
    }
    void Menu::MouseMove(windowsys::MouseArgs e){
        double x = e.x() - rect.x;
        double y = e.y() - rect.y;
        bool cc = false;
        size_t cci;
        for (size_t i = 0; i < items.size(); i++)
        {
            if (items[i].rect.Contains(drawing::PointD(x, y)) && i != hover_index){
                hover_index = i;
                ge->Enable(GE_MENU_OFFSET + i);
                cci = i;
                cc = true;
            }
        }
        if (cc) {
            for (size_t i = 0; i < items.size(); i++)
            {
                if (i != cci) ge->Disable(GE_MENU_OFFSET + i);
            }    
            Invalidate();
        }
    }
    void Menu::ReGen() {
        ge->ClearData();
        shadows->ClearData();
        for (int i = 0; i < items.size(); ++i) {
            (&items[i])->tb = new drawing::TextBlock(font->RenderTextBlock(items[i].text));
            (&items[i])->rect = drawing::Rectangle(0, i * (font->Height() + 30), wid, (font->Height() + 30));
        }
        height = (font->Height() + 30) * items.size();
        float w = 0;
        for(auto i : items){
            if (i.tb->Size().x > w) w = i.tb->Size().x;
        }
        wid = w + 20;
        Log_WriteLine("Regenrated Width = " + std::to_string(w), LogType::Debug, true);
        if (opened){
            rect.height = height;
            rect.width = w;
        }
    }
    void Menu::AddItem(std::u32string text, std::function<void()> cb, int** shortcut){
        Menu_Item mi;
        mi.text = text;
        mi.cb = cb;
        mi.shortcut = shortcut;
        mi.tb = new drawing::TextBlock(font->RenderTextBlock(text));
        auto ss = font->Height() + 30;
        height = ss * items.size();
        float w = 0;
        for(auto i : items){
            if (i.tb->Size().x > w) w = i.tb->Size().x;
        }
        wid = w + 20;
        for (size_t i = 0; i < items.size(); i++)
        {
            (&items[i])->rect.width = wid;
            ge->ModifyRectangle(GE_MENU_OFFSET + i, (&items[i])->rect, drawing::Color::Red(), true);
        }    
        mi.rect = drawing::Rectangle(0, ss * items.size(), wid, ss);
        ge->Disable(ge->AddRectangle(mi.rect, drawing::Color::Red(), true));
        items.push_back(mi);
    }
    void Menu::SetBackgroundColor(drawing::Color color){
        ge->ModifyRectangle(0, rect, color, true);
        bg = color;
    }
}