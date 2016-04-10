cmake_minimum_required (VERSION 3.0)
project(vis)

option(BUILD_DOCUMENTATION "Generate Doxygen documentation" OFF)

find_package(Qt5Quick)

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

add_subdirectory(glue)

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
    src/Texture.cpp
    src/Texture1D.cpp
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
    src/ClearFramebuffer.cpp
    src/Uniforms.cpp
    src/BindTexture.cpp
)

set(HEADERS
    include/VisualizationFramebuffer.h
    include/VisualizationRenderer.h
    include/RendererElement.h
    include/RendererProperty.h
    include/RenderPass.h
    include/Texture.h
    include/Texture1D.h
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
    include/ClearFramebuffer.h
    include/Uniforms.h
    include/BindTexture.h
)

set(QML
    qml/main.qml
    qml/VolumeRenderer.qml
    qml/TransferFunctionEditor.qml
    qml/PopupMenu.qml
    qml/PopupMenu.js
)

qt5_add_resources(RESOURCES qml.qrc)

include_directories(include glue/include)
add_executable(vis ${SOURCES} ${HEADERS} ${RESOURCES} ${QML})
target_link_libraries(vis glue -ldl -lpng)
qt5_use_modules(vis Core Gui Qml Quick)
