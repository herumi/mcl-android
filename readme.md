# mcl-android

This is a sample of how to use mcl on Android

## How to use

```
git clone --recursive https://github.com/herumi/mcl-android
cd mcl-android/app/src/main/cpp/mcl
make android # make static libraries in mcl/lib/android/
```
Open mcl-android with Android Studio and build it.
You can use C/C++ APIs.

### header

```C
#include <mcl/bn_c384_256.h>
```

```cpp
#define MCL_DONT_USE_XBYAK
#define MCL_MAX_BIT_SIZE 384
#define MCL_MAX_FP_BIT_SIZE 384
#define MCL_MAX_FR_BIT_SIZE 256
#include <mcl/bn.hpp>
```

### initialization

```C
int ret = mclBn_init(MCL_BLS12_381, MCLBN_COMPILED_TIME_VAR);
if (ret != 0) {
  throw std::runtime_error("mcl init err");
}
```

```cpp
mcl::bn::initPairing(mcl::BLS12_381);
```

See [hello-libs.cpp](https://github.com/herumi/mcl-android/blob/main/app/src/main/cpp/hello-libs.cpp).
