cmake_minimum_required(VERSION 3.20)
project(raytracing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(OPTIMIZATION_FLAGS "-O3 -march=native -mtune=native -flto")
set(PARALLEL_FLAGS "-fopenmp")
set(WARNING_FLAGS -Wall -Wextra -Wpedantic -Wconversion)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPTIMIZATION_FLAGS} ${PARALLEL_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")

set(ROOT "${CMAKE_CURRENT_LIST_DIR}")
set(SRC_DIR "${ROOT}/src")

file(GLOB SRCFILES CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")

set(CORE "${SRC_DIR}/*.cpp")
list(REMOVE_ITEM SRCFILES ${CORE})

set_source_files_properties(
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
        PROPERTIES
        COMPILE_FLAGS "-w"
)

list(APPEND SRCFILES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

set(EXTRA_SOURCES "")
if(DEFINED LIBIGL_EXTRA_SOURCES)
    list(APPEND EXTRA_SOURCES ${LIBIGL_EXTRA_SOURCES})
endif()

add_executable(${PROJECT_NAME} ${SRCFILES} ${EXTRA_SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})

target_include_directories(${PROJECT_NAME}
        PRIVATE
        "${ROOT}/include"
)

if(EXISTS "${ROOT}/eigen")
    target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "${ROOT}/eigen")
endif()

if(EXISTS "${ROOT}/json")
    target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "${ROOT}/json")
endif()

message(STATUS "Building core library from source")

file(GLOB CORE_SRCFILES CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")
add_library(core STATIC ${CORE_SRCFILES})

target_include_directories(core
        PRIVATE
        "${ROOT}/include"
)

if(EXISTS "${ROOT}/eigen")
    target_include_directories(core SYSTEM PRIVATE "${ROOT}/eigen")
endif()

if(EXISTS "${ROOT}/json")
    target_include_directories(core SYSTEM PRIVATE "${ROOT}/json")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE core)