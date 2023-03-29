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

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(BIT "64")
else()
    set(BIT "32")
endif()

set(BASE_OBJ "${CMAKE_CURRENT_BINARY_DIR}/base${BIT}.o")
message("BASE_OBJ=" ${BASE_OBJ})

add_custom_command(OUTPUT ${BASE_OBJ}
        COMMAND ${CMAKE_CXX_COMPILER} -c -o ${BASE_OBJ} ${CMAKE_CURRENT_SOURCE_DIR}/mcl/src/base${BIT}.ll -target ${CMAKE_ANDROID_ARCH} -O3 -fPIC
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(gen_base.o
        SOURCES ${BASE_OBJ})
add_dependencies(hello-libs gen_base.o)
target_sources(hello-libs PRIVATE ${BASE_OBJ})

set(BINT_OBJ "${CMAKE_CURRENT_BINARY_DIR}/bint${BIT}.o")
message("bint_obj=" ${BINT_OBJ})

add_custom_command(OUTPUT ${BINT_OBJ}
        COMMAND ${CMAKE_CXX_COMPILER} -c -o ${BINT_OBJ} ${CMAKE_CURRENT_SOURCE_DIR}/mcl/src/bint${BIT}.ll -target ${CMAKE_ANDROID_ARCH} -O3 -fPIC
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(gen_bint.o
        SOURCES ${BINT_OBJ})
add_dependencies(hello-libs gen_bint.o)
target_sources(hello-libs PRIVATE ${BINT_OBJ})


target_compile_options(hello-libs PRIVATE -fPIC) #-fno-exceptions -fno-rtti -fno-threadsafe-statics -fPIC)
target_compile_definitions(hello-libs PRIVATE NDEBUG)
target_compile_definitions(hello-libs PRIVATE MCL_DONT_USE_OPENSSL)
#target_compile_definitions(hello-libs PRIVATE CYBOZU_DONT_USE_EXCEPTION)
#target_compile_definitions(hello-libs PRIVATE CYBOZU_DONT_USE_STRING)
#target_compile_definitions(hello-libs PRIVATE MCL_LLVM_BMI2=0)
target_compile_definitions(hello-libs PRIVATE MCL_USE_VINT MCL_VINT_FIXED_BUFFER)
target_compile_definitions(hello-libs PRIVATE MCL_MAX_BIT_SIZE=384)
target_compile_definitions(hello-libs PRIVATE MCL_USE_LLVM=0)
target_compile_definitions(hello-libs PRIVATE MCL_BINT_ASM=0)

target_link_libraries(hello-libs PUBLIC android)
