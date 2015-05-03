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
#include <OTL/Problem/Problem.h>
#include "Distance/Distance3.h"
#include "Shape/Shape4.h"

namespace otl
{
namespace problem
{
namespace zdt
{
template <typename _TReal, typename _TInteger>
class ZDT5 : public Problem<_TReal, std::vector<_TInteger> >
{
public:
	typedef _TReal TReal;
	typedef _TInteger TInteger;
	typedef std::vector<_TInteger> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;

	ZDT5(const size_t nDistDecisions = 10);
	~ZDT5(void);
	const std::vector<size_t> &GetDecisionBits(void) const;

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);

private:
	std::vector<size_t> decisionBits_;
};

template <typename _TReal, typename _TInteger>
ZDT5<_TReal, _TInteger>::ZDT5(const size_t nDistDecisions)
	: TSuper(2)
{
	assert(nDistDecisions >= 0);
	decisionBits_.resize(1 + nDistDecisions);
	decisionBits_[0] = 30;
	for (size_t i = 1; i < decisionBits_.size(); ++i)
		decisionBits_[i] = 5;
}

template <typename _TReal, typename _TInteger>
ZDT5<_TReal, _TInteger>::~ZDT5(void)
{
}

template <typename _TReal, typename _TInteger>
const std::vector<size_t> &ZDT5<_TReal, _TInteger>::GetDecisionBits(void) const
{
	return decisionBits_;
}

template <typename _TReal, typename _TInteger>
size_t ZDT5<_TReal, _TInteger>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal, typename _TInteger>
void ZDT5<_TReal, _TInteger>::_DoFix(std::vector<TReal> &objective)
{
}

template <typename _TReal, typename _TInteger>
void ZDT5<_TReal, _TInteger>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	objective.resize(TSuper::GetNumberOfObjectives());
	const TReal distance = distance::Distance3(decision.begin() + 1, decision.end());
	objective[0] = 1 + distance::NumberOfTrueBits(decision[0]);
	objective[1] = distance * shape::Shape4(objective[0]);
}
}
}
}
