# Copyright (C) 2014, 申瑞珉 (Ruimin Shen)
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/Modules")
include(list_projects)

project(OptimizationTemplateLibrary) # special project name
include_directories("${CMAKE_SOURCE_DIR}/Include")
add_subdirectory("${CMAKE_SOURCE_DIR}/Include")
add_subdirectory("${CMAKE_SOURCE_DIR}/Tests")
add_subdirectory("${CMAKE_SOURCE_DIR}/Projects")

option(BUILD_SHARED_LIBS "Build shared libraries" NO)

option(OTL_BUILD_DOCUMENTATION "Build OTL documents" NO)
if(OTL_BUILD_DOCUMENTATION)
	add_subdirectory("${CMAKE_SOURCE_DIR}/Document")
	install(DIRECTORY "${CMAKE_BINARY_DIR}/doc" DESTINATION doc)
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()