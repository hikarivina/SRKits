# SRKits - An unofficial C/C++ API Wrapper for Silkroad Online.
# Copyright (C) 2019  Abdelhalem Samy (a.k.a. Klito)

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License and/or GNU Affero General Public License 
# as published by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License and/or GNU Affero General Public License for more details.

# You should have received a copy of the GNU General Public License and/or GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.13)
project(SRKits)

# SET C++ STANDARD VERSION TO: C++14
set(CMAKE_CXX_STANDARD 14)

# Windows (MSVC) Minimum Required Version
if (MSVC_TOOLSET_VERSION LESS 141)
    message(FATAL_ERROR "MSVC minimun required is 14.1 (a.k.a. Visual Studio 2017 (15.0))")
endif()

# Windows (MSVC) Definitions
if (MSVC)
    add_compile_definitions(_WIN32_WINNT=0x0601) # Target Windows 7
    add_compile_definitions(WIN32_LEAN_AND_MEAN) # Exclude APIs such as Cryptography, DDE, RPC, Shell, and Windows Sockets.
    # Speed up the compilerion time in Windows (MSVC) 
    message(STATUS "MSVC version: ${MSVC_VERSION}, toolset version: ${MSVC_TOOLSET_VERSION}")
endif()

# SET BOOST
set(BOOST_INCLUDE_DIR  "${CMAKE_SOURCE_DIR}/third-party/boost_1_70_0")
set(BOOST_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/third-party/boost_1_70_0/lib")
find_package(BOOST 1.70 REQUIRED)
include_directories(${BOOST_INCLUDE_DIR})
link_directories(${BOOST_LIBRARY_DIR})

include_directories("${CMAKE_SOURCE_DIR}/srkits/include")

add_subdirectory(srkits)
