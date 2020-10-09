#include <jni.h>
#include <Core/OSWindow/OSWindow.h>
#include <Core/Controls/Button.h>
#include <Core/Controls/TextInput.h>
#include <Core/Drawing/Rectangle.h>
#include <Core/Drawing/Animation/easeAnimation.h>
#include <Core/Controls/Menu.h>
#define LOG_FN "libcore"
#include <Core/Services/LogService.h>
#include <Core/Drawing/Shaders.h>
windowsys::oswindow* ptr;
controls::Button* b;
controls::Button* b2;
controls::Menu* m;
controls::Button* b3;
controls::TextInput* ti;
drawing::Font* fnt;
drawing::Font* tb_fnt;
drawing::animation::EasingAnimation* ea;
jobject class_h;
jmethodID show_im_id;
JNIEnv* helper_env;
float dpx = 1, dpy = 1;
float font_scale = 1.0f;
bool inited = false;
std::string jstring2string(JNIEnv *env, jstring jStr) {
    if (!jStr)
        return "";

    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}
int show_im(){
    auto cb = helper_env->GetMethodID(helper_env->GetObjectClass(class_h), "SHOW_IM", "()I");
    jint result = (jint)helper_env->CallIntMethod(class_h, cb);
    return result;
}

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_gles3jni_LibCore_init(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_android_gles3jni_LibCore_resize(JNIEnv* env, jobject obj, jint w, jint h);
    JNIEXPORT void JNICALL Java_com_android_gles3jni_LibCore_draw(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_android_gles3jni_LibCore_onTouchEvent(JNIEnv* env, jobject obj, jfloat x, jfloat y, jint action);
    JNIEXPORT void JNICALL Java_com_android_gles3jni_LibCore_setFontScale(JNIEnv* env, jobject obj, jfloat scale);
    JNIEXPORT void JNICALL Java_com_android_gles3jni_LibCore_onKeyEvent(JNIEnv* env, jobject obj, jint key, jint action, jint mods);
    JNIEXPORT void JNICALL Java_com_android_gles3jni_LibCore_onCharEvent(JNIEnv* env, jobject obj, jint ch);
    JNIEXPORT void JNICALL Java_com_android_gles3jni_LibCore_setHelperClass(JNIEnv* env, jobject obj, jobject refc);
    JNIEXPORT void JNICALL Java_com_android_gles3jni_LibCore_setupTitleBar(JNIEnv* env, jobject obj, jfloat r, jfloat g, jfloat b, jstring title);
    JNIEXPORT void JNICALL Java_com_android_gles3jni_LibCore_setDPIValues(JNIEnv* env, jobject obj, jfloat x, jfloat y);
}
JNIEXPORT void JNICALL
Java_com_android_gles3jni_LibCore_setFontScale(JNIEnv* env, jobject obj, jfloat scale){
    font_scale = scale;
    if (ptr == NULL)
        return;
    fnt = drawing::Font::GenerateFont("", 18 * font_scale);
    m->set_Font(fnt);
    ti->set_Font(fnt);
    b->set_Font(fnt);
    b2->set_Font(fnt);
    b3->set_Font(fnt);
    ptr->RegenAll();
}
JNIEXPORT void JNICALL
Java_com_android_gles3jni_LibCore_setDPIValues(JNIEnv* env, jobject obj, jfloat x, jfloat y){
    dpx = x;
    dpy = y;
    float dpi = x / y;
    Log_WriteLine("x / y = " + std::to_string(dpi), LogType::Debug, true);
}
JNIEXPORT void JNICALL
Java_com_android_gles3jni_LibCore_init(JNIEnv* env, jobject obj){
    if (inited)
    {
        shaders::Force_InitAll();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        fnt = drawing::Font::GenerateFont("", 18 * font_scale);
        tb_fnt = drawing::Font::GenerateFont("", 21 * font_scale);
        m->set_Font(fnt);
        ti->set_Font(fnt);
        b->set_Font(fnt);
        b2->set_Font(fnt);
        b3->set_Font(fnt);
        ptr->RegenAll();
        return;
    }

    inited = true;
    windowsys::initWindowSystem();
    ptr = new windowsys::oswindow(0, 0, "Hi");
    ptr->set_AppScale(font_scale);
    fnt = drawing::Font::GenerateFont("", 18 * font_scale);
    tb_fnt = drawing::Font::GenerateFont("", 21 * font_scale);
    b = new controls::Button(drawing::Rectangle(10, 110, 1.5f * dpx, 0.5f * dpy), ptr);
    b2 = new controls::Button(drawing::Rectangle(10, 110 + (0.5f * dpy), 1.5f * dpx, 0.5f * dpy), ptr);
    b3 = new controls::Button(drawing::Rectangle(10, 110 + (1.0f * dpy), 1.5f * dpx, 0.5f * dpy), ptr);
    ti = new controls::TextInput(drawing::Rectangle(10, 110 + (1.8f * dpy), 2.5f * dpx, 1.5f * dpy), ptr);
    ea = new drawing::animation::EasingAnimation(500, 0 , 300);
    m = new controls::Menu(drawing::Rectangle(0, 0, 100, 100), ptr);
    m->set_Font(fnt);
    m->SetBackgroundColor(drawing::Color::White());
    m->set_ForegroundColor(drawing::Color::Black());
    m->AddItem(U"Open", NULL);
    m->AddItem(U"Delete", NULL);
    m->AddItem(U"FileExplorer", NULL);
    m->AddItem(U"Exit", NULL);
    m->AddItem(U"Minimize", NULL);
    ea->set_Callback([&](float x){
        ti->set_Position(x - 300, ti->Rectangle().y);
    });
    ptr->addTimer(ea->InternalTimer());
    ti->set_Font(fnt);
    ti->set_ForegroundColor(drawing::Color::Red());
    b->set_Font(fnt);
    b->set_Text(U"Menu Test");
    b->set_ForegroundColor(drawing::Color::White());
    b2->set_Font(fnt);
    b2->set_Text(U"easeInOutQuart");
    b2->set_ForegroundColor(drawing::Color::White());
    b3->set_Font(fnt);
    b3->set_Text(U"easeInOutBack");
    b3->set_ForegroundColor(drawing::Color::White());

    b->set_MouseDown([&](windowsys::MouseArgs e){
        m->Show(e.x(), e.y());
    });
    b2->set_Click([&]{
        ti->set_Position(-300, ti->Rectangle().y);
        ea->set_func(drawing::animation::EasingFunction::easeInOutQuart);
        ea->Start();
    });
    b3->set_Click([&]{
        ti->set_Position(-300, ti->Rectangle().y);
        ea->set_func(drawing::animation::EasingFunction::easeInOutBack);
        ea->Start();
    });


    ptr->addControl(b);
    ptr->addControl(b2);
    ptr->addControl(b3);
    ptr->addControl(ti);
    ptr->addControl(m);
    windowsys::oswindow_timerhandler();

}
JNIEXPORT void JNICALL
Java_com_android_gles3jni_LibCore_resize(JNIEnv* env, jobject obj, jint w, jint h){
    if (ptr != NULL) ptr->resize(w, h);
}
JNIEXPORT void JNICALL
Java_com_android_gles3jni_LibCore_draw(JNIEnv* env, jobject obj) {
    if (ptr != NULL) ptr->paintAll();
}
JNIEXPORT void JNICALL
Java_com_android_gles3jni_LibCore_onTouchEvent(JNIEnv* env, jobject obj, jfloat x, jfloat y, jint action){
    windowsys::oswindow_ontouchEvent(x, y, action);
}
JNIEXPORT void JNICALL
Java_com_android_gles3jni_LibCore_setupTitleBar(JNIEnv* env, jobject obj, jfloat r, jfloat g, jfloat b, jstring title){
    std::u32string str;
    auto ss = jstring2string(env, title);
    for(auto ch : ss){
        str += (char32_t)ch;
    }
    if (ptr != NULL) ptr->setupAppTitleBar(drawing::Color(r, g, b), str, tb_fnt, font_scale);
}
JNIEXPORT void JNICALL
Java_com_android_gles3jni_LibCore_onKeyEvent(JNIEnv* env, jobject obj, jint key, jint action, jint mods){
    windowsys::oswindow_onkeyEvent(key, action, mods);
}
JNIEXPORT void JNICALL
Java_com_android_gles3jni_LibCore_setHelperClass(JNIEnv* env, jobject obj, jobject refc){
    class_h = env->NewGlobalRef(refc);
    helper_env = env;
    windowsys::set_android_show_im(show_im);
}
JNIEXPORT void JNICALL
Java_com_android_gles3jni_LibCore_onCharEvent(JNIEnv* env, jobject obj, jint ch){
    windowsys::oswindow_oncharkeyEvent((char32_t)ch);
}