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
#include <vector>
#include <utility>

namespace otl
{
namespace problem
{
namespace wfg
{
template <typename _TReal>
void Normalize(std::vector<_TReal> &decision)
{
	for(size_t i = 0; i < decision.size(); ++i)
	{
		const _TReal bound = 2 * (i + 1);
		assert(0 <= decision[i] && decision[i] <= bound);
		decision[i] = decision[i] / bound;
	}
}

template <typename _TReal>
void Scale(const _TReal distance, std::vector<_TReal> &objective)
{
	for(size_t i = 0; i < objective.size(); ++i)
		objective[i] = distance + 2 * (i + 1) * objective[i];
}
}
}
}
