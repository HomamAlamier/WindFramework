#include <Core/OSWindow/OSWindow.h>
#define LOG_FN "log"
#include <Core/Services/LogService.h>
#include <Core/Drawing/Shaders.h>
#include <thread>
std::vector<windowsys::oswindow*> oswindow_vector;
#ifdef __ANDROID__
std::thread* timers_t = NULL;
#define APP_BAR_HEIGHT 60
#endif
bool destroyRequest = false;
namespace windowsys {
    bool b_initWindowSystem = false;
    #ifdef __ANDROID__
        JNI_CALLBACK android_show_im = NULL;
        void set_android_show_im(JNI_CALLBACK cb) { android_show_im = cb; }
        int call_android_show_im() {
            if (android_show_im != NULL) return android_show_im();
            return -1;
        }
        void oswindow_ontouchEvent(float x, float y, int action){
            for(auto osw : oswindow_vector){
                osw->touchevent(x, y, action);
            }
        }
        void oswindow_timerhandler(){
            if (timers_t == NULL){
                timers_t = new std::thread([](){
                    while (!destroyRequest){
                        for(auto osw : oswindow_vector){
                            osw->tickTimers();
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    }
                });
            }
        }
        void oswindow_onkeyEvent(int key, int action, int mods){
            for(auto osw : oswindow_vector){
                osw->keyevent(key, action, mods);
            }
        }
        void oswindow_oncharkeyEvent(char32_t c){
            for(auto osw : oswindow_vector){
                osw->charkeyevent(c);
            }
        }
    #endif
    #ifdef GLFW_LIB
        double ox = 0, oy = 0;
        mouseKey last_mk;
        mouseKeyAction last_mka;
        void oswindow_ScrollCB(GLFWwindow* ptr, double x, double y){
            for(auto osw : oswindow_vector){
                if (osw->internal_ptr == ptr)
                    ox = x; oy = y;

                    for (int i = osw->basecontrol_vector.size() - 1; i >= 0; i--)
                    {
                        auto bc = osw->basecontrol_vector[i];
                        if (!bc->Enabled() || !bc->Visable()) continue;
                        if (bc->Rectangle().Contains(drawing::PointD(x, y))){
                            
                            break; 
                        }
                    }
                    break;
            }
        }
        void oswindow_processRedrawRequests(){
            for(auto osw : oswindow_vector){
                if (osw->isRedrawRequested()) osw->paintAll();
            }
        }
        void oswindow_framebuffersize_callback(GLFWwindow* ptr, int w, int h) {
            for(auto osw : oswindow_vector){
                if (osw->internal_ptr == ptr){
                    osw->resize(w, h, true);
                    osw->request_redraw();
                    break;
                }
            }
        }
        void oswindow_refreshCallBack(GLFWwindow* ptr){
            for(auto osw : oswindow_vector){
                if (osw->internal_ptr == ptr){
                    osw->request_redraw();
                    break;
                }
            }
        }
        void oswindow_KeyHandle(GLFWwindow* ptr, int key, int scancode, int action, int mods) {
            for(auto osw : oswindow_vector){
                if (osw->internal_ptr == ptr){
                    keyAction e;
                    switch (action)
                    {
                        case GLFW_PRESS: e = keyAction::press; break;
                        case GLFW_RELEASE: e = keyAction::release; break;
                        case GLFW_REPEAT: e = keyAction::repeat; break;
                    }
                    for(auto te : osw->events){
                        if (te.id == OSWINDOW_KEY_EVENT){
                            te.event(te.ptr, new KeyArgs(osw, (mods & 1) == 1, (mods & 2) == 2, (mods & 4) == 4, key, 0, e));
                        }
                    }
                    break;
                }
            }
        }
        void oswindow_CharHandle(GLFWwindow* ptr, unsigned int codepoint) {
            for(auto osw : oswindow_vector){
                if (osw->internal_ptr == ptr){
                    for(auto te : osw->events){
                        if (te.id == OSWINDOW_INPUT_CHAR_EVENT){
                            te.event(te.ptr, new KeyArgs(osw, false, false, false, 0, codepoint, keyAction::press));
                        }
                    }
                    break;
                }
            }
        }
        void oswindow_CursorMoveCB(GLFWwindow* ptr, double x, double y) {
            for(auto osw : oswindow_vector){
                if (osw->internal_ptr == ptr)
                    ox = x; oy = y;

                    for (int i = osw->basecontrol_vector.size() - 1; i >= 0; i--)
                    {
                        auto bc = osw->basecontrol_vector[i];
                        if (!bc->Enabled() || !bc->Visable()) continue;
                        if (bc->Rectangle().Contains(drawing::PointD(x, y))){
                            MouseArgs e(osw, x, y, last_mk, last_mka);
                            if (!bc->mEntered()) {
                                bc->setEntered(true);
                                bc->MouseEnter();
                                osw->setStandardCursor(bc->get_Cursor());
                            }
                            else bc->MouseMove(e);
                            break; 
                        }
                        else if (bc->mEntered()) {
                            bc->setEntered(false);
                            bc->MouseLeave();
                            osw->setStandardCursor(osw->getCursor());
                        }
                    }
                    break;
            }
        }
        void oswindow_MouseButtonCB(GLFWwindow* ptr, int button, int action, int mods) {
            for(auto osw : oswindow_vector){
                if (osw->internal_ptr == ptr){
                    mouseKey mk;
                    mouseKeyAction mka;
                    switch (button)
                    {
                        case GLFW_MOUSE_BUTTON_LEFT: mk = mouseKey::left; break;
                        case GLFW_MOUSE_BUTTON_MIDDLE: mk = mouseKey::middle; break;
                        case GLFW_MOUSE_BUTTON_RIGHT: mk = mouseKey::right; break;
                    }
                    switch (action)
                    {
                        case GLFW_PRESS: mka = mouseKeyAction::press; break;
                        case GLFW_RELEASE: mka = mouseKeyAction::release; break;
                    }
                    last_mk = mk;
                    last_mka = mka;

                    for (int i = osw->basecontrol_vector.size() - 1; i >= 0; i--)
                    {
                        auto bc = osw->basecontrol_vector[i];
                        if (!bc->Enabled() || !bc->Visable()) continue;
                        if (bc->Rectangle().Contains(drawing::PointD(ox, oy))){
                            osw->set_UnfocusAllControls();
                            if (!bc->Focused()){
                                bc->setFocus(true);
                                bc->GotFocus();
                            }
                            MouseArgs e(osw, ox, oy, mk, mka);
                            if (mka == mouseKeyAction::press) bc->MouseDown(e);
                            else bc->MouseUp(e);
                            break; 
                        }
                        else if (bc->mEntered()) {
                            bc->setEntered(false);
                            bc->MouseLeave();
                        }
                    }
                    break;
                }
            }
        }
        void glfw_error_cb(int err, const char* error){
            std::cout << "GLFW::ERROR: " << error << std::endl;
        }
    #endif
    void initWindowSystem(){
        #ifdef GLFW_LIB
            Log_WriteLine("Initing GLFW...", LogType::Info, true);
            if (!glfwInit())
            {
                Log_WriteLine("Error initing glfw !", LogType::Error, true);
                std::cout << "Error initing glfw !";
                return;
            }
        #else
            // Non GLFW
        #endif

        b_initWindowSystem = true;
    }
    oswindow::oswindow(int width, int height, const char* title){
        if (!b_initWindowSystem) initWindowSystem();
        #ifdef GLFW_LIB
            #ifndef __APPLE__
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            #else
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            #endif
            GLFWwindow* ptr;
            ptr = glfwCreateWindow(width, height, title, NULL, NULL);
            glfwSetErrorCallback(glfw_error_cb);
            glfwDefaultWindowHints();
            glfwSetWindowAttrib(ptr, GLFW_DECORATED, GLFW_TRUE);
            glfwSetWindowAttrib(ptr, GLFW_RESIZABLE, GLFW_TRUE);
            glfwSetWindowAttrib(ptr, GLFW_VISIBLE, GLFW_FALSE);
            glfwMakeContextCurrent(ptr);
            Log_WriteLine("Initing GLEW...", LogType::Info, true);
            if (glewInit() != GLEW_OK) {
                std::cout << "Error initing glew !";
                return;
            }
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            Log_WriteLine("Binding GLFW Callbacks...", LogType::Info, true);
            glfwSetFramebufferSizeCallback(ptr, oswindow_framebuffersize_callback);
            glfwSetWindowRefreshCallback(ptr, oswindow_refreshCallBack);
            glfwSetKeyCallback(ptr, oswindow_KeyHandle);
            glfwSetCharCallback(ptr, oswindow_CharHandle);
            glfwSetCursorPosCallback(ptr, oswindow_CursorMoveCB);
            glfwSetMouseButtonCallback(ptr, oswindow_MouseButtonCB);
            glfwSetScrollCallback(ptr, oswindow_ScrollCB);
            internal_ptr = ptr;
        #elif defined(__ANDROID__)
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        #endif
        this->width = width;
        this->height = height;
        this->title = title;
        ::shaders::InitAll();
        oswindow_vector.push_back(this);
    }
    void oswindow::close(){
    #ifdef GLFW_LIB
        glfwSetWindowShouldClose((GLFWwindow*)internal_ptr, GLFW_TRUE);
    #else
    #endif
    }
    void oswindow::paintAll(){
        #ifdef GLFW_LIB
            //Log_WriteLine("Repaint oswindow...( " + std::to_string(width) + "x" + st//d::to_string(height) + " )", LogType::Info, false);
            glfwMakeContextCurrent((GLFWwindow*)internal_ptr);
        #endif
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bgColor.R, bgColor.G, bgColor.B, bgColor.A);
        for(auto c : basecontrol_vector){
            if (c->Visable()) c->Paint(width, height);
        }
        #ifdef __ANDROID__
            if (appTB_Ready){
                float h = (app_scale * APP_BAR_HEIGHT) + 10;
                glViewport(0, height - h, width, h);
                appTitleBar->Draw(width, h);
                appTitleBarTitle->Render(20, (h / 2) - (appTitleBarTitle->Size().y / 2), width, h, drawing::Color::White());
                glViewport(0, 0, width, height);
            }
        #endif
        #ifdef GLFW_LIB
            glfwSwapBuffers((GLFWwindow*)internal_ptr);
            //glfwPollEvents();
        #endif
        is_redrawRequest = false;
    }
    void oswindow::tickTimers(){
        for(auto t : wtimer_vector){
            t->timetick();
        }
    }
    void oswindow::bindEvent(int id, void* ptr, OSWINDOW_EVENT value){
        Event_Tuple t;
        t.id = id;
        t.ptr = ptr;
        t.event = value;
        events.push_back(t);
    }
    #ifdef GLFW_LIB
        int oswindow::getCursor(){
            return cursor;
        }
        void oswindow::setStandardCursor(int cursor){
            if (cursor_ptr != NULL) glfwDestroyCursor(cursor_ptr);
            cursor_ptr = glfwCreateStandardCursor(cursor);
            glfwSetCursor((GLFWwindow*)internal_ptr, cursor_ptr);
        }
        void oswindow::setCustemCursor(GLFWcursor* cursor_ptr){
            if (this->cursor_ptr != NULL) glfwDestroyCursor(this->cursor_ptr);
            this->cursor_ptr = cursor_ptr;
            glfwSetCursor((GLFWwindow*)internal_ptr, this->cursor_ptr);
        }
        bool oswindow::maximized(){
            return glfwGetWindowAttrib((GLFWwindow*)internal_ptr, GLFW_MAXIMIZED) == 1;
        }
        bool oswindow::minimized(){
            return glfwGetWindowAttrib((GLFWwindow*)internal_ptr, GLFW_ICONIFIED) == 1;
        }
        void oswindow::show(){
            glfwShowWindow((GLFWwindow*)internal_ptr);
        }
        void oswindow::hide(){
            glfwHideWindow((GLFWwindow*)internal_ptr);
        }
        void oswindow::set_Resizeable(bool value) {
            glfwSetWindowAttrib((GLFWwindow*)internal_ptr, GLFW_RESIZABLE, value);
        }
        void oswindow::set_Decorated(bool value) {
            glfwSetWindowAttrib((GLFWwindow*)internal_ptr, GLFW_DECORATED, value);
        }
        void oswindow::resize(int width, int height, bool onlyupdate){
            this->width = width;
            this->height = height;
            if (!onlyupdate) glfwSetWindowSize((GLFWwindow*)internal_ptr, width, height);
        }
    #endif
    void oswindow::addControl(::controls::BaseControl* control){
        basecontrol_vector.push_back(control);
    }
    void oswindow::set_UnfocusAllControls(){
        for(auto c : basecontrol_vector){
            if (c->Focused()) {
                c->setFocus(false);
                c->LostFocus();
            }
        }
    }
    void oswindow::addTimer(::controls::w_timer* timer){
        wtimer_vector.push_back(timer);
    }
    #ifdef __ANDROID__
        void oswindow::set_AppScale(float scale) {
            app_scale = scale;
        }
        void oswindow::RegenAll() {
            for(auto c : basecontrol_vector){
                c->ReGen();
            }
        }
        void oswindow::resize(int width, int height){
            this->width = width;
            this->height = height;
            if (appTB_Ready) setupAppTitleBar(appbar_bgcolor, appbar_title, appbar_font, app_scale);
        }
        void oswindow::touchevent(float x, float y, int action) {
            //Log_WriteLine("x=" + std::to_string(x) + ", y=" + std::to_string(y), LogType::Info, true);
            mouseKey mk;
            mouseKeyAction mka;
            int act = -1;
            switch (action){
                case 0: mk = mouseKey::left; mka = mouseKeyAction::press; act = OSWINDOW_MOUSE_MOVE_EVENT; break;
                case 1: mk = mouseKey::left; mka = mouseKeyAction::press; act = OSWINDOW_MOUSE_BUTTON_EVENT; break;
                case 2: mk = mouseKey::left; mka = mouseKeyAction::release; act = OSWINDOW_MOUSE_BUTTON_EVENT; break;
            }
            //Log_WriteLine(act == 0 ? "ACTION=OSWINDOW_MOUSE_MOVE_EVENT" : "ACTION=OSWINDOW_MOUSE_BUTTON_EVENT", LogType::Info, true);
            /*for(auto te : events){
                if (te.id == act){
                    te.event(te.ptr, new MouseArgs(NULL, x, y, mk, mka));
                }
            }*/
            for (int i = basecontrol_vector.size() - 1; i >= 0; i--)
            {
                auto bc = basecontrol_vector[i];
                if (!bc->Enabled() || !bc->Visable()) continue;
                if (bc->Rectangle().Contains(drawing::PointD(x, y))){
                    set_UnfocusAllControls();
                    if (!bc->Focused()){
                        bc->setFocus(true);
                        bc->GotFocus();
                    }
                    MouseArgs e(this, x, y, mk, mka);
                    if (mka == mouseKeyAction::press) bc->MouseDown(e);
                    else bc->MouseUp(e);
                    break; 
                }
                else if (bc->mEntered()) {
                    bc->setEntered(false);
                    bc->MouseLeave();
                }
            }
        }
        void oswindow::charkeyevent(char32_t c) {
            for(auto te : events){
                if (te.id == OSWINDOW_INPUT_CHAR_EVENT){
                    te.event(te.ptr, new KeyArgs(NULL, false, false, false, 0, c, keyAction::press));
                }
            }
        }
        void oswindow::keyevent(int key, int action, int mods) {
            keyAction e;
            switch (action)
            {
                case 0: e = keyAction::press; break;
                case 1: e = keyAction::release; break;
                case 2: e = keyAction::repeat; break;
            }
            for(auto te : events){
                if (te.id == OSWINDOW_KEY_EVENT){
                    te.event(te.ptr, new KeyArgs(NULL, (mods & 1) == 1, (mods & 2) == 2, (mods & 4) == 4, key, 0, e));
                }
            }
        }
        void oswindow::setupAppTitleBar(drawing::Color c, std::u32string title, drawing::Font* fnt, float scale){
            appTitleBar = new drawing::GraphicsElement(drawing::Rectangle(0, 0, width, 50));
            appTitleBar->AddRectangle(drawing::Rectangle(0, 0, width, APP_BAR_HEIGHT * scale), c, true);
            appTitleBar->AddGradiantRectangle(drawing::Rectangle(0, APP_BAR_HEIGHT * scale, width, 10), drawing::Color(0, 0, 0, 120), drawing::Color::Transparent(), RectangleGradiantType::Vertical);
            appTitleBar->AddArc(drawing::Point(width - (30 * scale), (20 * scale)), 5, 0, 360, drawing::Color::White());
            appTitleBar->AddArc(drawing::Point(width - (30 * scale), (20 * scale) + 15), 5, 0, 360, drawing::Color::White());
            appTitleBar->AddArc(drawing::Point(width - (30 * scale), (20 * scale) + 30), 5, 0, 360, drawing::Color::White());
            appTitleBarTitle = new drawing::TextBlock(fnt->RenderTextBlock(title));
            if (appbar_title != title) appbar_title = title;
            appbar_bgcolor = c;
            appbar_font = fnt;
            appTB_Ready = true;
            Log_WriteLine("AppTitleBar Height = " + std::to_string(APP_BAR_HEIGHT * scale) + " ( scale = " + std::to_string(scale) + " )", LogType::Debug, true);
        }
    #endif
}