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
file(GLOB_RECURSE SOURCE_LIST RELATIVE "${PROJECT_DIR}" *.h *.c *.cpp)
add_library(${PROJECT_NAME} ${SOURCE_LIST})

FIND_PACKAGE(Boost)
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()

file(GLOB_RECURSE HEADER_LIST RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.h)
foreach(HEADER ${HEADER_LIST})
	string(REGEX MATCH "(.*)[/\\]" PARENT_DIR "${HEADER}")
	install(FILES "${HEADER}" DESTINATION "include/${PROJECT_NAME}/${PARENT_DIR}") 
endforeach()
install(TARGETS ${PROJECT_NAME}
	ARCHIVE DESTINATION "lib${LIB_SUFFIX}"
	LIBRARY DESTINATION "lib${LIB_SUFFIX}"
	RUNTIME DESTINATION "lib${LIB_SUFFIX}"
)