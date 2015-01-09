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
#include <vector>
#include "UF.h"
#include "Distance/Distance7.h"

namespace otl
{
namespace problem
{
namespace uf
{
template <typename _TReal>
class UF7 : public UF<_TReal>
{
public:
	typedef _TReal TReal;
	typedef UF<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;
	typedef typename TSuper::TRange TRange;
	typedef typename TSuper::TBoundary TBoundary;

	UF7(const size_t nDistDecisions = 29);
	~UF7(void);
	static TBoundary GenerateDecisionSpace(const size_t dimension);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);
};

template <typename _TReal>
UF7<_TReal>::UF7(const size_t nDistDecisions)
	: TSuper(2, GenerateDecisionSpace(1 + nDistDecisions))
{
}

template <typename _TReal>
UF7<_TReal>::~UF7(void)
{
}

template <typename _TReal>
typename UF7<_TReal>::TBoundary UF7<_TReal>::GenerateDecisionSpace(const size_t dimension)
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
size_t UF7<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void UF7<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	assert(this->IsInside(decision));
	objective.resize(TSuper::GetNumberOfObjectives());
	const TReal x = decision[0];
	const TReal bias = pow(x, 0.2);
	objective[0] = bias + 2 * distance::Distance7(decision, 2);
	objective[1] = 1 - bias + 2 * distance::Distance7(decision, 1);
}
}
}
}
