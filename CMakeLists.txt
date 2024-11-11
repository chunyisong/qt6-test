# qt6 wasm/static/apple requires cmake >= 3.21.1
cmake_minimum_required (VERSION 3.21.1)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("qt6test")
# qt6 requires a compiler >= CXX17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(install_library_dir lib CACHE PATH "CMAKE_INSTALL_PREFIX/lib,共享库.so/.lib/cmake配置文件等存放目录" FORCE)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)    #CMAKE的内建变量，为8就说明编译的是64bit的程序
    set(install_library_dir lib64 CACHE PATH "CMAKE_INSTALL_PREFIX/lib64,共享库.so/.lib64/cmake配置文件等存放目录" FORCE)
endif()
set(install_archive_dir ${install_library_dir} CACHE PATH "CMAKE_INSTALL_PREFIX/lib,静态库.a等存放目录" FORCE)
set(install_runtime_dir bin CACHE PATH "CMAKE_INSTALL_PREFIX/bin,运行库.dll/.exe等应用程序存放目录" FORCE)
set(install_wasm_dir wasm CACHE PATH "CMAKE_INSTALL_PREFIX/wasm,wasm应用程序存放目录" FORCE)
set(install_includes_dir include CACHE PATH "CMAKE_INSTALL_PREFIX/include,库头文件库.h等存放目录" FORCE)

# 包含子项目。
add_subdirectory("app-lib")
add_subdirectory ("app")
