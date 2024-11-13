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
include(GNUInstallDirs) # 设置编译产物安装时相对于CMAKE_INSTALL_PREFIX的相对目录名CMAKE_INSTALL_<dir>系列
message(STATUS "*****CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX},CMAKE_INSTALL_FULL_BINDIR:${CMAKE_INSTALL_FULL_BINDIR},BUILD_SHARED_LIBS:${BUILD_SHARED_LIBS}")

# 包含子项目。
add_subdirectory("app-lib")
add_subdirectory ("app")
