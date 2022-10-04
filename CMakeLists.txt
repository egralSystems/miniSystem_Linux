cmake_minimum_required(VERSION 3.16)

# Make sure all the submodules are there
execute_process(
    COMMAND "git submodule init  &&git submodule update"
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)

# Remember to set your port name in the <port_name_here> field
project(miniSystem_<port_name_here>)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

# Include the miniSystem core library
add_subdirectory(miniSystem)
target_link_libraries(${PROJECT_NAME} PRIVATE miniSystem)