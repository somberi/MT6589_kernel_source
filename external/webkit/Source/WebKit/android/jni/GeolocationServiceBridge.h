/*
 * Copyright 2012, The Android Open Source Project
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef GeolocationServiceBridge_h
#define GeolocationServiceBridge_h

#include <JNIUtility.h>
#include <wtf/PassRefPtr.h>

namespace WebCore {
class GeolocationError;
class GeolocationPosition;
}

namespace android {

class WebViewCore;

// GeolocationServiceBridge is the bridge to the Java implementation. It manages
// the lifetime of the Java object. It is an implementation detail of
// GeolocationClientAndroid.
class GeolocationServiceBridge {
public:
    class Listener {
    public:
        virtual ~Listener() {}
        virtual void newPositionAvailable(PassRefPtr<WebCore::GeolocationPosition>) = 0;
        virtual void newErrorAvailable(PassRefPtr<WebCore::GeolocationError>) = 0;
    };

    GeolocationServiceBridge(Listener*, WebViewCore*);
    ~GeolocationServiceBridge();

    bool start();
    void stop();
    void setEnableGps(bool enable);

    // Static wrapper functions to hide JNI nastiness.
    static void newLocationAvailable(JNIEnv *env, jclass, jlong nativeObject, jobject location);
    static void newErrorAvailable(JNIEnv *env, jclass, jlong nativeObject, jstring message);
    static PassRefPtr<WebCore::GeolocationPosition> toGeolocationPosition(JNIEnv *env, const jobject &location);

private:
    void startJavaImplementation(WebViewCore*);
    void stopJavaImplementation();

    Listener* m_listener;
    jobject m_javaGeolocationServiceObject;
};

} // namespace android

#endif // GeolocationServiceBridge_h