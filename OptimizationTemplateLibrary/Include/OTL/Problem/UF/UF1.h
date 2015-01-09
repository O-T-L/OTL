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
#include "UF.h"
#include "Distance/Distance1.h"

namespace otl
{
namespace problem
{
namespace uf
{
template <typename _TReal>
class UF1 : public UF<_TReal>
{
public:
	typedef _TReal TReal;
	typedef UF<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;
	typedef typename TSuper::TRange TRange;
	typedef typename TSuper::TBoundary TBoundary;

	UF1(const size_t nDistDecisions = 29);
	~UF1(void);
	static TBoundary GenerateDecisionSpace(const size_t dimension);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);
};

template <typename _TReal>
UF1<_TReal>::UF1(const size_t nDistDecisions)
	: TSuper(2, GenerateDecisionSpace(1 + nDistDecisions))
{
}

template <typename _TReal>
UF1<_TReal>::~UF1(void)
{
}

template <typename _TReal>
typename UF1<_TReal>::TBoundary UF1<_TReal>::GenerateDecisionSpace(const size_t dimension)
{
	TBoundary boundary(dimension);
	boundary[0].first = 0;
	boundary[0].second = 1;
	for (size_t i = 1; i < boundary.size(); ++i)
	{
		boundary[i].first = -1;
		boundary[i].second = 1;
	}
	return boundary;
}

template <typename _TReal>
size_t UF1<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void UF1<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	assert(this->IsInside(decision));
	objective.resize(TSuper::GetNumberOfObjectives());
	const TReal x = decision[0];
	objective[0] = x + 2 * distance::Distance1(decision, 2);
	objective[1] = 1 - sqrt(x) + 2 * distance::Distance1(decision, 1);
}
}
}
}
