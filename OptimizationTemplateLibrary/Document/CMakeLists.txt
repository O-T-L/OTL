###
# Copyright (c) 2014, 申瑞珉 (Ruimin Shen)
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice, this
#	list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#	this list of conditions and the following disclaimer in the documentation
#	and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

create_project("${CMAKE_CURRENT_SOURCE_DIR}")
project(${PROJECT_NAME})

FIND_PACKAGE(Doxygen)
if(DOXYGEN_FOUND)
	file(GLOB DOXYFILE_LIST RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.doxyfile)
	foreach(DOXYFILE_NAME "${DOXYFILE_LIST}")
		get_filename_component(DOXYFILE_TITLE "${DOXYFILE_NAME}" NAME_WE)
		configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${DOXYFILE_NAME}" "${PROJECT_BINARY_DIR}/${DOXYFILE_NAME}" @ONLY IMMEDIATE)
		add_custom_target("${DOXYFILE_TITLE}" ALL COMMAND "${DOXYGEN_EXECUTABLE}" "${PROJECT_BINARY_DIR}/${DOXYFILE_NAME}" SOURCES "${PROJECT_BINARY_DIR}/${DOXYFILE_NAME}")
	endforeach()
endif()