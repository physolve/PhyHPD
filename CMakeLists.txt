cmake_minimum_required(VERSION 3.16)

set(EXECUTABLE_NAME "MHgrph")

project(${EXECUTABLE_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 6.8 COMPONENTS Quick QuickControls2 Charts PrintSupport SerialPort OpenGL REQUIRED)

qt_add_resources(QML_RESOURCES resources.qrc)

file(GLOB CPP_SOURCES src/*.cpp)
file(GLOB HPP_SOURCES src/*.h)

file(GLOB CPP_LIBS lib/*.cpp)
file(GLOB HPP_LIBS lib/*.h)

add_definitions(-DQCUSTOMPLOT_USE_OPENGL )

qt_add_executable(${EXECUTABLE_NAME}
    ${CPP_SOURCES} ${HPP_SOURCES}
    ${CPP_LIBS} ${HPP_LIBS}
    ${QML_RESOURCES}
)
target_link_libraries(${EXECUTABLE_NAME}
    PRIVATE Qt6::Quick
            Qt6::QuickControls2
            Qt6::Charts
            Qt6::PrintSupport
            Qt6::SerialPort
            Qt6::OpenGL
            opengl32
)