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
    include/FixedQueue.h
)
set(FQUE_SRC
    src/FixedQueue.ipp
)

set(BTREE_INCLUDE
    include/BinaryTree.h
)
set(BTREE_SRC
    src/BinaryTree.ipp
)

set(MARENA_INCLUDE
    include/Arena.h
)
set(MARENA_SRC
    src/ModArena.cpp
    src/ModArena.ipp
    src/StaticArena.cpp 
    src/StaticArena.ipp 
    src/ArenaPtr.ipp 
)

# only generate library if non header only implementations are selected
if(${ADS_MEMORY_ARENA})
    add_library(${PROJECT_NAME} STATIC
        $<IF:$<BOOL:${ADS_FIXED_QUE}>,${FQUE_INCLUDE} ${FQUE_SRC}, >
        $<IF:$<BOOL:${ADS_BINARY_TREE}>,${BTREE_INCLUDE} ${BTREE_SRC}, >
        $<IF:$<BOOL:${ADS_MEMORY_ARENA}>,${MARENA_INCLUDE} ${MARENA_SRC}, >
    )

    target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    )
else()

add_library(${PROJECT_NAME} INTERFACE
$<IF:$<BOOL:${ADS_FIXED_QUE}>,${FQUE_INCLUDE} ${FQUE_SRC}, >
$<IF:$<BOOL:${ADS_BINARY_TREE}>,${BTREE_INCLUDE} ${BTREE_SRC}, >
)

target_include_directories(${PROJECT_NAME} INTERFACE
${CMAKE_SOURCE_DIR}/include
)
endif()

if(${ADS_FIXED_QUE})
    source_group("FixedQueue/Include" FILES ${FQUE_INCLUDE})
    source_group("FixedQueue/Src" FILES ${FQUE_SRC})
endif()

if(${ADS_BINARY_TREE})
    source_group("BinaryTree/Include" FILES ${BTREE_INCLUDE})
    source_group("BinaryTree/Src" FILES ${BTREE_SRC})
endif()

if(${ADS_BINARY_TREE})
    source_group("MemoryArena/Include" FILES ${MARENA_INCLUDE})
    source_group("MemoryArena/Src" FILES ${MARENA_SRC})
endif()
