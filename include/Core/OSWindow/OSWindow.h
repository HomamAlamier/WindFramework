#ifndef OSWINDOW_H
    #define OSWINDOW_H

    #include <GL.h>
    #include <Core/Drawing/Color.h>
    #include <Core/Drawing/GraphicsElement.h>
    #include <functional>
    #include <chrono>
    #include <vector>
    #include <string>
    #include <Core/Controls/w_Timer.h>
    #include <map>
    #include <Core/Controls/BaseControl.h>
    #include <Core/OSWindow/EventArgs.h>


    #define OSWINDOW_NULL_EVENT 0x0
    #define OSWINDOW_MOUSE_MOVE_EVENT 0x1
    #define OSWINDOW_MOUSE_BUTTON_EVENT 0x2
    #define OSWINDOW_KEY_EVENT 0x3
    #define OSWINDOW_INPUT_CHAR_EVENT 0x4
    #define OSWINDOW_RESIZE_EVENT 0x5
    #define OSWINDOW_CLOSE_EVENT 0x6
    
    namespace windowsys {

        #ifdef __ANDROID__
            typedef int (*JNI_CALLBACK)();
            void set_android_show_im(JNI_CALLBACK cb);
            int call_android_show_im();
            void oswindow_ontouchEvent(float x, float y, int action);
            void oswindow_onkeyEvent(int key, int action, int mods);
            void oswindow_oncharkeyEvent(char32_t c);
            void oswindow_timerhandler();
        #endif
        void initWindowSystem();
        void oswindow_processRedrawRequests();


        

        struct Event_Tuple{
            int id = OSWINDOW_NULL_EVENT;
            void* ptr = NULL;
            OSWINDOW_EVENT event = NULL;
        };

        class oswindow{
            void* internal_ptr = NULL;
            std::vector<::controls::BaseControl*> basecontrol_vector;
            std::vector<::controls::w_timer*> wtimer_vector;
            std::vector<Event_Tuple> events;
            drawing::Color bgColor = drawing::Color::White();
            bool is_redrawRequest = false;
            int width = 0;
            int height = 0;
            std::string title;
            #ifdef __ANDROID__
                drawing::GraphicsElement* appTitleBar;
                drawing::TextBlock* appTitleBarTitle;
                std::u32string appbar_title;
                drawing::Color appbar_bgcolor;
                drawing::Font* appbar_font;
                bool appTB_Ready = false;
                float app_scale = 1.0f;
            #endif
            #ifdef GLFW_LIB
                int cursor = GLFW_ARROW_CURSOR;
                GLFWcursor* cursor_ptr = NULL;
                friend void oswindow_framebuffersize_callback(GLFWwindow* ptr, int w, int h);
                friend void oswindow_refreshCallBack(GLFWwindow* ptr);
                friend void oswindow_KeyHandle(GLFWwindow* ptr, int key, int scancode, int action, int mods);
                friend void oswindow_CharHandle(GLFWwindow* ptr, unsigned int codepoint);
                friend void oswindow_CursorMoveCB(GLFWwindow* wind, double x, double y);
                friend void oswindow_MouseButtonCB(GLFWwindow* window, int button, int action, int mods);
                friend void oswindow_ScrollCB(GLFWwindow* ptr, double x, double y);
            #endif // DEBUG

        public:
            oswindow(int width, int height, const char* title);
            void close();
            void addControl(controls::BaseControl* control);
            void set_UnfocusAllControls();
            void addTimer(controls::w_timer* timer);
            void bindEvent(int event, void* ptr, OSWINDOW_EVENT value);
            #ifdef GLFW_LIB
                void resize(int width, int height, bool onlyupdate = false);
                int getCursor();
                void setStandardCursor(int cursor);
                void setCustemCursor(GLFWcursor* cursor_ptr);
                bool maximized();
                bool minimized();
                void show();
                void hide();
                void set_Resizeable(bool value);
                void set_Decorated(bool value);
                void request_redraw() { is_redrawRequest = true; }
                bool isRedrawRequested() { return is_redrawRequest; }
            #elif defined(__ANDROID__)
                void resize(int width, int height);
                void touchevent(float x, float y, int action);
                void keyevent(int key, int action, int mods);
                void charkeyevent(char32_t c);
                void setupAppTitleBar(drawing::Color c, std::u32string title, drawing::Font* fnt, float scale = 1.0f);
                void RegenAll();
                void set_AppScale(float scale);
            #endif // DEBUG
            void paintAll();
            void tickTimers();
            void* nativeWindowPointer() { return internal_ptr; }
            int getHeight() { return height; }
            int getWidth() { return width; }
            std::string getTitle() { return title; }
            drawing::Color getBackgroundColor() { return bgColor; }
            void setBackgroundColor(drawing::Color color) { bgColor = color; }
        };
    }
#endif