#pragma once

//
// Created by GHr_Ryuuka on 05/12/2021.
//

#include <android/input.h>
#include <map>
#include <queue>

#define HOOK(ret, func, ...) \
    ret (*orig##func)(__VA_ARGS__); \
    ret my##func(__VA_ARGS__)
	
int32_t (*orig_ANativeWindow_getWidth)(ANativeWindow* window);
int32_t _ANativeWindow_getWidth(ANativeWindow* window) {
	screenWidth = orig_ANativeWindow_getWidth(window);
	return orig_ANativeWindow_getWidth(window);
}

int32_t (*orig_ANativeWindow_getHeight)(ANativeWindow* window);
int32_t _ANativeWindow_getHeight(ANativeWindow* window) {
	screenHeight = orig_ANativeWindow_getHeight(window);
	return orig_ANativeWindow_getHeight(window);
}

int32_t (*orig_AConfiguration_getDensity)(ANativeWindow* config);
int32_t _AConfiguration_getDensity(ANativeWindow* config) {
	density = orig_AConfiguration_getDensity(config);
	return orig_AConfiguration_getDensity(config);
}

HOOK(void, Input, void *thiz, void *ex_ab, void *ex_ac) { 
    origInput(thiz, ex_ab, ex_ac);
	ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz, {(float) screenWidth / (float) glWidth, (float) screenHeight / (float) glHeight});
    return;
}

jboolean (*orig_unity__nativeInjectEvent)(JNIEnv *env, jobject object, jobject inputEvent);
jboolean unity_nativeInjectEvent(JNIEnv *env, jobject object, jobject inputEvent) {
	ImGuiIO &io = ImGui::GetIO();
	
	jclass motionEventClass = env->FindClass(OBFUSCATE("android/view/MotionEvent"));
	
	if (env->IsInstanceOf(inputEvent, motionEventClass)) {
		jmethodID getActionMethod = env->GetMethodID(motionEventClass, OBFUSCATE("getAction"), OBFUSCATE("()I"));
		jint getAction = env->CallIntMethod(inputEvent, getActionMethod);
		
		jmethodID getXMethod = env->GetMethodID(motionEventClass, OBFUSCATE("getX"), OBFUSCATE("()F"));
		jfloat getX = env->CallFloatMethod(inputEvent, getXMethod);
		
		jmethodID getYMethod = env->GetMethodID(motionEventClass, OBFUSCATE("getY"), OBFUSCATE("()F"));
		jfloat getY = env->CallFloatMethod(inputEvent, getYMethod);
		
		jmethodID getPointerCountMethod = env->GetMethodID(motionEventClass, OBFUSCATE("getPointerCount"), OBFUSCATE("()I"));
		jint getPointerCount = env->CallIntMethod(inputEvent, getPointerCountMethod);
		
		switch(getAction) {
			
		case 0:
			io.MouseDown[0] = true;
			break;
		case 1:
			io.MouseDown[0] = false;
			break;
		case 2:
			if (getPointerCount > 1) {
				io.MouseDown[0] = false;
			} else {
				io.MouseWheel = 0;
            }
			break;
		}
		io.MousePos = ImVec2(getX, getY);
	}
	
	jclass KeyEventClass = env->FindClass(OBFUSCATE("android/view/KeyEvent"));
	if (env->IsInstanceOf(inputEvent, KeyEventClass)) {
		jmethodID getActionMethod = env->GetMethodID(KeyEventClass, OBFUSCATE("getAction"), OBFUSCATE("()I"));
		if (env->CallIntMethod(inputEvent, getActionMethod) == 0) {
			jmethodID getKeyCodeMethod = env->GetMethodID(KeyEventClass, OBFUSCATE("getKeyCode"), OBFUSCATE("()I"));
			jmethodID getUnicodeCharMethod = env->GetMethodID(KeyEventClass, OBFUSCATE("getUnicodeChar"), OBFUSCATE("(I)I"));
			jmethodID getMetaStateMethod = env->GetMethodID(KeyEventClass, OBFUSCATE("getMetaState"), OBFUSCATE("()I"));
			
			io.AddInputCharacter(env->CallIntMethod(inputEvent, getUnicodeCharMethod, env->CallIntMethod(inputEvent, getMetaStateMethod)));
		}
	}
	return orig_unity__nativeInjectEvent(env, object, inputEvent);
}

jint (*old_RegisterNatives)(JNIEnv*, jclass, const JNINativeMethod*, jint);
jint hook_RegisterNatives(JNIEnv* env, jclass klazz, const JNINativeMethod* methods, jint methodcount) {
	for (int i = 0; i < methodcount; ++i) {
		auto method = methods[i];
		
		if (strcmp(method.name, OBFUSCATE("nativeInjectEvent")) == 0) {
            Tools::Hook((void *) method.fnPtr, (void *) unity_nativeInjectEvent, (void **) &orig_unity__nativeInjectEvent);
		}
	}
	return old_RegisterNatives(env, klazz, methods, methodcount);
}
