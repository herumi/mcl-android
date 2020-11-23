cmake_minimum_required(VERSION 3.4.1)

# build application's shared lib
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#  -DMCL_USE_LLVM=1

add_library(hello-libs SHARED
            mcl/src/fp.cpp
            mcl/src/bn_c384_256.cpp
            mcl/ffi/java/mcl_wrap.cxx
            hello-libs.cpp)

target_include_directories(hello-libs PRIVATE mcl/src mcl/include)
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
#    target_compile_definitions(hello-libs PRIVATE MCL_USE_LLVM=1)
    target_sources(hello-libs PRIVATE base64.ll)
endif()

add_custom_command(OUTPUT base64.o
        COMMAND ${CMAKE_CXX_COMPILER} -c -o ${CMAKE_CURRENT_BINARY_DIR}/base64.o ${CMAKE_CURRENT_SOURCE_DIR}/base64.ll -O3
        DEPENDS base64.ll
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

target_compile_options(hello-libs PRIVATE -fPIC) #-fno-exceptions -fno-rtti -fno-threadsafe-statics -fPIC)
target_compile_definitions(hello-libs PRIVATE NDEBUG)
target_compile_definitions(hello-libs PRIVATE MCL_DONT_USE_OPENSSL)
#target_compile_definitions(hello-libs PRIVATE CYBOZU_DONT_USE_EXCEPTION)
#target_compile_definitions(hello-libs PRIVATE CYBOZU_DONT_USE_STRING)
target_compile_definitions(hello-libs PRIVATE MCL_LLVM_BMI2=0)
target_compile_definitions(hello-libs PRIVATE MCL_USE_VINT MCL_VINT_FIXED_BUFFER)
target_compile_definitions(hello-libs PRIVATE MCL_MAX_BIT_SIZE=384)

target_link_libraries(hello-libs android)

