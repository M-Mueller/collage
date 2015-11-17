cmake_minimum_required (VERSION 3.0)
project(vis)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)

#find_package(grogl REQUIRED)
set(GROGL_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/../grogl/include)
set(GROGL_LIBRARIES optimized ${CMAKE_SOURCE_DIR}/../grogl-build-release/libgrogl.a debug ${CMAKE_SOURCE_DIR}/../grogl-build-debug/libgrogl.a)

set(SOURCES 
    src/main.cpp
    src/VisualizationFramebuffer.cpp
    src/VisualizationRenderer.cpp
    src/RenderPass.cpp
    src/Entity.cpp
    src/Camera.cpp
    src/Triangle.cpp
    src/Cube.cpp
)

set(HEADERS 
    include/VisualizationFramebuffer.h
    include/VisualizationRenderer.h
    include/RenderPass.h
    include/Entity.h
    include/Camera.h
    include/Triangle.h
    include/Cube.h
)

set(QML
    qml/main.qml
)

qt5_add_resources(RESOURCES qml.qrc)

include_directories(include ${GROGL_INCLUDE_DIRS})
add_executable(vis ${SOURCES} ${HEADERS} ${RESOURCES} ${QML})
target_link_libraries(vis ${GROGL_LIBRARIES} -ldl)
qt5_use_modules(vis Core Gui Qml Quick Widgets)
