/*!
Copyright (C) 2014, 申瑞珉 (Ruimin Shen)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <cassert>

namespace otl
{
namespace utility
{
namespace fix
{
template <typename _TCoordinate>
_TCoordinate Truncate(const _TCoordinate coordinate, const _TCoordinate lower, const _TCoordinate upper)
{
	assert(lower < upper);
	if (coordinate < lower)
		return lower;
	else if (coordinate > upper)
		return upper;
	else
		return coordinate;
}
}
}
}
