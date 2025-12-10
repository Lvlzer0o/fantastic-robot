cmake_minimum_required(VERSION 3.20)
project(fantastic_robot)

set(CMAKE_CXX_STANDARD 17)

# Add the executable target with its source file
add_executable(fantastic_robot main.cpp)

# Explicitly list header files for better IDE integration
target_sources(fantastic_robot PUBLIC
    ECGMath.hpp
    ECGMorphology.hpp
    HeartVectorAndLeads.hpp
)

# Ensure the headers are available in the include path
target_include_directories(fantastic_robot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
