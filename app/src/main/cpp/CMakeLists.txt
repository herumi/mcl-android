cmake_minimum_required(VERSION 3.4.1)

add_library(hello-libs SHARED
            test.cpp
            hello-libs.cpp)

target_include_directories(hello-libs PRIVATE mcl/src mcl/include)

#target_compile_definitions(hello-libs PRIVATE __ANDROID__=1)

add_library(mclbn STATIC IMPORTED)
set_target_properties(mclbn PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/mcl/lib/android/${ANDROID_ABI}/libmclbn384_256.a)
target_link_libraries(hello-libs PUBLIC mclbn android)

#find_library(mclbn mcl/android/jni)
#target_link_libraries(hello-libs PUBLIC ${mclbn} android)
