cmake_minimum_required(VERSION 3.1)
project(MatrixScript)

find_package(Java REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)

set(CMAKE_CXX_STANDARD 11)
set(java_source_dir Matrix\ Script\ Java)
file(GLOB source_list Matrix\ Script\/*.cpp)
add_library(Calc SHARED ${source_list} ${java_source_dir}/MatrixScript.cpp)
target_include_directories(Calc PRIVATE ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2} Matrix\ Script)

add_jar(MatrixScript ${java_source_dir}/MatrixScript.java ${java_source_dir}/MainFrame.java ENTRY_POINT MatrixScript)
