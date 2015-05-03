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

#include <OTL/Utility/Aggregation/Tchebycheff.h>
#include "MOEA-D.h"

namespace otl
{
namespace optimizer
{
namespace moea_d
{
namespace xtriple
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class MOEA_D_Tchebycheff : public otl::optimizer::moea_d::xtriple::MOEA_D<_TReal, _TDecision, _TRandom>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef otl::optimizer::moea_d::xtriple::MOEA_D<TReal, TDecision, TRandom> TSuper;
	typedef typename TSuper::TIndividual TIndividual;
	typedef typename TSuper::TSolutionSet TSolutionSet;
	typedef typename TSuper::TProblem TProblem;
	typedef typename TSuper::TCrossover TCrossover;
	typedef typename TSuper::TMutation TMutation;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TNeighbors TNeighbors;

	MOEA_D_Tchebycheff(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const std::vector<TPoint> &weightVectors, const size_t nNeighbors);
	~MOEA_D_Tchebycheff(void);

protected:
	TReal _DoAggregation(const TPoint &objective, const TPoint &weight);
};

template <typename _TReal, typename _TDecision, typename _TRandom>
MOEA_D_Tchebycheff<_TReal, _TDecision, _TRandom>::MOEA_D_Tchebycheff(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const std::vector<TPoint> &weightVectors, const size_t nNeighbors)
	: TSuper(random, problem, initial, crossover, mutation, weightVectors, nNeighbors)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
MOEA_D_Tchebycheff<_TReal, _TDecision, _TRandom>::~MOEA_D_Tchebycheff(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal MOEA_D_Tchebycheff<_TReal, _TDecision, _TRandom>::_DoAggregation(const TPoint &objective, const TPoint &weight)
{
	const auto direction = ComputeDirection(objective, TSuper::GetReferencePoint());
	return otl::utility::aggregation::Tchebycheff(weight, direction);
}
}
}
}
}
