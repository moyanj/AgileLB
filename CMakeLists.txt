cmake_minimum_required(VERSION 3.10)

project(AgileLB)

set(CMAKE_CXX_STANDARD 17)

# 设置LB子目录为包含源文件和头文件的目录
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/LB)

# 设置LB头文件目录
set(LB_INCLUDE_DIR ${SRC}/include)
set(LIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

# 搜索LB目录下的所有源文件
file(GLOB_RECURSE LB_SOURCES "${SRC}/*.cpp")

# 搜索LB头文件目录下的所有头文件
file(GLOB_RECURSE LB_HEADERS "${LB_INCLUDE_DIR}/*.h" "${LB_INCLUDE_DIR}/*.hpp")

# 搜索libs目录下的所有头文件
file(GLOB_RECURSE LIB_HEADERS "${LIB_INCLUDE_DIR}/*.h" "${LIB_INCLUDE_DIR}/*.hpp")

# 查找Boost库
find_package(Boost REQUIRED)

# 查找yaml-cpp库
find_package(YAML-CPP REQUIRED)

# 查找OpenSSL库
find_package(OpenSSL REQUIRED)

# 添加可执行文件，包含LB目录下的所有源文件和头文件
add_executable(AgileLB ${LB_SOURCES} ${LB_HEADERS})

# 包含Boost头文件
target_include_directories(AgileLB PRIVATE ${Boost_INCLUDE_DIRS})

# 包含yaml-cpp头文件
target_include_directories(AgileLB PRIVATE ${YAML_CPP_INCLUDE_DIR})

# 包含openssl头文件
target_include_directories(AgileLB PRIVATE ${OpenSSL_INCLUDE_DIR})

# 包含LB头文件目录
target_include_directories(AgileLB PRIVATE ${LB_INCLUDE_DIR})

# 包含LB头文件目录
target_include_directories(AgileLB PRIVATE ${LIB_INCLUDE_DIR})

# 链接Boost库
target_link_libraries(AgileLB ${Boost_LIBRARIES})

# 链接yaml-cpp库
target_link_libraries(AgileLB ${YAML_CPP_LIBRARIES})

target_link_libraries(AgileLB ${OpenSSL_LIBRARIES})