#ifndef JNISTUFF
#define JNISTUFF


jclass UnityPlayer_cls;
jfieldID UnityPlayer_CurrentActivity_fid;
JavaVM* jvm = nullptr;

JNIEnv* getEnv() {
    JNIEnv *env;
    int status = jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (status < 0) {
        status = jvm->AttachCurrentThread(&env, NULL);
        if (status < 0) {
            return nullptr;
        }
    }
    return env;
}

jobject getGlobalContext(JNIEnv *env) {
    jclass activityThread = env->FindClass("android/app/ActivityThread");
    jmethodID currentActivityThread = env->GetStaticMethodID(activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
    jobject at = env->CallStaticObjectMethod(activityThread, currentActivityThread);
    jmethodID getApplication = env->GetMethodID(activityThread, "getApplication", "()Landroid/app/Application;");
    jobject context = env->CallObjectMethod(at, getApplication);
    return context;
}

bool isChromeInstalled() {
    JNIEnv* env = getEnv();
    if (!env) return false;

    jobject context = getGlobalContext(env);
    if (!context) return false;

    jclass packageManagerClass = env->FindClass("android/content/pm/PackageManager");
    jclass contextClass = env->GetObjectClass(context);

    jmethodID getPackageManagerMethod = env->GetMethodID(contextClass, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jobject packageManager = env->CallObjectMethod(context, getPackageManagerMethod);

    jstring chromePackage = env->NewStringUTF("com.android.chrome");
    jmethodID getLaunchIntent = env->GetMethodID(packageManagerClass, "getLaunchIntentForPackage", "(Ljava/lang/String;)Landroid/content/Intent;");
    jobject launchIntent = env->CallObjectMethod(packageManager, getLaunchIntent, chromePackage);

    env->DeleteLocalRef(chromePackage);
    env->DeleteLocalRef(packageManager);
    env->DeleteLocalRef(context);

    return launchIntent != nullptr;
}

void openBrowserWithIntent(const char* url) {
    JNIEnv* env = getEnv();
    if (!env) {
        __android_log_print(ANDROID_LOG_ERROR, "JNIDebug", "Failed to get JNIEnv");
        return;
    }

    if (!isChromeInstalled()) {
        __android_log_print(ANDROID_LOG_WARN, "JNIDebug", "Google Chrome is not installed.");
        return;
    }

    jobject context = getGlobalContext(env);
    if (!context) {
        __android_log_print(ANDROID_LOG_ERROR, "JNIDebug", "Failed to get context");
        return;
    }

    jclass uriClass = env->FindClass("android/net/Uri");
    jmethodID parseMethod = env->GetStaticMethodID(uriClass, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
    jstring urlString = env->NewStringUTF(url);
    jobject uri = env->CallStaticObjectMethod(uriClass, parseMethod, urlString);

    jclass intentClass = env->FindClass("android/content/Intent");
    jmethodID intentConstructor = env->GetMethodID(intentClass, "<init>", "(Ljava/lang/String;Landroid/net/Uri;)V");
    jstring actionView = env->NewStringUTF("android.intent.action.VIEW");
    jobject intent = env->NewObject(intentClass, intentConstructor, actionView, uri);

    jmethodID setPackageMethod = env->GetMethodID(intentClass, "setPackage", "(Ljava/lang/String;)Landroid/content/Intent;");
    jstring chromePackage = env->NewStringUTF("com.android.chrome");
    env->CallObjectMethod(intent, setPackageMethod, chromePackage);

    jclass activityClass = env->GetObjectClass(context);
    jmethodID startActivityMethod = env->GetMethodID(activityClass, "startActivity", "(Landroid/content/Intent;)V");
    env->CallVoidMethod(context, startActivityMethod, intent);

    env->DeleteLocalRef(urlString);
    env->DeleteLocalRef(uri);
    env->DeleteLocalRef(intent);
    env->DeleteLocalRef(actionView);
    env->DeleteLocalRef(chromePackage);
    env->DeleteLocalRef(context);
}

#endif // JNISTUFF

