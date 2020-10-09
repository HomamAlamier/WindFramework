#ifndef ANIMATION_BASE_H
#define ANIMATION_BASE_H

#include <GL.h>
#include <functional>
#include <Core/Controls/w_Timer.h>
namespace drawing {
    namespace animation {
        class AnimationBase{
        protected:
            controls::w_timer* internalTimer;
            int duration = 0;
            bool attached = false;
        public:
            AnimationBase(int duration);
            virtual ~AnimationBase(){ }
            virtual void Start();
            virtual void Stop();
            virtual void Change();
            virtual controls::w_timer* InternalTimer() { return internalTimer; }
            virtual bool Running() { return internalTimer->running(); }
        };
    }
}

#endif // !ANIMATION_BASE_H