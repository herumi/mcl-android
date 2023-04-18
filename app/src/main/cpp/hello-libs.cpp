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
//#include <mcl/bn_c384_256.h>
//#define MCL_C_API
#ifdef MCL_C_API
#include <mcl/bn_c384_256.h>
#else
#define MCL_DONT_USE_XBYAK
#define MCL_MAX_BIT_SIZE 384
#define MCL_MAX_FP_BIT_SIZE 384
#define MCL_MAX_FR_BIT_SIZE 256
#include <mcl/bn.hpp>
#endif
#include <exception>

#if _FORTIFY_SOURCE == 2
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
        try {
            mcl::bn::initPairing(mcl::BLS12_381);
        } catch (std::exception& e) {
            printf("err %s\n", e.what());
            throw std::runtime_error(e.what());
        }
#endif
    }
} g_init;

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
    y = 12;
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

    return env->NewStringUTF(s.c_str());
#endif
}
