cmake_minimum_required(VERSION 3.9)

qt5_wrap_ui(Viewer_UIS mainwindow.ui  SaveSelection.ui)
qt5_add_resources(Viewer_QRC mainwindow.qrc)
set(Viewer_INC 
    MainWindow.h 
    MeshViewer.h 
    MeshModel.h 
    MeshSelector.h
    MeshSelector.h
    Common.h
    ArcBall.h
    )
set(Viewer_SRC main.cpp
    MainWindow.cpp
    MeshViewer.cpp 
    MeshModel.cpp
    MeshSelector.cpp
    MeshSelector.cpp
    Common.cpp
    ArcBall.cpp)

add_executable(MeshViewer
        ${Viewer_INC}
        ${Viewer_SRC}
        ${Viewer_UIS}
        ${Viewer_QRC}
        )
#qt5_use_modules(window Core Gui Widgets)
target_link_libraries(MeshViewer Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL OpenMesh ANN ${OpenGL_LIBS})

macro(qt5_copy_dll APP DLL)
    # https://gist.github.com/Rod-Persky/e6b93e9ee31f9516261b
    get_target_property(Qt5_${DLL}Location Qt5::${DLL} LOCATION)  # find the release *.dll file
    get_target_property(Qt5_${DLL}LocationDebug Qt5::${DLL} IMPORTED_LOCATION_DEBUG)  # find the debug *d.dll file
    message("Qt5_${DLL}Location = ${Qt5_${DLL}Location}")
    message("Qt5_${DLL}LocationDebug = ${Qt5_${DLL}LocationDebug}")
    add_custom_command(TARGET ${APP} POST_BUILD
       COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${Qt5_${DLL}LocationDebug}> $<$<NOT:$<CONFIG:Debug>>:${Qt5_${DLL}Location}> $<TARGET_FILE_DIR:${APP}>)
endmacro()

if(WIN32)
    qt5_copy_dll(MeshViewer Core)
    qt5_copy_dll(MeshViewer Gui)
    qt5_copy_dll(MeshViewer Widgets)
    qt5_copy_dll(MeshViewer OpenGL)
endif()
