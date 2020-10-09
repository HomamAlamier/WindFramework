#ifndef EASE_ANIMATION_H
#define EASE_ANIMATION_H

#include <Core/Drawing/Animation/AnimationBase.h>

namespace drawing {
    namespace animation {
        enum class EasingFunction {
            easeInOutSine, easeInOutBack, easeInOutQuart
        };
        class EasingAnimation : public AnimationBase {
            EasingFunction _func;
            std::function<void(float)> _cb;
            float _start = 0, _end = 0;
            int _dur = 0;
            bool rev = false;
        public:
            EasingAnimation(int duration, float start, float end, EasingFunction func = EasingFunction::easeInOutSine);
            void set_Callback(std::function<void(float)> cb);
            void set_start(float x);
            void set_end(float x);
            void set_func(EasingFunction func);
            void set_duration(int value);
            void Start();
            void Reverse();
            void Change();
        };
    }
}


#endif // !EASE_ANIMATION_H
