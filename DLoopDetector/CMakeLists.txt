cmake_minimum_required(VERSION 3.0)
project(DLoopDetector)
include(ExternalProject)

option(BUILD_DemoBRIEF  "Build demo application with BRIEF features" ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(HDRS
  include/DLoopDetector/DLoopDetector.h         include/DLoopDetector/TemplatedLoopDetector.h)

set(DEPENDENCY_DIR ${CMAKE_CURRENT_BINARY_DIR}/dependencies)
set(DEPENDENCY_INSTALL_DIR ${DEPENDENCY_DIR}/install)
set(DEPENDENCY_CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
if(DEFINED OpenCV_DIR)
    set(DEPENDENCY_CMAKE_ARGS ${DEPENDENCY_CMAKE_ARGS} -DOpenCV_DIR=${OpenCV_DIR})
endif()

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(include/DLoopDetector/)

if(BUILD_DemoBRIEF)
  add_executable(demo_brief demo/demo_brief.cpp)
  target_link_libraries(demo_brief ${OpenCV_LIBS} DLib DBoW2)
  target_include_directories(demo_brief PUBLIC ${DLib_INCLUDE_DIRS} ${DBoW2_INCLUDE_DIRS})
  set_target_properties(demo_brief PROPERTIES CXX_STANDARD 11)
endif(BUILD_DemoBRIEF)

#if(BUILD_DemoSURF)
#  add_executable(demo_surf demo/demo_surf.cpp)
#  target_link_libraries(demo_surf ${OpenCV_LIBS} ${DLib_LIBS} ${DBoW2_LIBS})
#endif(BUILD_DemoSURF)

if(BUILD_DemoBRIEF)
  set(RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/resources/)
  set(RESOURCE_FILE ${CMAKE_CURRENT_BINARY_DIR}/resources.tar.gz)
  if(NOT EXISTS ${RESOURCE_DIR})
    if(EXISTS ${RESOURCE_FILE})
      execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${RESOURCE_FILE})
    else()
      message("To run the demo application, please download \
https://drive.google.com/uc?export=download&id=1MpZwPjXDAUxKfSTpeCjG0PAUpaeWuo7D \
into ${RESOURCE_FILE} and run cmake again.")
    endif()
  endif(NOT EXISTS ${RESOURCE_DIR})
endif(BUILD_DemoBRIEF)

configure_file(src/DLoopDetector.cmake.in
  "${PROJECT_BINARY_DIR}/DLoopDetectorConfig.cmake" @ONLY)

install(DIRECTORY include/DLoopDetector DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/DLoopDetectorConfig.cmake"
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})
install(FILES "${PROJECT_BINARY_DIR}/DLoopDetectorConfig.cmake" 
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/DLoopDetector/)
install(DIRECTORY ${DEPENDENCY_INSTALL_DIR}/ DESTINATION ${CMAKE_INSTALL_PREFIX} OPTIONAL)
