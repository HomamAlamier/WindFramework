#ifndef W_TIMER_H
#define W_TIMER_H

#include <functional>
namespace controls {
    class w_timer
    {
        int64_t lastTime;	
        int64_t performCount = 0;
        int64_t runfor_interval = 0, c_runfor_interval = 0;
        std::function<void(void*)> tick;
        std::function<void()> s_cb = NULL;
        std::function<void()> e_cb = NULL;
        void* ptr = NULL;
        int interval = 0;
        bool enabled = false;
        bool infinite = false;
    public:
        w_timer(){}
        w_timer(int interval);
        void set_tick(std::function<void(void*)> func, void* ptr = NULL);
        void set_interval(int interval);
        void set_infinite(bool val);
        void set_runfor_interval(int64_t v);
        void reset();
        void start();
        void stop();
        void timetick();
        int64_t get_c_runfor_time() { return c_runfor_interval; }
        bool running() { return enabled; }
        void set_startCallback(std::function<void()> cb) { s_cb = cb; }
        void set_endCallback(std::function<void()> cb) { e_cb = cb; }
    };
}

#endif // !W_TIMER_H