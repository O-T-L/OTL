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
#include "../Utility.h"

namespace otl
{
namespace problem
{
namespace wfg
{
namespace transform
{
namespace shift
{
template <typename _TReal>
_TReal Deceptive(const _TReal decision, const _TReal globalMin, const _TReal aperture, const _TReal deceptiveMin)
{
	assert(0 <= decision && decision <= 1);
	assert(0 < globalMin && globalMin < 1);
	assert(0 < aperture && aperture < 1);
	assert(0 < deceptiveMin && deceptiveMin < 1);
	assert(globalMin - aperture > 0);
	assert(globalMin + aperture < 1);
	const _TReal tmp1 = floor(decision - globalMin + aperture) * (1 - deceptiveMin + (globalMin - aperture) / aperture) / (globalMin - aperture);
	const _TReal tmp2 = floor(globalMin + aperture - decision) * (1 - deceptiveMin + (1 - globalMin - aperture) / aperture) / (1 - globalMin - aperture);
	return Fix<_TReal>(1 + (fabs(decision - globalMin) - aperture) * (tmp1 + tmp2 + 1 / aperture), 0, 1);
}
}
}
}
}
}
