# CMakeList.txt: timeControllerService 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("timeControllerService")
# 将源代码添加到此项目的可执行文件。

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	# msvcpch.cmake的路径
	set(MSVCCMAKE_PCH ${CMAKE_SOURCE_DIR}/build/)
	# 预编译头文件和源文件的路径
	set(STDAFX_PCH_H ${CMAKE_SOURCE_DIR}/Protocol/stdafx.h)
	set(STDAFX_PCH_C ${CMAKE_SOURCE_DIR}/Protocol/stdafx.cpp)	
	
	# 添加预编译cmake的路径
	LIST(APPEND CMAKE_MODULE_PATH ${MSVCCMAKE_PCH})
	# 包含msvcpch.cmake
	INCLUDE(msvcpch.cmake)
endif ()
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	# 添加源文件，生成可执行文件
	add_executable (timeControllerService WIN32 "timeControllerService.cpp" "timeControllerService.h" ${STDAFX_PCH_C})
	# 添加链接库	
	# 此处是关键，添加我们在msvcpch.cmake定义的宏
	USE_MSVC_PCH(timeControllerService stdafx.h ${STDAFX_PCH_C})
endif ()
# TODO: 如有需要，请添加测试并安装目标。
