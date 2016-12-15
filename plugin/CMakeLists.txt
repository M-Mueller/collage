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

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_AUTOMOC ON)

# enable Easylogging Qt support
add_definitions(-DELPP_QT_LOGGING)

include_directories(../glue/include)
include_directories(${CMAKE_BINARY_DIR}/glue/include)
include_directories(include)

set(SOURCES
    src/Canvas.cpp
    src/Renderer.cpp
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
    src/Cube.cpp
    src/ClearFramebuffer.cpp
    src/Uniforms.cpp
    src/BindTexture.cpp
    src/Octree.cpp
    src/BoundingBox.cpp
    src/Image.cpp
    src/ImageData.cpp
    src/CollagePlugin.cpp
)

set(HEADERS
    include/Canvas.h
    include/Renderer.h
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
    include/Cube.h
    include/ClearFramebuffer.h
    include/Uniforms.h
    include/BindTexture.h
    include/Octree.h
    include/BoundingBox.h
    include/Image.h
    include/ImageData.h
    include/CollagePlugin.h
)

include_directories(include glue/include)
add_library(collage-plugin SHARED ${SOURCES} ${HEADERS} ${RESOURCES})
target_link_libraries(collage-plugin glue -ldl -lpng)
qt5_use_modules(collage-plugin Core Gui Qml Quick)
