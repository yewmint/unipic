cmake_minimum_required (VERSION 3.4)
project (unipic CXX)

# set(CMAKE_C_COMPILE_OBJECT TRUE)
add_definitions("-Wall -std=c++17")
file(GLOB SOURCES "src/*.cpp")

# import opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# use boost-cmake to import boost
add_subdirectory(boost-cmake)

# use TBBGet to import TBB
set(TBB_ROOT ${PROJECT_SOURCE_DIR}/tbb)
include(${TBB_ROOT}/cmake/TBBBuild.cmake)
tbb_build(TBB_ROOT ${TBB_ROOT} CONFIG_DIR TBB_DIR)
find_package(TBB REQUIRED)
include_directories(${TBB_ROOT}/include)

# import spdlog header-only library
include_directories(spdlog/include)

# import pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# import catch 2
include_directories(Catch2/single_include)

# import sqlite3
add_library(sliqte3 sqlite3/sqlite3.c)
include_directories(sqlite3)

# target for normal usage
add_executable(unipic ${SOURCES})
target_link_libraries(unipic ${OpenCV_LIBS})
target_link_libraries(unipic Threads::Threads)
target_link_libraries(unipic Boost::filesystem)
target_link_libraries(unipic ${TBB_IMPORTED_TARGETS})
target_link_libraries(unipic sqlite3)

# target for unit test using catch 2
add_executable(unipic_test ${SOURCES})
target_link_libraries(unipic_test ${OpenCV_LIBS})
target_link_libraries(unipic_test Threads::Threads)
target_link_libraries(unipic_test Boost::filesystem)
target_link_libraries(unipic_test ${TBB_IMPORTED_TARGETS})
target_link_libraries(unipic_test sqlite3)
target_compile_definitions(unipic_test PUBLIC GENERATE_UNIT_TEST)

install(TARGETS unipic RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)
install(TARGETS unipic_test RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)
