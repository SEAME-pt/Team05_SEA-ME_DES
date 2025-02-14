cmake_minimum_required(VERSION 3.14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)   # If using .ui files
project(digital_cluster VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS include/*.h)
option(BUILD_TESTS "Build test targets" OFF)

set(CMAKE_COLOR_MAKEFILE ON)
find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets Mqtt Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Mqtt Test)
include_directories(${CMAKE_SOURCE_DIR})

option(USE_SANITIZER "Enable sanitizer" OFF)

if(USE_SANITIZER)
  set(SANITIZER_COMPILE_FLAGS -fsanitize=address -fno-omit-frame-pointer)
  set(SANITIZER_LINK_FLAGS -fsanitize=address)
  # set(SANITIZER_FLAGS -fsanitize=thread) # Uncomment this line if you want ThreadSanitizer instead:
endif()

include(FetchContent)
FetchContent_Declare(
googletest
URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

if(BUILD_TESTS)
    add_compile_definitions(BUILD_TESTS)
    file(GLOB TESTS test/*.cpp)
    add_executable(digital_cluster_test
        ${SOURCES}
        ${HEADERS}
        ${TESTS}
    )
    target_link_libraries(digital_cluster_test
        gmock
        gtest
        gtest_main
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Mqtt
        ${CMAKE_CXX_LINKER_FLAGS} 
        Qt5::Test
    )
    target_include_directories(digital_cluster_test PUBLIC
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
    )

    if(USE_SANITIZER)
        target_compile_options(digital_cluster_test PRIVATE ${SANITIZER_COMPILE_FLAGS})
        target_link_options(digital_cluster_test PRIVATE ${SANITIZER_LINK_FLAGS})
    endif()

    enable_testing()
    add_test(NAME DigitalClusterTest COMMAND digital_cluster_test)
    
    else()
        if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
            qt_add_executable(digital_cluster
                MANUAL_FINALIZATION
                ${SOURCES} 
                ${HEADERS}
                main.cpp
                )
                qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
        else()
            if(ANDROID)
                add_library(digital_cluster SHARED
                    ${SOURCES}
                )
                else()
                    add_executable(digital_cluster
                    ${SOURCES}
                    main.cpp
                    ${HEADERS}
                    )
                    endif()
            endif()
            target_link_libraries(digital_cluster
                gmock
                Qt${QT_VERSION_MAJOR}::Widgets
                Qt${QT_VERSION_MAJOR}::Mqtt
            )
            target_include_directories(digital_cluster PUBLIC
                ${gmock_SOURCE_DIR}/include
            )
            if(USE_SANITIZER)
                target_compile_options(digital_cluster PRIVATE ${SANITIZER_COMPILE_FLAGS})
                target_link_options(digital_cluster PRIVATE ${SANITIZER_FLAGS})
            endif()
    
        if(QT_VERSION_MAJOR EQUAL 6)
            qt_finalize_executable(digital_cluster)
        endif()
            
        set_target_properties(digital_cluster PROPERTIES
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )
        include(GNUInstallDirs)
        install(TARGETS digital_cluster
            BUNDLE DESTINATION .
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            )
            if(QT_VERSION_MAJOR EQUAL 6)
            qt_finalize_executable(digital_cluster)
        endif()
    endif()