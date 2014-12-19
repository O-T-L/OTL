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
#include "DTLZ2.h"

namespace otl
{
namespace problem
{
namespace dtlz
{
template <typename _TReal>
class NegativeDTLZ2 : public DTLZ2<_TReal>
{
public:
	typedef _TReal TReal;
	typedef DTLZ2<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;
	typedef typename TSuper::TMinMax TMinMax;
	typedef typename TSuper::TBoundary TBoundary;

	NegativeDTLZ2(const size_t nObjectives, const size_t distDecisions = 10);
	~NegativeDTLZ2(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);
	void _DoFix(std::vector<TReal> &objective);

private:
	const TReal maxDistance_;
};

template <typename _TReal>
NegativeDTLZ2<_TReal>::NegativeDTLZ2(const size_t nObjectives, const size_t distDecisions)
	: TSuper(nObjectives, distDecisions)
	, maxDistance_(1 + 0.5 * 0.5 * distDecisions)
{
}

template <typename _TReal>
NegativeDTLZ2<_TReal>::~NegativeDTLZ2(void)
{
}

template <typename _TReal>
size_t NegativeDTLZ2<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void NegativeDTLZ2<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	TSuper::_Evaluate(decision, objective);
	for (size_t i = 0; i < objective.size(); ++i)
	{
		objective[i] -= maxDistance_;
		assert(objective[i] <= 0);
	}
}

template <typename _TReal>
void NegativeDTLZ2<_TReal>::_DoFix(std::vector<TReal> &objective)
{
	for (size_t i = 0; i < objective.size(); ++i)
	{
		objective[i] += maxDistance_;
		assert(objective[i] >= 0);
	}
}
}
}
}
