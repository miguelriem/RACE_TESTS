cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#Graphviz
find_path(GRAPHVIZ_INCLUDE_DIR graphviz/gvc.h)
find_library(GRAPHVIZ_GVC_LIBRARY NAMES gvc )
find_library(GRAPHVIZ_GRAPH_LIBRARY NAMES graph )
find_package(GTK2 2.6 REQUIRED gtk)

#set(DO_MHT true)

if(${GRAPHVIZ_INCLUDE_DIR} STREQUAL GRAPHVIZ_INCLUDE_DIR-NOTFOUND )
	message(WARNING "Graphviz is not installed please do: sudo apt-get
			install libgraphviz-dev")
endif()

if(${GRAPHVIZ_GVC_LIBRARY} STREQUAL GRAPHVIZ_GVC_LIBRARY-NOTFOUND )
	message(WARNING "GVC library is not installed please do: sudo apt-get
			install libgraphviz-dev")
endif()

if(${GRAPHVIZ_GRAPH_LIBRARY} STREQUAL GRAPHVIZ_GRAPH_LIBRARY-NOTFOUND )
	message(WARNING "Graph library is not installed please do: sudo apt-get
			install libgraphviz-dev")
endif()

if(NOT GTK2_FOUND)
	message(WARNING "Gtk library is not installed please do: sudo apt-get
			install libgtk2.0-dev")
endif()


#uncomment if you have defined messages
#rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
#rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
#rosbuild_add_executable(example examples/example.cpp)
#target_link_libraries(example ${PROJECT_NAME})


rosbuild_add_executable(test_graphviz src/main.cpp)

SET(XGTK_PLUGIN gvplugin_xgtk)
include_directories(${GTK2_INCLUDE_DIRS})
include_directories(${GRAPHVIZ_INCLUDE_DIR})

rosbuild_add_library(${XGTK_PLUGIN} src/gvplugin_xgtk.cpp src/callbacks_mht.cpp	src/graph_wrapper.cpp) #src/hypothesisTree.cpp)
target_link_libraries(${XGTK_PLUGIN} ${GRAPHVIZ_GVC_LIBRARY} ${GRAPHVIZ_GRAPH_LIBRARY} ${GTK2_LIBRARIES})


target_link_libraries(test_graphviz ${XGTK_PLUGIN})

#rosbuild_add_library(munkres src/munkres.cpp src/matrix.cpp
#src/k_best.cpp)

#rosbuild_add_executable(mht src/mht.cpp src/point.cpp
#src/measurement.cpp src/target.cpp src/metrics.cpp
#src/motionModel.cpp src/hypothesis.cpp
#src/cluster.cpp src/hypothesisTree.cpp
#src/mht_implementation.cpp)
