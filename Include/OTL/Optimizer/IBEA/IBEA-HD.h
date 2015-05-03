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
#include "Boundary.h"
#include "Hypervolume.h"

namespace otl
{
namespace optimizer
{
namespace ibea
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class IBEA_HD : public IBEA<_TReal, _TDecision, _TRandom>
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

	IBEA_HD(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const TReal scalingFactor);
	virtual ~IBEA_HD(void);
	template <typename _TPointer> void EnvironmentalSelection(std::list<_TPointer> &mix, TSolutionSet &solutionSet);

protected:
	void _DoStep(void);
	TReal _DoEvaluate(const TIndividual &individual1, const TIndividual &individual2);
	TReal _Evaluate(const std::vector<TReal> &objective1, const std::vector<TReal> &objective2) const;

private:
	std::vector<std::pair<TReal, TReal> > boundary_;
};

template <typename _TReal, typename _TDecision, typename _TRandom>
IBEA_HD<_TReal, _TDecision, _TRandom>::IBEA_HD(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation,const TReal scalingFactor)
	: TSuper(random, problem, initial, crossover, mutation, scalingFactor)
{
	boundary_ = CalculateBoundary<TReal>(TSuper::solutionSet_.begin(), TSuper::solutionSet_.end(), [](const TIndividual &individual)->const std::vector<TReal> &{return individual.objective_;});
	TSuper::FitnessAssignment(TSuper::solutionSet_.begin(), TSuper::solutionSet_.end(), [](TIndividual &individual)->TIndividual &{return individual;});
}

template <typename _TReal, typename _TDecision, typename _TRandom>
IBEA_HD<_TReal, _TDecision, _TRandom>::~IBEA_HD(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer> void IBEA_HD<_TReal, _TDecision, _TRandom>::EnvironmentalSelection(std::list<_TPointer> &mix, TSolutionSet &solutionSet)
{
	boundary_ = CalculateBoundary<TReal>(mix.begin(), mix.end(), [](_TPointer individual)->const std::vector<TReal> &{return individual->objective_;});
	TSuper::FitnessAssignment(mix.begin(), mix.end(), [](_TPointer individual)->TIndividual &{return *individual;});
	assert(mix.size() > solutionSet.size());
	TSuper::Reduce(mix.size() - solutionSet.size(), mix);
	assert(mix.size() == solutionSet.size());
	size_t index = 0;
	for (auto i = mix.begin(); i != mix.end(); ++i, ++index)
		solutionSet[index] = **i;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void IBEA_HD<_TReal, _TDecision, _TRandom>::_DoStep(void)
{
	TSolutionSet ancestor = TSuper::solutionSet_;
	TSolutionSet offspring = TSuper::MakeOffspring(ancestor);
	typedef typename TSolutionSet::pointer _TPointer;
	std::list<_TPointer> mix;
	for (size_t i = 0; i < ancestor.size(); ++i)
		mix.push_back(&ancestor[i]);
	for (size_t i = 0; i < offspring.size(); ++i)
		mix.push_back(&offspring[i]);
	EnvironmentalSelection(mix, TSuper::solutionSet_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal IBEA_HD<_TReal, _TDecision, _TRandom>::_DoEvaluate(const TIndividual &individual1, const TIndividual &individual2)
{
	return _Evaluate(individual1.objective_, individual2.objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal IBEA_HD<_TReal, _TDecision, _TRandom>::_Evaluate(const std::vector<TReal> &objective1, const std::vector<TReal> &objective2) const
{
	assert(objective1.size() == objective2.size());
	if (otl::utility::relation::Dominate(objective2, objective1))
		return -CalculateHypervolume(TSuper::GetProblem().GetNumberOfObjectives() - 1, boundary_, objective2, objective1);
	else
		return CalculateHypervolume(TSuper::GetProblem().GetNumberOfObjectives() - 1, boundary_, objective1, objective2);
}
}
}
}

