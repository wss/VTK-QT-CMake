cmake_minimum_required(VERSION 3.5)

project(changeImagePixel VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(VTK REQUIRED)

if(NOT VTK_FOUND)
      message(FATAL_ERROR "Axes: Unable to find the VTK build folder.")
endif()

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(changeImagePixel
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

else()
    if(ANDROID)
        add_library(changeImagePixel SHARED
            ${PROJECT_SOURCES}
        )

    else()
        add_executable(changeImagePixel
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(changeImagePixel PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(changeImagePixel PRIVATE ${VTK_LIBRARIES})


set_target_properties(changeImagePixel PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS changeImagePixel
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(MSVC)
file(GLOB IMAGE_BMP "${CMAKE_SOURCE_DIR}/*.bmp")
set(BUILD_DIR "${CMAKE_SOURCE_DIR}/build")

add_custom_command(TARGET changeImagePixel POST_BUILD 
                                        COMMAND ${CMAKE_COMMAND} -E copy
                                       ${IMAGE_BMP} $<TARGET_FILE_DIR:changeImagePixel> 
                                       COMMAND ${CMAKE_COMMAND} -E copy
                                       ${IMAGE_BMP}  ${BUILD_DIR}
)

endif()
