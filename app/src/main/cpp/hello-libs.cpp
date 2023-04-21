/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <cstring>
#include <jni.h>
#include <cinttypes>
#include <string>
#include "mcl_common.hpp"

// not necessary if a library is built with _FORTIFY_SOURCE = 0
#if 0 // _FORTIFY_SOURCE == 2
extern "C" void *__memset_chk (void *dest, int val, size_t len, size_t dstlen)
{
    return memset(dest, val, len);
}
extern "C" void * __memcpy_chk (void *dest, const void *src, size_t len, size_t dstlen)
{
    return memcpy (dest, src, len);
}
#endif

static struct Init {
    Init() {
#ifdef MCL_C_API
        int ret = mclBn_init(MCL_BLS12_381, MCLBN_COMPILED_TIME_VAR);
        if (ret != 0) {
          throw std::runtime_error("mcl init err");
        }
#else
        mcl::bn::initPairing(mcl::BLS12_381);
#endif
    }
} g_init;

#ifndef MCL_C_PI
std::string test_func(int a, int b); // defined in test.cpp
#endif

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   app/src/main/java/com/example/hellolibs/MainActivity.java
 */
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_hellolibs_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
#ifdef MCL_C_API
    mclBnFr x, y;
    mclBnFr_setInt(&x, 123);
    mclBnFr_setInt(&y, 200);
    mclBnFr_add(&x, &x, &y);
    char buf[256];
    mclBnFr_getStr(buf, sizeof(buf), &x, 10);
    return env->NewStringUTF(buf);
#else
    using namespace mcl::bn;
    std::string s;
    Fr x, y, z;
    x = 50;
    y = 11;
    z = x * y;
    s += "z=" + z.getStr(10) + "\n";
    G1 P;
    G2 Q;
    hashAndMapToG1(P, "abc", 3);
    s += "P=" + P.getStr(16) + "\n";
    hashAndMapToG2(Q, "abc", 3);
    GT e1;
    pairing(e1, P, Q);
    s += "e=" + e1.getStr(16) + "\n";
    GT::pow(e1, e1, z);
    P *= x;
    Q *= y;
    GT e2;
    pairing(e2, P, Q);
    s += e1 == e2 ? "ok" : "err";
#ifdef __ANDROID__
    s += "\nandroid";
#else
    s += "\nnot defined";
#endif
    s += "\n";
    s += test_func(3, 128);

    return env->NewStringUTF(s.c_str());
#endif
}
