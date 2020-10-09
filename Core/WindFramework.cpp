#include <Core/WindCore.h>
#include <thread>
#define LOG_FN "log"
#include <Core/Services/LogService.h>
#include <ResourceHeaders/Images_RF.h>
std::u32string tou32(std::string s){
    std::u32string tmp;
    for(auto ch : s)
        tmp += (char32_t)ch;
    return tmp;
}
int main(){
    windowsys::oswindow osw(800, 600, "OSWindow");
    drawing::Font* fc = drawing::Font::GenerateFont(FONT_TEST, 14);
    controls::Button b(drawing::Rectangle(0, 50, 150, 40), &osw);
    controls::Button b2(drawing::Rectangle(160, 50, 150, 40), &osw);
    controls::Button b3(drawing::Rectangle(320, 50, 150, 40), &osw);
    controls::Button b4(drawing::Rectangle(750, 50, 40, 40), &osw);
    controls::ScrollBar sb(drawing::Rectangle(750, 100, 50, 300), &osw);
    controls::TextInput ti(drawing::Rectangle(0, 200, 350, 21 * 5), &osw);
    controls::Menu menu(drawing::Rectangle(0, 0, 200, 300), &osw);
    controls::ImageView iv(drawing::Rectangle(0, 550, 200, 200), &osw);
    iv.FromMemory(img_bytes, sizeof(img_bytes));
    iv.set_MouseDown([&](windowsys::MouseArgs e){
        menu.Show(e.x(), e.y());
    });
    menu.set_Font(fc);
    menu.set_ForegroundColor(drawing::Color::Black());
    menu.SetBackgroundColor(drawing::Color::White());
    menu.AddItem(U"Test", NULL);
    menu.AddItem(U"Test", NULL);
    menu.AddItem(U"Test", NULL);
    menu.AddItem(U"Test", NULL);
    drawing::animation::EasingAnimation ea(500, 0, 350,drawing::animation::EasingFunction::easeInOutBack);
    ea.set_Callback([&](float x) {
        ti.set_Position(x - 350, ti.Rectangle().y);
        osw.request_redraw();
    });
    osw.addTimer(ea.InternalTimer());

    Log_WriteLine("Setting up controls....", LogType::Debug, true);
    b.set_Font(fc);
    b.set_Text(U"easeInOutSine");
    b.set_ForegroundColor(drawing::Color::White());
    b2.set_Font(fc);
    b2.set_Text(U"easeInOutQuart");
    b2.set_ForegroundColor(drawing::Color::White());
    b3.set_Font(fc);
    b3.set_Text(U"easeInOutBack");
    b3.set_ForegroundColor(drawing::Color::White());
    b4.set_Font(fc);
    b4.set_Text(U":");
    b4.set_ForegroundColor(drawing::Color::White());
    Log_WriteLine("Setting up controls callbacks....", LogType::Debug, true);
    b.set_Click([&]{
        menu.Show(b.Rectangle().x, b.Rectangle().y);
    });
    b4.set_MouseDown([&](windowsys::MouseArgs e){
        menu.Show(b4.Rectangle().x, b4.Rectangle().y);
    });
    b2.set_Click([&]{
        ti.set_Position(-350, ti.Rectangle().y);
        ea.set_func(drawing::animation::EasingFunction::easeInOutQuart);
        ea.Start();
    });
    b3.set_Click([&]{
        ti.set_Position(-350, ti.Rectangle().y);
        ea.set_func(drawing::animation::EasingFunction::easeInOutBack);
        ea.Start();
    });
    ti.set_Font(fc);    
    ti.set_ForegroundColor(drawing::Color::Red());
    Log_WriteLine("Adding controls....", LogType::Debug, true);
    osw.addControl(&b);
    osw.addControl(&b2);
    osw.addControl(&b3);
    osw.addControl(&b4);
    osw.addControl(&ti);
    osw.addControl(&iv);
    osw.addControl(&menu);
    osw.addControl(&sb);
    osw.show();
    bool xxx = false;
    Log_WriteLine("Starting main loop....", LogType::Info, true);
    while(!glfwWindowShouldClose((GLFWwindow*)osw.nativeWindowPointer())){
        windowsys::oswindow_processRedrawRequests();
        osw.tickTimers();
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
    }
}
