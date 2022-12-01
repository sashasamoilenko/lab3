cmake_minimum_required(VERSION 3.23)
project(untitled5)

set(CMAKE_CXX_STANDARD 14)

#set(SFML_ROOT "/Users/sasha/CLionProjects/SFML-2.5.1")
#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

find_package(SFML 2 REQUIRED system window graphics)

add_executable(untitled5 main.cpp)

if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif()