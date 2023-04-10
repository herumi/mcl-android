cmake_minimum_required(VERSION 3.4.1)

add_library(hello-libs SHARED
            hello-libs.cpp)

target_include_directories(hello-libs PRIVATE mcl/src mcl/include)


#target_compile_options(hello-libs PRIVATE -fPIC)
#target_compile_definitions(hello-libs PRIVATE _FORTIFY_SOURCE=0)

message("ANDROID_ABI=" ${ANDROID_ABI})
#link_directories(mcl/lib/${ANDROID_ABI}/)
add_library(mclbn STATIC IMPORTED)
set_target_properties(mclbn PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/mcl/lib/${ANDROID_ABI}/libmclbn384_256.a)

target_link_libraries(hello-libs PUBLIC mclbn android)
