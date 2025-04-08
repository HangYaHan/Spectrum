# 指定 CMake 的最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目的名称
project(MyOpenCVProject CXX)

# 设置 C++ 标准为 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 找到 OpenCV 库
find_package(OpenCV REQUIRED)

# 打印 OpenCV 的路径和版本信息（可选，用于调试）
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

# 指定编译的源文件
set(SOURCE_FILES select.cpp)

# 添加可执行文件
add_executable(MyOpenCVApp ${SOURCE_FILES})

# 链接 OpenCV 库
target_link_libraries(MyOpenCVApp ${OpenCV_LIBS})
