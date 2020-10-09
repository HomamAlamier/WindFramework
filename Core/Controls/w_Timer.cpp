#include <Core/Controls/w_Timer.h>
#include <chrono>
#include <GL.h>
namespace controls {
    w_timer::w_timer(int inter){
        interval = inter;
    }
    void w_timer::start(){
        enabled = true;
        c_runfor_interval = 0;
        lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if (s_cb != NULL) s_cb();
    }
    void w_timer::stop(){
        enabled = false;
    }
    void w_timer::timetick(){
        if (!enabled) return;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if (ms - lastTime >= interval){
            if (tick != NULL) tick(ptr);
            if (!infinite){
                if (c_runfor_interval >= runfor_interval) {
                    enabled = false;
                    if (e_cb != NULL) e_cb();
                }
                else c_runfor_interval += ms - lastTime;
            }
            lastTime = ms;
            performCount++;
        }
    }
    void w_timer::reset(){
        lastTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
    void w_timer::set_tick(std::function<void(void*)> func, void* ptr) { tick = func; this->ptr = ptr; }
    void w_timer::set_infinite(bool v) { infinite = v; }
    void w_timer::set_interval(int v) { interval = v; }
    void w_timer::set_runfor_interval(int64_t v) { runfor_interval = v; }
}