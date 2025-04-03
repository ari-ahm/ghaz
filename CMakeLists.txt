cmake_minimum_required(VERSION 3.16)

# Define the project
project(dgport VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the required Qt packages
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)


# Enable automatic MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Set the output directory for the executable to the project root
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Collect all source and header files recursively from the src directory
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.hpp)

# Add the executable target
add_executable(dgport ${SOURCES})

# Link the Qt libraries
target_link_libraries(dgport PRIVATE Qt6::Widgets Qt6::OpenGLWidgets)

# Specify the include directories
target_include_directories(dgport PRIVATE ${CMAKE_SOURCE_DIR}/src)
