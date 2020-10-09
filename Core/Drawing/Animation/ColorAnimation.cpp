#include <Core/Drawing/Animation/ColorAnimation.h>

namespace drawing {
    namespace animation {  
        void internal_tick(void* ptr){
            auto p = (ColorAnimation*)ptr;
            p->Change();
        }
        ColorAnimation::ColorAnimation(int duration, Color startColor, Color endColor) : AnimationBase(duration){
            auto c1 = Color::Max(startColor, endColor);
            auto c2 = Color::Min(startColor, endColor);
            float x = (c1.R - c2.R) + (c1.G - c2.G) + (c1.B - c2.B) + (c1.A - c2.A);
            a_step = x / duration;
            a_color = startColor;
            s_color = startColor;
            e_color = endColor;
            internalTimer->set_tick(internal_tick, this);
            internalTimer->set_runfor_interval(duration);
        }
        void ColorAnimation::set_StartColor(Color color){
            s_color = color;
        }
        void ColorAnimation::set_EndColor(Color color){
            e_color = color;
        }
        void ColorAnimation::set_ChangeCallback(std::function<void(Color)> callback){
            _cb = callback;
        }
        void ColorAnimation::Reverse(){
            a_color = e_color;
            rev = true;
            internalTimer->start();
        }
        void ColorAnimation::Start(){
            a_color = s_color;
            rev = false;
            internalTimer->start();
        }
        void ColorAnimation::Change() {
            if (rev)
                a_color = Color::Translate(e_color, s_color, a_step * internalTimer->get_c_runfor_time());
            else
                a_color = Color::Translate(s_color, e_color, a_step * internalTimer->get_c_runfor_time());
            _cb(a_color);
        }
    }
}