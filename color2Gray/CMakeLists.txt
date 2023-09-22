cmake_minimum_required(VERSION 3.5)

project(color2Gray VERSION 0.1 LANGUAGES CXX)

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
    qt_add_executable(color2Gray
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

else()
    if(ANDROID)
        add_library(color2Gray SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(color2Gray
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(color2Gray PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_link_libraries(color2Gray PRIVATE ${VTK_LIBRARIES})

set_target_properties(color2Gray PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS color2Gray
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(color2Gray)
endif()

# If you use MSVC
if(MSVC)
file(GLOB IMAGE_DMP "${CMAKE_CURRENT_SOURCE_DIR}/*.bmp")
set(BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")

# Copy the image to the release folder and builde folder
add_custom_command(TARGET color2Gray POST_BUILD 
                                        COMMAND ${CMAKE_COMMAND} -E copy
                                        ${IMAGE_DMP} $<TARGET_FILE_DIR:color2Gray>
                                         COMMAND ${CMAKE_COMMAND} -E copy
                                        ${IMAGE_DMP}  ${BUILD_DIR}
)

endif()

