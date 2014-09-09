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

function(list_projects ROOT_DIR)
	file(GLOB FILE_LIST RELATIVE "${ROOT_DIR}" *)
	unset(PROJECT_LIST)
	foreach(PROJECT_NAME ${FILE_LIST})
		set(PROJECT_PATH "${ROOT_DIR}/${PROJECT_NAME}")
		if(IS_DIRECTORY "${PROJECT_PATH}" AND EXISTS "${PROJECT_PATH}/CMakeLists.txt")
			set(PROJECT_LIST "${PROJECT_LIST}" ${PROJECT_NAME})
		endif()
	endforeach()
	set(PROJECT_LIST "${PROJECT_LIST}" PARENT_SCOPE)
endfunction()