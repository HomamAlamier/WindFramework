#include <Core/Drawing/Animation/AnimationBase.h>
namespace drawing {
    namespace animation { 
        AnimationBase::AnimationBase(int duration){
            internalTimer = new controls::w_timer(1);
        }          
        void AnimationBase::Start(){
            internalTimer->start();
        }       
        void AnimationBase::Stop(){
            internalTimer->stop();
        }  
        void AnimationBase::Change() {}
    }
}