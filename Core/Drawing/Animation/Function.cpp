#include <Core/Drawing/Animation/Functions.h>
#define PI 3.14159265358979323846
float easeInOutSine(float x) {
    return -(cos(PI * x) - 1) / 2;
}
float easeInOutBack(float x) {
    float c1 = 1.70158f;
    float c2 = c1 * 1.525f;
    return x < 0.5f
    ? (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
    : (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}
float easeInOutQuart(float x) {
    return x < 0.5f ? 8 * x * x * x * x : 1.0f - pow(-2 * x + 2, 4) / 2.0f;
}