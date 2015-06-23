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
_TReal Flat(const _TReal decision, const _TReal region, const _TReal regionMin, const _TReal regionMax)
{
	assert(0 <= decision && decision <= 1);
	assert(0 <= region && region <= 1);
	assert(0 <= regionMin && regionMin <= 1);
	assert(0 <= regionMax && regionMax <= 1);
	assert(regionMin < regionMax);
	assert(regionMin != 0 || region == 0);
	assert(regionMin != 0 || regionMax != 1);
	assert(regionMax != 1 || region == 1);
	assert(regionMax != 1 || regionMin != 0);
	const _TReal tmp1 = std::min((_TReal)0, floor(decision - regionMin)) * region * (regionMin - decision) / regionMin;
	const _TReal tmp2 = std::min((_TReal)0, floor(regionMax - decision)) * (1 - region) * (decision - regionMax) / (1 - regionMax);
	return Fix<_TReal>(region + tmp1 - tmp2, 0, 1);
}
}
}
}
}
}
