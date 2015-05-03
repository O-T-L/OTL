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

#include "IBEA.h"

namespace otl
{
namespace optimizer
{
namespace ibea
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class IBEA_Epsilon : public IBEA<_TReal, _TDecision, _TRandom>
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

	IBEA_Epsilon(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const TReal scalingFactor);
	virtual ~IBEA_Epsilon(void);

protected:
	TReal _DoEvaluate(const TIndividual &individual1, const TIndividual &individual2);
	static TReal _Evaluate(const std::vector<TReal> &objective1, const std::vector<TReal> &objective2);
};

template <typename _TReal, typename _TDecision, typename _TRandom>
IBEA_Epsilon<_TReal, _TDecision, _TRandom>::IBEA_Epsilon(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation,const TReal scalingFactor)
	: TSuper(random, problem, initial, crossover, mutation, scalingFactor)
{
	TSuper::FitnessAssignment(TSuper::solutionSet_.begin(), TSuper::solutionSet_.end(), [](TIndividual &individual)->TIndividual &{return individual;});
}

template <typename _TReal, typename _TDecision, typename _TRandom>
IBEA_Epsilon<_TReal, _TDecision, _TRandom>::~IBEA_Epsilon(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal IBEA_Epsilon<_TReal, _TDecision, _TRandom>::_DoEvaluate(const TIndividual &individual1, const TIndividual &individual2)
{
	return _Evaluate(individual1.objective_, individual2.objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal IBEA_Epsilon<_TReal, _TDecision, _TRandom>::_Evaluate(const std::vector<TReal> &objective1, const std::vector<TReal> &objective2)
{
	assert(objective1.size() > 0);
	assert(objective1.size() == objective2.size());
	TReal maxEpsilon = objective2[0] - objective1[0];
	for (size_t i = 1; i < objective1.size(); ++i)
	{
		const TReal epsilon = objective2[i] - objective1[i];
		if (epsilon > maxEpsilon)
			maxEpsilon = epsilon;
	}
	return maxEpsilon;
}
}
}
}

