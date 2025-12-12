cmake_minimum_required(VERSION 3.20)
project(fantastic_robot)

# JSF-AV standards often imply C++03, but the use of `auto` and other features
# in the original code suggests a modern standard was intended. We will use C++17
# but note that a strict audit might require an older standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add the executable target with all its source files
add_executable(fantastic_robot
    main.cpp
    ECGMath.cpp
    ECGMorphology.cpp
    ECGSimulation.cpp
)

# Explicitly list header files for IDE integration and clarity
target_sources(fantastic_robot PUBLIC
    Types.h
    ECGMath.h
    ECGMorphology.h
    ECGSimulation.h
)

# Per Rule 33, includes should use <>, so we add the project directory
# to the include path.
target_include_directories(fantastic_robot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(ecg_tests
    ECGMathTests.cpp
    ECGMorphology.cpp
    ECGMath.cpp
    ECGSimulation.cpp
)

target_link_libraries(ecg_tests
    GTest::gtest_main
)

target_include_directories(ecg_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

include(GoogleTest)
gtest_discover_tests(ecg_tests)
