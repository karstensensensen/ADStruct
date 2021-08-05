cmake_minimum_required(VERSION 3.0.0)
Project(ADStruct)

set(CMAKE_CXX_STANDARD 17)

# place all predefined target projects into a filter
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PREDEFINED_TARGETS_FOLDER "CustomTargets")

option(ADS_FIXED_QUE "enable FixedQueue data type" ON)

set(FQUE_INCLUDE
    include/FixedQueue.h
)

set(FQUE_SRC
    src/FixedQueue.ipp
)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<IF:$<BOOL:${ADS_FIXED_QUE}>,${FQUE_INCLUDE} ${FQUE_SRC}, >
)
