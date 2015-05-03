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

#include "StdWFG.h"
#include "WFG6.h"
#include "WFG4.h"
#include "Utility.h"
#include "Transform/Reduction/WeightedSum.h"
#include "Transform/Bias/Dependent.h"
#include "Transform/Shift/Deceptive.h"
#include "Transform/Shift/MultiModal.h"
#include "Transform/Degenerate.h"
#include "Shape/Utility.h"

namespace otl
{
namespace problem
{
namespace wfg
{
template <typename _TReal>
class WFG9 : public StdWFG<_TReal>
{
public:
	typedef _TReal TReal;
	typedef StdWFG<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;

	WFG9(const size_t nObjectives, const size_t posGroups = 2, const size_t distDecisions = 20);
	~WFG9(void);
	static void Transition1(TDecision &decision);
	static void Transition2(TDecision &decision, const size_t nPosDecisions);
	static TReal Transition3(const TDecision &decision, const size_t nPosDecisions, TDecision &posDecision);
	static void Shape(const TDecision &decision, const _TReal distance, std::vector<TReal> &objective);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);
};

template <typename _TReal>
WFG9<_TReal>::WFG9(const size_t nObjectives, const size_t posGroups, const size_t distDecisions)
	: TSuper(nObjectives, posGroups, distDecisions)
{
}

template <typename _TReal>
WFG9<_TReal>::~WFG9(void)
{
}

template <typename _TReal>
void WFG9<_TReal>::Transition1(TDecision &decision)
{
	TDecision weight(decision.size(), 1);
	for(size_t i = 0; i < decision.size() - 1; ++i)
	{
		const size_t begin = i + 1;
		const TReal regionMapping = transform::reduction::WeightedSum<TReal>(weight.begin() + begin, weight.end(), decision.begin() + begin);
		decision[i] = transform::bias::Dependent<TReal>(decision[i], regionMapping, 0.98 / 49.98, 0.02, 50);
	}
}

template <typename _TReal>
void WFG9<_TReal>::Transition2(TDecision &decision, const size_t nPosDecisions)
{
	assert(0 < nPosDecisions && nPosDecisions <= decision.size());
	for(size_t i = 0; i < nPosDecisions; ++i)
		decision[i] = transform::shift::Deceptive<TReal>(decision[i], 0.35, 0.001, 0.05);
	for(size_t i = nPosDecisions; i < decision.size(); ++i)
		decision[i] = transform::shift::MultiModal<TReal>(decision[i], 30, 95, 0.35);
}

template <typename _TReal>
_TReal WFG9<_TReal>::Transition3(const TDecision &decision, const size_t nPosDecisions, TDecision &posDecision)
{
	return WFG6<TReal>::Transition2(decision, nPosDecisions, posDecision);
}

template <typename _TReal>
void WFG9<_TReal>::Shape(const TDecision &decision, const _TReal distance, std::vector<TReal> &objective)
{
	WFG4<TReal>::Shape(decision, distance, objective);
}

template <typename _TReal>
size_t WFG9<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void WFG9<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	assert(this->IsInside(decision));
	objective.resize(TSuper::GetNumberOfObjectives());
	TDecision _decision = decision;
	Normalize(_decision);
	Transition1(_decision);
	Transition2(_decision, TSuper::GetPosDecisions());
	TDecision posDecision(TSuper::GetNumberOfObjectives() - 1);
	TReal distance = Transition3(_decision, TSuper::GetPosDecisions(), posDecision);
	Shape(posDecision, distance, objective);
}
}
}
}
