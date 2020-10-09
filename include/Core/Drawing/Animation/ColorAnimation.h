#ifndef COLOR_ANIMATION_H
#define COLOR_ANIMATION_H

#include <Core/Drawing/Animation/AnimationBase.h>
#include <Core/Drawing/Color.h>
namespace drawing {
    namespace animation {
        
        class ColorAnimation : public AnimationBase {
            std::function<void(Color)> _cb;
            Color a_color, s_color, e_color;
            float a_step = 0.0f;
            bool rev = false;
        public:
            ColorAnimation(int duration, Color startColor, Color endColor);
            void set_StartColor(Color color);
            void set_EndColor(Color color);
            void Start();
            void Reverse();
            void set_ChangeCallback(std::function<void(Color)> callback);
            void Change();
        };
    }
}
#endif // !COLOR_ANIMATION_H