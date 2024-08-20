# cmake_minimum_required(VERSION 3.0.2)
project(input_atas)
# set(Eigen3_DIR /usr/share/eigen3/cmake)
# link_directories( /usr/local/lib)
# link_directories( /usr/lib/aarch64-linux-gnu/tegra ) #to add nvlbuf_utils
# link_directories( /home/jetson/jetson-inference/ros/src)


include_directories(/home/rival/jetson-inference/utils/)
include_directories(/home/rival/jetson-inference/utils/video)
include_directories(/home/rival/jetson-inference/utils/image)
include_directories(/home/rival/jetson-inference/utils/network)
include_directories(/home/rival/jetson-inference/c)
include_directories(/home/rival/jetson-inference/c/tracking)
include_directories(/home/rival/jetson-inference/utils/cuda)

include_directories(/usr/local/cuda/include)
include_directories(/usr/local/include/jetson-utils)


# find_package( GStreamer MODULE REQUIRED COMPONENTS Video Audio Rtp )
# find_package(Eigen3 REQUIRED)
# find_package(orocos_kdl REQUIRED)
# find_package(jetson-utils)
# find_package(jetson-inference)

# CUDA is required
find_package(CUDA)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  romusa_messages
  # communication_manager
)

catkin_package(
 INCLUDE_DIRS include
 CATKIN_DEPENDS roscpp romusa_messages
#  LIBRARIES hexapod_controller
#  CATKIN_DEPENDS orocos_kdl phynix_messages roscpp
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
  ${orocos_kdl_INCLUDE_DIRS}
  ${jetson-inference_LIBRARIES}
  ${jetson-utils_LIBRARIES}
)



file(GLOB_RECURSE SOURCES "src/*.cpp")

file(GLOB detectnetSources *.cpp)
file(GLOB detectnetIncludes *.h )

# target_link_libraries(./src/objectDetect.cpp /home/jetson/jetson-inference/utils/)


# cuda_add_executable(${PROJECT_NAME} ${SOURCES})
cuda_add_executable(${PROJECT_NAME} src/input.cpp src/detectNet.cpp src/image_converter.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${catkin_INCLUDE_DIRS}
  # ${EIGEN3_INCLUDE_DIR}
  # ${orocos_kdl_INCLUDE_DIRS}
  ${jetson-inference_LIBRARIES}
  ${jetson-utils_LIBRARIES}


include
)

target_link_libraries(${PROJECT_NAME} 
        jetson-inference #add so source files get compiled
        jetson-utils
        ${catkin_LIBRARIES}
        ${EIGEN3_LIBRARIES}
        ${orocos_kdl_LIBRARIES}
        ${jetson-inference_LIBRARIES}
        ${jetson-utils_LIBRARIES}
)

install(TARGETS
${PROJECT_NAME} 
  RUNTIME DESTINATION lib/${PROJECT_NAME})
install(FILES ./src/ssd-mobilenet.onnx ./src/labels.txt
DESTINATION lib/${PROJECT_NAME})

# add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
#                    COMMAND ${CMAKE_COMMAND} -E make_directory
#                        ${catkin_workspace}/devel/lib/${PROJECT_NAME})

# add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
#                    COMMAND ${CMAKE_COMMAND} -E copy
#                        ${CMAKE_SOURCE_DIR}/src/ssd-mobilenet.onnx
#                        ${CMAKE_SOURCE_DIR}/src/labels.txt
#                        ${CMAKE_HOME_DIR}/devel/lib/${PROJECT_NAME})


add_dependencies(${PROJECT_NAME} romusa_messages_generate_messages_cpp)