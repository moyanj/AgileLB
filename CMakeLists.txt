cmake_minimum_required(VERSION 3.10)

project(AgileLB)

set(CMAKE_CXX_STANDARD 17)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/LB)

set(LB_INCLUDE_DIR ${SRC}/include)
set(LIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache) # Less useful to do it for linking, see edit2
endif(CCACHE_FOUND)

# 搜索源文件
file(GLOB_RECURSE LB_SOURCES "${SRC}/*.cpp")
file(GLOB_RECURSE LB_HEADERS "${LB_INCLUDE_DIR}/*.h" "${LB_INCLUDE_DIR}/*.hpp")
file(GLOB_RECURSE LIB_HEADERS "${LIB_INCLUDE_DIR}/*.h" "${LIB_INCLUDE_DIR}/*.hpp")

add_executable(AgileLB ${LB_SOURCES} ${LB_HEADERS})

# 头文件目录
target_include_directories(AgileLB PRIVATE ${LB_INCLUDE_DIR})
target_include_directories(AgileLB PRIVATE ${LIB_INCLUDE_DIR})

# 查找库
find_package(Boost REQUIRED) #Boost
find_package(YAML-CPP REQUIRED) # yaml-cpp
find_package(OpenSSL REQUIRED) #openssl

# 库的头文件
target_include_directories(AgileLB PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(AgileLB PRIVATE ${YAML_CPP_INCLUDE_DIR})
target_include_directories(AgileLB PRIVATE ${OpenSSL_INCLUDE_DIR})

target_link_libraries(AgileLB ${Boost_LIBRARIES})
target_link_libraries(AgileLB ${YAML_CPP_LIBRARIES})
target_link_libraries(AgileLB ${OpenSSL_LIBRARIES})