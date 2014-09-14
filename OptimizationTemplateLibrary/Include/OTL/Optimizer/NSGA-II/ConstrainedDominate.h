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

#include <vector>
#include <cassert>
#include <OTL/Solution.h>
#include <OTL/Utility/Relation/Pareto.h>

namespace otl
{
namespace optimizer
{
namespace nsga_ii
{
template <typename _TReal>
_TReal InequalityViolation(const std::vector<_TReal> &inequality)
{
	_TReal violation = 0;
	for (size_t i = 0; i < inequality.size(); ++i)
	{
		if (inequality[i] < 0)
			violation += inequality[i];
	}
	return violation;
}

template <typename _TReal>
_TReal EqualityViolation(const std::vector<_TReal> &equality)
{
	_TReal violation = 0;
	for (size_t i = 0; i < equality.size(); ++i)
	{
		if (equality[i] != 0)
			violation += std::fabs(equality[i]);
	}
	return violation;
}

template <typename _TReal, typename _TDecision>
bool ConstrainedDominate(const otl::Solution<_TReal, _TDecision> &solution1, const otl::Solution<_TReal, _TDecision> &solution2)
{
	const _TReal violation1 = InequalityViolation(solution1.inequality_);
	if (violation1 >= 0)
	{
		const _TReal violation2 = InequalityViolation(solution2.inequality_);
		if (violation2 >= 0)
			return otl::utility::relation::Dominate(solution1.objective_, solution2.objective_);
		else
			return true;
	}
	else
	{
		const _TReal violation2 = InequalityViolation(solution2.inequality_);
		if (violation2 >= 0)
			return false;
		else
			return violation1 < violation2;
	}
}
}
}
}
