cmake_minimum_required(VERSION 3.16)

# Make sure all the submodules are there
MESSAGE("Initializing submodules")
execute_process(
    COMMAND git submodule init
    COMMAND git submodule update --remote --merge
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

# Remember to set your port name in the <port_name_here> field
project(miniSystem_Linux)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

# Include the miniSystem core library
add_subdirectory(miniSystem)
target_link_libraries(${PROJECT_NAME} PRIVATE miniSystem)