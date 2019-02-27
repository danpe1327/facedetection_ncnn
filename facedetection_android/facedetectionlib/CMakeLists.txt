# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.
cmake_minimum_required(VERSION 3.4.1)

set(ROOT src/main/cpp)

# 定义变量
aux_source_directory(${ROOT} DIRSRCS)
aux_source_directory(${ROOT}/facedetection DIRSRCS)

#添加头文件路径
include_directories(${ROOT}
                    ${ROOT}/common
                    ${ROOT}/include
                    ${ROOT}/include/opencv
                    ${ROOT}/include/opencv2
                    ${ROOT}/facedetection
                    )

#链接库路径
link_directories(${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI})

#添加ncnn库
set(ncnn_lib ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libncnn.a)
add_library(ncnn_lib STATIC IMPORTED)
set_target_properties(ncnn_lib PROPERTIES IMPORTED_LOCATION ${ncnn_lib})

# 生成库名 共享库 包含源文件
add_library(facedetection SHARED ${DIRSRCS})

#导入库：库别名 原库名
find_library(log-lib log )

#链接库：库 链接库
target_link_libraries(
    facedetection
    ${log-lib}
    opencv_java3
    ncnn_lib
 )