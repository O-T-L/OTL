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
#include <OTL/Problem/DTLZ/Transform/Bias.h>
#include <OTL/Problem/DTLZ/Shape/Utility.h>
#include <OTL/Problem/DTLZ/Shape/Concave.h>
#include "StdFDA.h"
#include "Distance/Distance2.h"

namespace otl
{
namespace problem
{
namespace fda
{
template <typename _TReal>
class FDA5 : public StdFDA<_TReal>
{
public:
	typedef _TReal TReal;
	typedef StdFDA<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;

	FDA5(const size_t nObjectives, const size_t &progress, const size_t nFixedSteps = 5, const size_t nDistinctSteps = 10, const size_t distDecisions = 10);
	~FDA5(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);
};

template <typename _TReal>
FDA5<_TReal>::FDA5(const size_t nObjectives, const size_t &progress, const size_t nFixedSteps, const size_t nDistinctSteps, const size_t distDecisions)
	: TSuper(nObjectives, progress, nFixedSteps, nDistinctSteps, distDecisions)
{
}

template <typename _TReal>
FDA5<_TReal>::~FDA5(void)
{
}

template <typename _TReal>
size_t FDA5<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void FDA5<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	objective.resize(TSuper::GetNumberOfObjectives());
	const size_t posDecisions = TSuper::GetNumberOfObjectives() - 1;
	const TReal cycle = sin(TSuper::CalculateEnvironment() * boost::math::constants::pi<_TReal>() / 2);
	const TReal radius = 1 + distance::Distance2(decision, objective.size(), std::fabs(cycle));
	const TReal bias = 1 + 100 * pow(cycle, 4);
	TDecision angles(decision.begin(), decision.begin() + posDecisions);
	dtlz::transform::Bias(angles.begin(), angles.end(), bias);
	dtlz::shape::ConvertAngles<TReal>(angles.begin(), angles.end());
	dtlz::shape::Concave(angles.begin(), angles.end(), objective.begin(), objective.end(), radius);
}
}
}
}
