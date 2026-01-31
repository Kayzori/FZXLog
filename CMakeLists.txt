cmake_minimum_required(VERSION 3.10.0)
project(FZXLog VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(FZXLog)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/FZXLog
)
