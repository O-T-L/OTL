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
#include <OTL/Problem/Problem.h>

namespace otl
{
namespace problem
{
template <typename _TReal, typename _TDecision>
class DynamicProblem : public Problem<_TReal, _TDecision>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef Problem<TReal, TDecision> TSuper;

	DynamicProblem(const size_t nObjectives, const size_t &progress);
	~DynamicProblem(void);
	size_t GetProgress(void) const;

private:
	const size_t &progress_;
};

template <typename _TReal, typename _TDecision>
DynamicProblem<_TReal, _TDecision>::DynamicProblem(const size_t nObjectives, const size_t &progress)
	: TSuper(nObjectives)
	, progress_(progress)
{
}

template <typename _TReal, typename _TDecision>
DynamicProblem<_TReal, _TDecision>::~DynamicProblem(void)
{
}

template <typename _TReal, typename _TDecision>
size_t DynamicProblem<_TReal, _TDecision>::GetProgress(void) const
{
	return progress_;
}
}
}
