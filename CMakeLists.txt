cmake_minimum_required(VERSION 3.20)
project(raytracing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -O2 -fopenmp")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

set(ROOT "${CMAKE_CURRENT_LIST_DIR}")

option(USE_SOLUTION "Build the reference solution" OFF)
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
if (USE_SOLUTION)
  set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/solution")
endif()

set(BVH_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/AABB/src")

file(GLOB SRCFILES CONFIGURE_DEPENDS "${SRC_DIR}/*.cpp")

set(HW2FILES
        "${SRC_DIR}/Plane.cpp"
        "${SRC_DIR}/Sphere.cpp"
        "${BVH_SRC_DIR}/Triangle.cpp"
        "${BVH_SRC_DIR}/TriangleSoup.cpp"
        "${BVH_SRC_DIR}/first_hit.cpp"
        "${SRC_DIR}/viewing_ray.cpp"
        "${SRC_DIR}/write_ppm.cpp"
)
list(REMOVE_ITEM SRCFILES ${HW2FILES})

set_source_files_properties(
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
        PROPERTIES
        COMPILE_FLAGS "-w"
)

list(APPEND SRCFILES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

# Add AABB subdirectory BEFORE creating executable
add_subdirectory(AABB)

set(EXTRA_SOURCES "")
if (DEFINED LIBIGL_EXTRA_SOURCES)
  list(APPEND EXTRA_SOURCES ${LIBIGL_EXTRA_SOURCES})
endif()

# Create executable WITHOUT AABB in the source list
add_executable(${PROJECT_NAME} ${SRCFILES} ${EXTRA_SOURCES})

# Link AABB library to executable
target_link_libraries(${PROJECT_NAME} PRIVATE AABB)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        "${ROOT}/include"
)
if (EXISTS "${ROOT}/eigen")
  target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "${ROOT}/eigen")
endif()
if (EXISTS "${ROOT}/json")
  target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "${ROOT}/json")
endif()

set(HW2LIB_DIR  "" CACHE PATH   "Directory containing prebuilt hw2 library (optional)")
set(HW2LIB_NAME "hw2" CACHE STRING "Name of the prebuilt hw2 library (optional)")

if (HW2LIB_DIR)
  message(STATUS "Using HW2LIB_DIR: ${HW2LIB_DIR}, HW2LIB_NAME: ${HW2LIB_NAME}")
  target_link_directories(${PROJECT_NAME} PRIVATE "${HW2LIB_DIR}")
  target_link_libraries(${PROJECT_NAME} PRIVATE ${HW2LIB_NAME})
else()
  message(STATUS "No HW2LIB_DIR provided, building hw2 from source.")
  add_library(hw2 STATIC ${HW2FILES})

  target_include_directories(hw2
          PRIVATE
          "${ROOT}/include"
  )
  if (EXISTS "${ROOT}/eigen")
    target_include_directories(hw2 SYSTEM PRIVATE "${ROOT}/eigen")
  endif()
  if (EXISTS "${ROOT}/json")
    target_include_directories(hw2 SYSTEM PRIVATE "${ROOT}/json")
  endif()

  target_link_libraries(${PROJECT_NAME} PRIVATE hw2)
endif()

if (MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
  if (TARGET hw2)
    target_compile_options(hw2 PRIVATE /W4 /permissive-)
  endif()
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
  if (TARGET hw2)
    target_compile_options(hw2 PRIVATE -Wall -Wextra -Wpedantic)
  endif()
endif()