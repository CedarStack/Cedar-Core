# Copyright (C) 2024 Cedar Community
# This file is part of Cedar-Core, distributed under the MIT License.
# See the LICENSE file in the project root for full license information.

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.0.zip
)
FetchContent_MakeAvailable(googletest)

#add_compile_options(-fsanitize=address)
#link_libraries(-fsanitize=address)

file(GLOB_RECURSE TEST_SOURCES "./*.cpp")

foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source})
    target_link_libraries(${test_name} gtest_main Cedar)
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

enable_testing()
