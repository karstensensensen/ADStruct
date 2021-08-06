cmake_minimum_required(VERSION 3.0.0)
Project(ADStruct)

set(CMAKE_CXX_STANDARD 20)

# place all predefined target projects into a filter
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PREDEFINED_TARGETS_FOLDER "CustomTargets")

option(ADS_FIXED_QUE "enable FixedQueue data type" ON)
option(ADS_BINARY_TREE "enable binary tree Node and SNode data types" ON)

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

add_library(${PROJECT_NAME} INTERFACE
    $<IF:$<BOOL:${ADS_FIXED_QUE}>,${FQUE_INCLUDE} ${FQUE_SRC}, >
    $<IF:$<BOOL:${ADS_BINARY_TREE}>,${BTREE_INCLUDE} ${BTREE_SRC}, >
)

target_include_directories(${PROJECT_NAME} INTERFACE
    ${CMAKE_SOURCE_DIR}/include
)

if(${ADS_FIXED_QUE})
    source_group("FixedQueue/Include" ${FQUE_INCLUDE})
    source_group("FixedQueue/Src" ${FQUE_SRC})

    source_group("BinaryTree/Include" ${BTREE_INCLUDE})
    source_group("BinaryTree/Src" ${BTREE_SRC})
endif()
