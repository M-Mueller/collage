cmake_minimum_required (VERSION 3.0)
project(VolumeRenderer)

find_package(Qt5Quick)
set(CMAKE_AUTOMOC ON)

set(SOURCES
	main.cpp
	PopOver.cpp
	TransferFunctionEditor.cpp
)

set(HEADERS
	PopOver.h
	TransferFunctionEditor.h
)

set(QML
	main.qml
	VolumeRenderer.qml
	PopupMenu.qml
)

qt5_add_resources(RESOURCES qml.qrc)

include_directories(include glue/include)
add_executable(VolumeRenderer ${SOURCES} ${HEADERS} ${QML} ${RESOURCES})
qt5_use_modules(VolumeRenderer Core Gui Qml Quick)

# Qt searches for plugins next to executable
add_custom_target(CopyPlugin ALL
	COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/collage"
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:collage-plugin> "${CMAKE_CURRENT_BINARY_DIR}/collage"
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/plugin/qmldir" "${CMAKE_CURRENT_BINARY_DIR}/collage"
)
add_dependencies(VolumeRenderer CopyPlugin)
