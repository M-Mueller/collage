cmake_minimum_required (VERSION 3.0)
project(vis)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)

#find_package(grogl REQUIRED)
set(GROGL_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/../grogl/include)
set(GROGL_LIBRARIES optimized ${CMAKE_SOURCE_DIR}/../grogl-build-release/libgrogl.a debug ${CMAKE_SOURCE_DIR}/../grogl-build-debug/libgrogl.a)

add_definitions(-DELPP_QT_LOGGING)

set(SOURCES 
    src/main.cpp
    src/VisualizationFramebuffer.cpp
    src/VisualizationRenderer.cpp
    src/RendererElement.cpp
    src/RenderPass.cpp
    src/Uniform.cpp
    src/Texture.cpp
    src/Texture2D.cpp
    src/Texture3D.cpp
    src/Framebuffer.cpp
    src/RenderBuffer.cpp
    src/Camera.cpp
    src/TurnTableCamera.cpp
    src/Entity.cpp
    src/Rectangle.cpp
    src/NearClippingRectangle.cpp
    src/Cube.cpp
)

set(HEADERS 
    include/VisualizationFramebuffer.h
    include/VisualizationRenderer.h
    include/RendererElement.h
    include/RenderPass.h
    include/Uniform.h
    include/Texture.h
    include/Texture2D.h
    include/Texture3D.h
    include/Framebuffer.h
    include/RenderBuffer.h
    include/Camera.h
    include/TurnTableCamera.h
    include/Entity.h
    include/Rectangle.h
    include/NearClippingRectangle.h
    include/Cube.h
)

set(QML
    qml/main.qml
    qml/VolumeRenderer.qml
)

qt5_add_resources(RESOURCES qml.qrc)

include_directories(include ${GROGL_INCLUDE_DIRS})
add_executable(vis ${SOURCES} ${HEADERS} ${RESOURCES} ${QML})
target_link_libraries(vis ${GROGL_LIBRARIES} -ldl -lpng)
qt5_use_modules(vis Core Gui Qml Quick Widgets)
