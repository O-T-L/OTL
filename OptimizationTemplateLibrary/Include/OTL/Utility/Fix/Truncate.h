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
#include <OTL/Utility/WithBoundary.h>

namespace otl
{
namespace utility
{
namespace fix
{
template <typename _TCoordinate>
_TCoordinate Truncate(const _TCoordinate coordinate, const typename WithBoundary<_TCoordinate>::TRange &range)
{
	assert(range.first < range.second);
	if (coordinate < range.first)
		return range.first;
	else if (coordinate > range.second)
		return range.second;
	else
		return coordinate;
}
}
}
}
