cmake_minimum_required(VERSION 3.21.1)

project(MyWindow)

#设置自动生成目录
set(autogen_dir ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_autogen)
# 安装阶段(类库使用者需要的)头文件目录列表
set(install_includes_dir ${install_includes_dir}/${lib_name})
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# qt_standard_project_setup() # auto uic/moc/dirs
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
# 把.qrc文件当成源文件放入add_executable/add_library中即可;对于静态库,qrc文件名必须全局唯一且符合c++变量名规范以便添加到main Q_INIT_RESOURCE(qrc文件名);
set(CMAKE_AUTORCC ON)

if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE OFF) # ON will make app wihtout console
endif()
qt_policy(SET QTP0003 NEW) # 对qt_add_library等操作,让Qt6优先遵守已设定的BUILD_SHARED_LIBS,未设定则根据qt类型确定(静态qt则静态库)

qt_add_library(${PROJECT_NAME} mywindow.cpp mywindow.qrc mywindow2.qrc)
if(BUILD_SHARED_LIBS)
    message("***${CMAKE_CURRENT_SOURCE_DIR}-BUILD_SHARED_LIBS:" ${BUILD_SHARED_LIBS})
    target_compile_definitions(${PROJECT_NAME} PRIVATE APP_LIB_EXPORTS)
    target_compile_definitions(${PROJECT_NAME} INTERFACE APP_LIB_IMPORTS)
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOGEN_BUILD_DIR ${autogen_dir}
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/ui"
)
# 编译阶段(本次构建树需要的)头文件目录列表;生成表达式内务必使用绝对路径;需要发布的ui头文件需要特别处理 # msvc根据构建类型放置自动生成的ui等文件
set(build_interface_dirs
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${autogen_dir}/include>
    $<BUILD_INTERFACE:${autogen_dir}/include_${CMAKE_BUILD_TYPE}>
)
# 添加项目构建和安装发布需要的头文件目录
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${build_interface_dirs}
        $<INSTALL_INTERFACE:${install_includes_dir}>
    # PRIVATE
    #     ${CMAKE_CURRENT_SOURCE_DIR}/srccd
)
message(STATUS "*****CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX},CMAKE_INSTALL_FULL_BINDIR:${CMAKE_INSTALL_FULL_BINDIR},BUILD_SHARED_LIBS:${BUILD_SHARED_LIBS}")
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
if(EMSCRIPTEN)
    # qtbase src/corelib/Qt6WasmMacros.cmake
    set_target_properties(${PROJECT_NAME} PROPERTIES
        QT_WASM_MAXIMUM_MEMORY "2GB" # 最大内存4G,必须是64KB的整数倍
        QT_WASM_INITIAL_MEMORY "500MB" # 初始内存500NB,必须是64KB的整数倍
        QT_WASM_PTHREAD_POOL_SIZE 4 # 除主线程为额外线程池大小,多线程启用时有效
    )
endif()
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Exports
		RUNTIME DESTINATION ${install_runtime_dir}
		ARCHIVE DESTINATION ${install_archive_dir}
        LIBRARY DESTINATION ${install_library_dir}
)
# 拷贝代码目录下的头文件及其树结构;如果目录后缀不带反斜杠/则在安装时会包括目录本身
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION ${install_includes_dir}
        FILES_MATCHING PATTERN "*.h"
        REGEX "(/|^)([gG]eneratedFiles|bin|dist|build|lib|Debug|debug|Release|release|(\\.[^/^\\.]*))/" EXCLUDE
)
# 需要发布的ui头文件需要拷贝到包含目录
install(DIRECTORY ${autogen_dir}/include/  ${autogen_dir}/include_${CMAKE_BUILD_TYPE}/
        DESTINATION ${install_includes_dir}
        FILES_MATCHING PATTERN "*.h"
        REGEX "(/|^)([gG]eneratedFiles|bin|dist|build|lib|Debug|debug|Release|release|(\\.[^/^\\.]*))/" EXCLUDE
)