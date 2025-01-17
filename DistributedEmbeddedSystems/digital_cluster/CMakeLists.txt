cmake_minimum_required(VERSION 3.5)

project(digital_cluster VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the required Qt modules (Widgets, Mqtt)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Mqtt)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Mqtt)

include_directories(${CMAKE_SOURCE_DIR})

# Define source files
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        customdial.cpp
        mainwindow.h
        customdial.h
        mainwindow.ui
        batterydial.h
        batterydial.cpp
        tempbar.h
        tempbar.cpp
        autonomybar.h
        autonomybar.cpp
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(digital_cluster
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(digital_cluster SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(digital_cluster
            ${PROJECT_SOURCES}

        )
    endif()
endif()

# Link to the appropriate Qt libraries based on version
if(${QT_VERSION_MAJOR} EQUAL 6)
    target_link_libraries(digital_cluster PRIVATE Qt6::Widgets Qt6::Mqtt)
else()
    target_link_libraries(digital_cluster PRIVATE Qt5::Widgets Qt5::Mqtt)
endif()

# Set properties for macOS/iOS packaging
set_target_properties(digital_cluster PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install the application
include(GNUInstallDirs)
install(TARGETS digital_cluster
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(digital_cluster)
endif()

