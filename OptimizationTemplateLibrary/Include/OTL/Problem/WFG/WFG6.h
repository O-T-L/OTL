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
#include "WFG1.h"
#include "Utility.h"
#include "Transform/Reduction/NonSeparable.h"
#include "Transform/Degenerate.h"
#include "Shape/Utility.h"
#include "Shape/InvertConcave.h"

namespace otl
{
namespace problem
{
namespace wfg
{
template <typename _TReal>
class WFG6 : public StdWFG<_TReal>
{
public:
	typedef _TReal TReal;
	typedef StdWFG<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;

	WFG6(const size_t nObjectives, const size_t posGroups = 2, const size_t distDecisions = 20);
	~WFG6(void);
	static void Transition1(TDecision &decision, const size_t nPosDecisions);
	static TReal Transition2(const TDecision &decision, const size_t nPosDecisions, TDecision &posDecision);
	static void Shape(const TDecision &decision, const _TReal distance, std::vector<TReal> &objective);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);
};

template <typename _TReal>
WFG6<_TReal>::WFG6(const size_t nObjectives, const size_t posGroups, const size_t distDecisions)
	: TSuper(nObjectives, posGroups, distDecisions)
{
}

template <typename _TReal>
WFG6<_TReal>::~WFG6(void)
{
}

template <typename _TReal>
void WFG6<_TReal>::Transition1(TDecision &decision, const size_t nPosDecisions)
{
	return WFG1<TReal>::Transition1(decision, nPosDecisions);
}

template <typename _TReal>
_TReal WFG6<_TReal>::Transition2(const TDecision &decision, const size_t nPosDecisions, TDecision &posDecision)
{
	assert(0 < nPosDecisions && nPosDecisions <= decision.size());
	assert(posDecision.size() > 0);
	assert(nPosDecisions >= posDecision.size() && nPosDecisions % posDecision.size() == 0);
	const size_t nDistDecisions = decision.size() - nPosDecisions;

	const size_t nUnit = nPosDecisions / posDecision.size();
	for(size_t i = 0; i < posDecision.size(); ++i)
	{
		const size_t begin = i * nUnit;
		const size_t end = (i + 1) * nUnit;
		posDecision[i] = transform::reduction::NonSeparable<TReal>(decision.begin() + begin, decision.begin() + end, nUnit);
	}
	if (nDistDecisions > 0)
		return transform::reduction::NonSeparable<TReal>(decision.begin() + nPosDecisions, decision.end(), decision.size() - nPosDecisions);
	else
		return 0;
}

template <typename _TReal>
void WFG6<_TReal>::Shape(const TDecision &decision, const _TReal distance, std::vector<TReal> &objective)
{
	assert(decision.size() > 0);
	TDecision angle = decision;
	const std::vector<TReal> degenerate(angle.size(), 1);
	transform::Degenerate(angle, degenerate, distance);
	shape::ConvertAngles<TReal>(angle.begin(), angle.end());
	shape::InvertConcave(angle.begin(), angle.end(), objective.begin(), objective.end(), (TReal)1);
	Scale(distance, objective);
}

template <typename _TReal>
size_t WFG6<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void WFG6<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	objective.resize(TSuper::GetNumberOfObjectives());
	TDecision _decision = decision;
	Normalize(_decision);
	Transition1(_decision, TSuper::GetPosDecisions());
	TDecision posDecision(TSuper::GetNumberOfObjectives() - 1);
	TReal distance = Transition2(_decision, TSuper::GetPosDecisions(), posDecision);
	Shape(posDecision, distance, objective);
}
}
}
}
