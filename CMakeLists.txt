cmake_minimum_required (VERSION 3.4)
project (unipic CXX)

# set(CMAKE_C_COMPILE_OBJECT TRUE)
add_definitions("-Wall -std=c++17")
file(GLOB SOURCES "src/*.cpp")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# use boost-cmake to import boost
add_subdirectory(boost-cmake)

# use TBBGet to import TBB
include(cmake/TBBGet.cmake)
tbb_get(TBB_ROOT tbb_root CONFIG_DIR TBB_DIR)
find_package(TBB REQUIRED)
include_directories(${tbb_root}/include)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# link sqlite3
link_directories(/usr/local/lib)
include_directories(${PROJECT_SOURCE_DIR}/lib)

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