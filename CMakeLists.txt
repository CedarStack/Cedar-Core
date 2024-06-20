# Copyright (C) 2024 Cedar Community
# This file is part of Cedar-Core, distributed under the MIT License.
# See the LICENSE file in the project root for full license information.

cmake_minimum_required(VERSION 3.22)

project(Cedar VERSION 0.1 DESCRIPTION "Cedar Core")

set(CMAKE_CXX_STANDARD 17)

include_directories(./include)
add_library(Cedar STATIC "")
add_subdirectory(src)
target_link_libraries(Cedar PUBLIC pthread)
add_subdirectory(test)
