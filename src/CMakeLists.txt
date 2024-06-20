# Copyright (C) 2024 Cedar Community
# This file is part of Cedar-Core, distributed under the MIT License.
# See the LICENSE file in the project root for full license information.

target_sources(Cedar PRIVATE
        Exception.cpp
        String.cpp
        Memory.cpp
)

add_subdirectory(Threading)