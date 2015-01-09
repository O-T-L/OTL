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
#include "Distance/Distance5.h"

namespace otl
{
namespace problem
{
namespace uf
{
template <typename _TReal>
class UF5 : public UF<_TReal>
{
public:
	typedef _TReal TReal;
	typedef UF<TReal> TSuper;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;
	typedef typename TSuper::TRange TRange;
	typedef typename TSuper::TBoundary TBoundary;

	UF5(const size_t nDistDecisions = 29, const TReal epsilon = 0.1, const size_t n = 10);
	~UF5(void);
	static TBoundary GenerateDecisionSpace(const size_t dimension);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);

private:
	TReal epsilon_;
	size_t n_;
};

template <typename _TReal>
UF5<_TReal>::UF5(const size_t nDistDecisions, const TReal epsilon, const size_t n)
	: TSuper(2, GenerateDecisionSpace(1 + nDistDecisions))
	, epsilon_(epsilon)
	, n_(n)
{
}

template <typename _TReal>
UF5<_TReal>::~UF5(void)
{
}

template <typename _TReal>
typename UF5<_TReal>::TBoundary UF5<_TReal>::GenerateDecisionSpace(const size_t dimension)
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
size_t UF5<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void UF5<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	objective.resize(TSuper::GetNumberOfObjectives());
	const TReal x = decision[0];
	const TReal temp = (0.5 / n_ + epsilon_) * std::fabs(sin(2 * n_ * boost::math::constants::pi<TReal>() * x));
	objective[0] = x + temp + 2 * distance::Distance5(decision, 2);
	objective[1] = 1 - x + temp + 2 * distance::Distance5(decision, 1);
}
}
}
}
