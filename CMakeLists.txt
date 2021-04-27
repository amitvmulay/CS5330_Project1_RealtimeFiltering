cmake_minimum_required(VERSION 3.15)

project(CVProject1)

# Include headers
include_directories("/usr/local/include/")
include_directories("./include/")

# Create the executable
file(GLOB SRC "src/*.cpp")
add_executable(imgDisp ./src/imgDisplay.cpp)
add_executable(vidDisp ./src/filters.cpp ./src/vidDisplay.cpp)

find_package(OpenCV REQUIRED)
target_link_libraries(imgDisp ${OpenCV_LIBS})
target_link_libraries(vidDisp ${OpenCV_LIBS})

