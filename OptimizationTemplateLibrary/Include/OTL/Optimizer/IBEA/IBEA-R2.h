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

/*
@CONFERENCE{,
  author = {Dũng H. Phan and Junichi Suzuki},
  title = {{R2-IBEA}: {R2} Indicator Based Evolutionary Algorithm for Multiobjective
	Optimization},
  booktitle = {IEEE Congress on Evolutionary Computation},
  year = {2013},
  pages = {1836-1845},
  address = {Cancun, Mexico},
  month = {20-23 June},
  doi = {10.1109/CEC.2013.6557783}
}
*/
#pragma once

#include <OTL/Utility/Aggregation/Tchebycheff.h>
#include "IBEA.h"

namespace otl
{
namespace optimizer
{
namespace ibea
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class IBEA_R2 : public IBEA<_TReal, _TDecision, _TRandom>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef IBEA<TReal, TDecision, TRandom> TSuper;
	typedef typename TSuper::TIndividual TIndividual;
	typedef typename TSuper::TSolutionSet TSolutionSet;
	typedef typename TSuper::TProblem TProblem;
	typedef typename TSuper::TCrossover TCrossover;
	typedef typename TSuper::TMutation TMutation;
	typedef std::vector<TReal> TPoint;
	typedef std::vector<TPoint> TFront;

	IBEA_R2(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const TReal scalingFactor, const TPoint &referencePoint, const TFront &weightVectors);
	virtual ~IBEA_R2(void);
	const TPoint &GetReferencePoint(void) const;
	const TFront &GetWeightVectors(void) const;
	TPoint ComputeDirection(const TPoint &objective) const;

protected:
	TReal _DoEvaluate(const TIndividual &individual1, const TIndividual &individual2);
	TReal _Evaluate(const std::vector<TReal> &objective1, const std::vector<TReal> &objective2) const;

private:
	TPoint referencePoint_;
	TFront weightVectors_;
};

template <typename _TReal, typename _TDecision, typename _TRandom>
IBEA_R2<_TReal, _TDecision, _TRandom>::IBEA_R2(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation,const TReal scalingFactor, const TPoint &referencePoint, const TFront &weightVectors)
	: TSuper(random, problem, initial, crossover, mutation, scalingFactor)
	, referencePoint_(referencePoint)
	, weightVectors_(weightVectors)
{
	TSuper::FitnessAssignment(TSuper::solutionSet_.begin(), TSuper::solutionSet_.end(), [](TIndividual &individual)->TIndividual &{return individual;});
}

template <typename _TReal, typename _TDecision, typename _TRandom>
IBEA_R2<_TReal, _TDecision, _TRandom>::~IBEA_R2(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename IBEA_R2<_TReal, _TDecision, _TRandom>::TPoint &IBEA_R2<_TReal, _TDecision, _TRandom>::GetReferencePoint(void) const
{
	return referencePoint_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename IBEA_R2<_TReal, _TDecision, _TRandom>::TFront &IBEA_R2<_TReal, _TDecision, _TRandom>::GetWeightVectors(void) const
{
	return weightVectors_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
typename IBEA_R2<_TReal, _TDecision, _TRandom>::TPoint IBEA_R2<_TReal, _TDecision, _TRandom>::ComputeDirection(const TPoint &objective) const
{
	assert(objective.size() == referencePoint_.size());
	TPoint direction(objective.size());
	for (size_t i = 0; i < direction.size(); ++i)
	{
		direction[i] = objective[i] - referencePoint_[i];
		assert(direction[i] >= 0);
	}
	return direction;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal IBEA_R2<_TReal, _TDecision, _TRandom>::_DoEvaluate(const TIndividual &individual1, const TIndividual &individual2)
{
	return _Evaluate(individual1.objective_, individual2.objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal IBEA_R2<_TReal, _TDecision, _TRandom>::_Evaluate(const std::vector<TReal> &objective1, const std::vector<TReal> &objective2) const
{
	assert(objective1.size() == objective2.size());
	assert(!weightVectors_.empty());
	TReal sum1 = 0, sum2 = 0;
	for (size_t i = 0; i < weightVectors_.size(); ++i)
	{
		sum1 += otl::utility::aggregation::Tchebycheff(weightVectors_[i], ComputeDirection(objective1));
		sum2 += std::min(otl::utility::aggregation::Tchebycheff(weightVectors_[i], ComputeDirection(objective1)), otl::utility::aggregation::Tchebycheff(weightVectors_[i], ComputeDirection(objective2)));
	}
	return sum1 / weightVectors_.size() - sum2 / weightVectors_.size();
}
}
}
}

