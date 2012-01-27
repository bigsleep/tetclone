LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := tetclone
LOCAL_SRC_FILES := glue.cpp tetclone.cpp view.cpp bitmap_font.cpp

LOCAL_LDLIBS := -lGLESv1_CM -llog
LOCAL_STATIC_LIBRARIES := libfreetype

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../freetype/include

include $(BUILD_SHARED_LIBRARY)

