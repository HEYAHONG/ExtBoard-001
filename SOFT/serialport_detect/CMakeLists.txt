cmake_minimum_required(VERSION 3.20)

#程序名称
Set(PROJECT_NAME extboard_001_serialport_detect)
#主版本
set(PROJECT_MAJOR_VERSION_STR 1)
#次版本号
set(PROJECT_MINOR_VERSION_STR 0)
#修订号
set(PROJECT_REVISION_VERSION_STR 0)
#构建版本（以时间戳天数作为构建版本）
string(TIMESTAMP PROJECT_BUILD_TIME_SECOND "%s" UTC)
math(EXPR  PROJECT_BUILD_VERSION_STR "${PROJECT_BUILD_TIME_SECOND}/60/60/24"  OUTPUT_FORMAT DECIMAL)
#程序版本
set(PROJECT_VERSION_STR "${PROJECT_MAJOR_VERSION_STR}.${PROJECT_MINOR_VERSION_STR}.${PROJECT_REVISION_VERSION_STR}.${PROJECT_BUILD_VERSION_STR}")
message(STATUS "Version:${PROJECT_VERSION_STR}")
configure_file(Version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/Version.h)

#工程设置
Project(${PROJECT_NAME} C CXX ASM)
Project(${PROJECT_NAME}	VERSION "${PROJECT_VERSION_STR}")

#C++标准
set(CMAKE_CXX_STANDARD 20)

#MSVC需使用UTF-8编码
if(MSVC)
	add_compile_options(-utf-8 )
endif()


#添加源代码
file(GLOB MAIN_CXX_C_FILES *.cpp *.c  *.h)

#添加可执行文件
add_executable(${PROJECT_NAME} ${MAIN_CXX_C_FILES})

include(FindPkgConfig)

#链接libserialport
pkg_check_modules(LibSerialPort REQUIRED IMPORTED_TARGET libserialport)
target_link_libraries(${PROJECT_NAME} PkgConfig::LibSerialPort)

#安装设置
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME})
