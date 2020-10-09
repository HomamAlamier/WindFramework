#include <Core/Drawing/Animation/easeAnimation.h>
#include <Core/Drawing/Animation/Functions.h>
namespace drawing {
    namespace animation {
        void easing_tick(void* ptr){
            ((EasingAnimation*)ptr)->Change();
        }
        EasingAnimation::EasingAnimation(int duration, float start, float end, EasingFunction func) : AnimationBase(duration){
            _func = func;
            _start = start;
            _end = end;
            _dur = duration;
            internalTimer->set_tick(easing_tick, this);
            internalTimer->set_runfor_interval(duration);
        }
        void EasingAnimation::set_Callback(std::function<void(float)> cb){
            _cb = cb;
        }
        void EasingAnimation::set_duration(int value){
            internalTimer->set_runfor_interval(value);
            _dur = value;
        }
        void EasingAnimation::set_start(float x){
            _start = x;
        }
        void EasingAnimation::set_end(float x){
            _end = x;
        }
        void EasingAnimation::Start(){
            rev = false;
            internalTimer->start();
        }
        void EasingAnimation::Reverse(){
            rev = true;
            internalTimer->start();
        }
        void EasingAnimation::set_func(EasingFunction func){
            _func = func;
        }
        void EasingAnimation::Change(){
            float fx = 0;
            switch (_func)
            {
                case EasingFunction::easeInOutSine: fx = easeInOutSine(internalTimer->get_c_runfor_time() / (float)_dur); break;
                case EasingFunction::easeInOutQuart: fx = easeInOutQuart(internalTimer->get_c_runfor_time() / (float)_dur); break;
                case EasingFunction::easeInOutBack: fx = easeInOutBack(internalTimer->get_c_runfor_time() / (float)_dur); break;
            }
            if (rev && _cb != NULL) 
                    _cb(_start + (_end * (1.0f - fx)));
            else if (_cb != NULL) 
                    _cb(_start + (_end * fx));
        }
    }
}