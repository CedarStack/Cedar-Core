# Copyright (C) 2024 Cedar Community
# This file is part of Cedar-Core, distributed under the MIT License.
# See the LICENSE file in the project root for full license information.

target_sources(Cedar PRIVATE
        BasicHashDefinitions.cpp
        Exception.cpp
        String.cpp
        Memory.cpp
)

add_subdirectory(IO)
add_subdirectory(Math)
add_subdirectory(Text)
add_subdirectory(Threading)
