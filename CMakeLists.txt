cmake_minimum_required(VERSION 3.0.0)
Project(ADStruct)

set(CMAKE_CXX_STANDARD 20)

# place all predefined target projects into a filter
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PREDEFINED_TARGETS_FOLDER "CustomTargets")

option(ADS_FIXED_QUE "enable FixedQueue data type" OFF)
option(ADS_BINARY_TREE "enable binary tree Node and SNode data types" OFF)
option(ADS_MEMORY_ARENA "enable MemoryArena data types" OFF)

set(FQUE_INCLUDE
   "${CMAKE_CURRENT_SOURCE_DIR}/include/FixedQueue.h"
)
set(FQUE_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/FixedQueue.ipp"
)

add_library(${PROJECT_NAME} INTERFACE)

get_target_property(SRC ${PROJECT_NAME} SOURCES)

target_include_directories(${PROJECT_NAME} PUBLIC INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src")

if(${ADS_FIXED_QUE})
    source_group("FixedQueue/Include" FILES ${FQUE_INCLUDE})
    source_group("FixedQueue/Src" FILES ${FQUE_SRC})
endif()
