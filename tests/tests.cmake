## Important variables ##
set(TEST_DIR "${PROJECT_SOURCE_DIR}/tests")

## Download and compile googltest - creduts to scrascit
add_subdirectory("${TEST_DIR}/googletest")

enable_testing()

## Create tests ##
file(GLOB TEST_FILES "${TEST_DIR}/src/*.cpp")
add_executable(mandelbrot_tests "${TEST_DIR}/main_tests.cpp" ${TEST_FILES})

## Link main lib and gtest ##
target_link_libraries(mandelbrot_tests mandelbroy_lib GTest::GTest GTest::Main)

## Add tests ##
add_test(mandelbrot mandelbrot_tests)
