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
#include <cmath>
#include <vector>
#include <boost/math/constants/constants.hpp>

namespace otl
{
namespace problem
{
namespace wfg
{
namespace shape
{
template <typename _TReal>
_TReal Disconnected(const _TReal decision, const size_t nRegions, const _TReal shape, const _TReal location)
{
	assert(nRegions > 0);
	assert(shape > 0);
	assert(location > 0);
	assert(0 <= decision && decision <= 1);
	const _TReal tmp = nRegions * pow(decision, location) * boost::math::constants::pi<_TReal>();
	return 1 - pow(decision, shape) * pow(cos(tmp), 2);
}
}
}
}
}
