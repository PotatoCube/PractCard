cmake_minimum_required(VERSION 3.10.0)
project(PractCard VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    flashcard.cpp
    cardmanager.cpp
    mainwindow.cpp)

add_executable(FlashcardsApp ${PROJECT_SOURCES})

target_link_libraries(FlashcardsApp PRIVATE Qt6::Widgets)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/words.txt")
    configure_file(words.txt ${CMAKE_CURRENT_BINARY_DIR}/words.txt COPYONLY)
endif()