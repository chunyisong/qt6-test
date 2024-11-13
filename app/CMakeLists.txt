cmake_minimum_required(VERSION 3.21.1)

project(app)

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools)

qt_standard_project_setup() # auto uic/moc/dirs
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTOUIC ON)
# 把.qrc文件当成源文件放入add_executable/add_library中即可;对于静态库,qrc文件名必须全局唯一且符合c++变量名规范以便添加到main Q_INIT_RESOURCE(qrc文件名);
set(CMAKE_AUTORCC ON)

#设置自动生成目录
set(autogen_dir ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_autogen)
# 编译阶段(本次构建树需要的)头文件目录列表;生成表达式内务必使用绝对路径;需要发布的ui头文件需要特别处理
set(build_interface_dirs
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${autogen_dir}/include>
    $<BUILD_INTERFACE:${autogen_dir}/include_${CMAKE_BUILD_TYPE}> # msvc根据构建类型放置自动生成的ui等文件
)

qt_policy(SET QTP0003 NEW) # 对qt_add_library等操作,让Qt6优先遵守已设定的BUILD_SHARED_LIBS,未设定则根据qt类型确定(静态qt则静态库)

message(STATUS "*****CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX},CMAKE_INSTALL_FULL_BINDIR:${CMAKE_INSTALL_FULL_BINDIR},BUILD_SHARED_LIBS:${BUILD_SHARED_LIBS}")

qt_add_executable(${PROJECT_NAME} main.cpp rcc.qrc)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets MyWindow)
add_dependencies(${PROJECT_NAME} MyWindow)

# 添加项目构建和安装发布需要的头文件目录
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${build_interface_dirs}
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/ui"
    AUTOGEN_BUILD_DIR ${autogen_dir}
    WIN32_EXECUTABLE OFF # For WIN32,default ON will make app wihtout console (CMAKE_WIN32_EXECUTABLE)
)
if(EMSCRIPTEN)
    # qtbase src/corelib/Qt6WasmMacros.cmake
    set_target_properties(${PROJECT_NAME} PROPERTIES
        QT_WASM_MAXIMUM_MEMORY "2GB" # 最大内存4G,必须是64KB的整数倍
        QT_WASM_INITIAL_MEMORY "500MB" # 初始内存500NB,必须是64KB的整数倍
        QT_WASM_PTHREAD_POOL_SIZE 4 # 除主线程为额外线程池大小,多线程启用时有效
    )
    # 添加命令行参数把config目录映射到emscripten虚拟文件系统的根目录下
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/res@/res"
    )
    # 安装部署相关的html文件到指定目录
    install(
        DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/
        DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}
        FILES_MATCHING
        REGEX "^${CMAKE_CURRENT_BINARY_DIR}/[^/]+\\.(js|wasm|data|html|map|ico|svg)$"
        PATTERN "res/*"
        PATTERN "CMakeFiles" EXCLUDE
        PATTERN "*_autogen" EXCLUDE
    )
else()
    install(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
    # 拷贝配置目录本身及其后代目录结构和文件;如果目录后缀不带反斜杠/则在安装时会包括目录本身
    # install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/config
    #         DESTINATION ${install_runtime_dir}
    # )

    qt_generate_deploy_app_script(
        TARGET ${PROJECT_NAME}
        OUTPUT_SCRIPT deploy_script
        NO_TRANSLATIONS
        NO_COMPILER_RUNTIME
        # NO_UNSUPPORTED_PLATFORM_ERROR
    )
    message(STATUS "**** ${PROJECT_NAME} deploy_script: ${deploy_script}")
    install(SCRIPT ${deploy_script})
endif()

