cmake_minimum_required(VERSION 3.21.1)

project(app)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
# qt_standard_project_setup() # auto uic/moc/dirs

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
# 把.qrc文件当成源文件放入add_executable/add_library中即可;对于静态库,qrc文件名必须全局唯一且符合c++变量名规范以便添加到main Q_INIT_RESOURCE(qrc_file_name);
set(CMAKE_AUTORCC ON)

if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE OFF) # ON will make app wihtout console
endif()
qt_add_executable(${PROJECT_NAME} main.cpp rcc.qrc)
add_dependencies(${PROJECT_NAME} MyWindow)

if(EMSCRIPTEN)
    # qtbase src/corelib/Qt6WasmMacros.cmake
    set_target_properties(${PROJECT_NAME} PROPERTIES
        QT_WASM_MAXIMUM_MEMORY "2GB" # 最大内存4G,必须是64KB的整数倍
        QT_WASM_INITIAL_MEMORY "500MB" # 初始内存500NB,必须是64KB的整数倍
        QT_WASM_PTHREAD_POOL_SIZE 4 # 除主线程为额外线程池大小,多线程启用时有效
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets MyWindow)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Exports
		RUNTIME DESTINATION ${install_runtime_dir}
		ARCHIVE DESTINATION ${install_archive_dir}
        LIBRARY DESTINATION ${install_library_dir}
)
