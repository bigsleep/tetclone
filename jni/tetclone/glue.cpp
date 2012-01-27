#include <jni.h>
#include <GLES/gl.h>
#include <utility>
#include <android/log.h>

#include "tetclone.h"
#include "view.h"

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)

#define  LOG_TAG    "mgame.tetclone"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern "C" {
    JNIEXPORT void Java_mgame_tetclone_Glue_onNdkSurfaceCreated(JNIEnv* env, jobject obj);
    JNIEXPORT void Java_mgame_tetclone_Glue_onNdkSurfaceChanged(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void Java_mgame_tetclone_Glue_onNdkDrawFrame(JNIEnv* env, jobject obj);
    JNIEXPORT void Java_mgame_tetclone_Glue_step(JNIEnv* env, jobject obj);
    JNIEXPORT void Java_mgame_tetclone_Glue_LoadFont(JNIEnv *env, jobject, jbyteArray fontdata);
    JNIEXPORT void Java_mgame_tetclone_Glue_onRotate(JNIEnv* env, jobject obj);
    JNIEXPORT void Java_mgame_tetclone_Glue_onLeft(JNIEnv* env, jobject obj);
    JNIEXPORT void Java_mgame_tetclone_Glue_onRight(JNIEnv* env, jobject obj);
    JNIEXPORT void Java_mgame_tetclone_Glue_onDown(JNIEnv* env, jobject obj);
    JNIEXPORT void Java_mgame_tetclone_Glue_Restart(JNIEnv* env, jobject obj);


void Java_mgame_tetclone_Glue_onNdkSurfaceCreated (JNIEnv* env, jobject obj)
{
    //LOGI("onNdkSurfaceCreated");
    mgame::tetclone_view.bind_font_texture();
}

void Java_mgame_tetclone_Glue_onNdkSurfaceChanged (JNIEnv* env, jobject obj, jint width, jint height)
{
    //LOGI("onNdkSurfaceChanged width: %d, height %d", width, height);
    mgame::tetclone_view.set_frame_size(width, height);
}
 

void Java_mgame_tetclone_Glue_onNdkDrawFrame(JNIEnv* env, jobject obj)
{
    //LOGI("onNdkDrawFrame");
    mgame::tetclone_view.draw();
}

void Java_mgame_tetclone_Glue_step(JNIEnv* env, jobject obj)
{
    //LOGI("step");
    mgame::tetclone_model.step();
}

void Java_mgame_tetclone_Glue_LoadFont(
    JNIEnv *env, jobject, jbyteArray fontdata)
{
    //LOGI("LoadFont");
    jbyte* arr = env->GetByteArrayElements(fontdata, NULL);
    jint size = env->GetArrayLength(fontdata);
    std::vector<char> storage(size);
    std::copy(arr, arr + size, storage.begin());
    mgame::tetclone_view.set_font(storage);
    env->ReleaseByteArrayElements(fontdata, arr, 0);
}

void Java_mgame_tetclone_Glue_onRotate(JNIEnv* env, jobject obj)
{
    mgame::tetclone_model.on_rotate();
}

void Java_mgame_tetclone_Glue_onLeft(JNIEnv* env, jobject obj)
{
    mgame::tetclone_model.on_left();
}

void Java_mgame_tetclone_Glue_onRight(JNIEnv* env, jobject obj)
{
    mgame::tetclone_model.on_right();
}

void Java_mgame_tetclone_Glue_onDown(JNIEnv* env, jobject obj)
{
    mgame::tetclone_model.on_down();
}

void Java_mgame_tetclone_Glue_Restart(JNIEnv* env, jobject obj)
{
    mgame::tetclone_model.restart();
}

}

