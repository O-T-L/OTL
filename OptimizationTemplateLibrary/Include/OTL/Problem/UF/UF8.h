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
#include <boost/math/constants/constants.hpp>
#include "UF.h"
#include "Distance/Distance8.h"

namespace otl
{
namespace problem
{
namespace uf
{
template <typename _TReal>
class UF8 : public UF<_TReal>
{
public:
	typedef _TReal TReal;
	typedef UF<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;
	typedef typename TSuper::TRange TRange;
	typedef typename TSuper::TBoundary TBoundary;

	UF8(const size_t nDistDecisions = 28);
	~UF8(void);
	static TBoundary GenerateDecisionSpace(const size_t dimension);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);
};

template <typename _TReal>
UF8<_TReal>::UF8(const size_t nDistDecisions)
	: TSuper(3, GenerateDecisionSpace(2 + nDistDecisions))
{
}

template <typename _TReal>
UF8<_TReal>::~UF8(void)
{
}

template <typename _TReal>
typename UF8<_TReal>::TBoundary UF8<_TReal>::GenerateDecisionSpace(const size_t dimension)
{
	TBoundary boundary(dimension);
	for (size_t i = 0; i < 2; ++i)
	{
		boundary[i].first = 0;
		boundary[i].second = 1;
	}
	for (size_t i = 2; i < boundary.size(); ++i)
	{
		boundary[i].first = -2;
		boundary[i].second = 2;
	}
	return boundary;
}

template <typename _TReal>
size_t UF8<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void UF8<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	objective.resize(TSuper::GetNumberOfObjectives());
	const TReal alpha = decision[0] * boost::math::constants::pi<TReal>() / 2;
	const TReal beta = decision[1] * boost::math::constants::pi<TReal>() / 2;
	objective[0] = cos(alpha) * cos(beta) + 2 * distance::Distance8(decision, 3);
	objective[1] = cos(alpha) * sin(beta) + 2 * distance::Distance8(decision, 4);
	objective[2] = sin(alpha) + 2 * distance::Distance8(decision, 2);
}
}
}
}
