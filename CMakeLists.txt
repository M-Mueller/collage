cmake_minimum_required (VERSION 3.0)
project(vis)

option(BUILD_DOCUMENTATION "Generate Doxygen documentation" OFF)

find_package(Qt5Widgets)

if(BUILD_DOCUMENTATION)
	find_package(Doxygen)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen must be installed to build the documentation.")
	else()
		configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
	    add_custom_target(doc
		    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		    COMMENT "Generating Doxygen documentation"
		    VERBATIM)
    endif()
endif()

#find_package(grogl REQUIRED)
set(GROGL_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/../grogl/include)
set(GROGL_LIBRARIES optimized ${CMAKE_SOURCE_DIR}/../grogl-build-release/libgrogl.a debug ${CMAKE_SOURCE_DIR}/../grogl-build-debug/libgrogl.a)

# enable Easylogging Qt support
add_definitions(-DELPP_QT_LOGGING)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
set(CMAKE_AUTOMOC ON)

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
    include/RendererProperty.h
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
