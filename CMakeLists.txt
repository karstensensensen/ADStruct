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

add_executable(${PROJECT_NAME}
    src/main.cpp
    $<IF:$<BOOL:${ADS_FIXED_QUE}>,${FQUE_INCLUDE} ${FQUE_SRC}, >
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

if(${ADS_FIXED_QUE})
    source_group("FixedQueue/Include" ${FQUE_INCLUDE})
    source_group("FixedQueue/Src" ${FQUE_SRC})
endif()
