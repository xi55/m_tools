cmake_minimum_required(VERSION 3.15)

project(run)

add_subdirectory(json)

add_executable(run main.cpp)

target_link_libraries(run PUBLIC json)

target_include_directories(run PUBLIC 
                            ${PROJECT_BINARY_DIR}
                            ${PROJECT_SOURCE_DIR}/json)   