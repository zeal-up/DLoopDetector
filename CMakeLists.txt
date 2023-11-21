cmake_minimum_required(VERSION 3.0)
project(ORB_DBoW2)
include(ExternalProject)

add_subdirectory(DBoW2)
add_subdirectory(DLib)
add_subdirectory(DLoopDetector)


set(RESOURCE_FILE_NAME resources.tar.gz)
set(RESOURCE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE_FILE_NAME})
set(RESOURCE_COPY_PATH ${CMAKE_CURRENT_BINARY_DIR}/${RESOURCE_FILE_NAME})
set(EXTRACT_DIR ${CMAKE_CURRENT_BINARY_DIR})

# copy resource file to RESOUTCE_COPY_PATH
configure_file(${RESOURCE_FILE} ${RESOURCE_COPY_PATH} COPYONLY)

execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${RESOURCE_COPY_PATH} WORKING_DIRECTORY ${EXTRACT_DIR})