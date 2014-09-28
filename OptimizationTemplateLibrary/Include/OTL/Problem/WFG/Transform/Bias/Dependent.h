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
namespace bias
{
template <typename _TReal>
_TReal Dependent(const _TReal decision, const _TReal regionMapping, const _TReal regionFraction, const _TReal regionMin, const _TReal regionMax)
{
	assert(0 <= decision && decision <= 1);
	assert(0 <= regionMapping && regionMapping <= 1);
	assert(0 < regionFraction && regionFraction < 1);
	assert(regionMin > 0);
	assert(regionMin < regionMax);
	const _TReal tmp = regionFraction - (1 - 2 * regionMapping) * std::fabs(floor(0.5 - regionMapping) + regionFraction);
	return Fix<_TReal>(pow(decision, regionMin + (regionMax - regionMin) * tmp), 0, 1);
}
}
}
}
}
}
