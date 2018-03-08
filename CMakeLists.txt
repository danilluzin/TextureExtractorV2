cmake_minimum_required(VERSION 3.0)
project(texture_extractor)

set (CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE sources   TextureExtractorV2/*.cpp TextureExtractorV2/*.h TextureExtractorV2/*.hpp)
add_executable(tex_extract ${sources})
target_include_directories(tex_extract PUBLIC TextureExtractorV2)

SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/tbb)

find_package(GLM REQUIRED)
target_link_libraries(tex_extract PUBLIC  ${GLM_LIBRARIES})
include_directories(${GLM_INCLUDE_DIRS})
if(NOT GLM_FOUND)
        message(Error "GLM not found")
endif(NOT GLM_FOUND)

 
find_package(TBB REQUIRED)
target_link_libraries(tex_extract PUBLIC  ${TBB_LIBRARIES})
include_directories(${TBB_INCLUDE_DIRS})
if(NOT TBB_FOUND)
        message(Error "TBB not found")
endif(NOT TBB_FOUND)


find_package(OpenCV REQUIRED)
target_link_libraries(tex_extract PUBLIC  ${OpenCV_LIBRARIES})
include_directories(${OpenCV_INCLUDE_DIRS})
if(NOT OpenCV_FOUND)
        message(Error "OpenCV not found")
endif(NOT OpenCV_FOUND)

