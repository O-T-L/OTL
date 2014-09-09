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
  author = {Eckart Zitzler and Simon Künzli},
  title = {Indicator-Based Selection in Multiobjective Search},
  booktitle = {Parallel Problem Solving from Nature},
  year = {2004},
  pages = {832-842},
  publisher = {Springer},
  doi = {10.1007/978-3-540-30217-9_84}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <list>
#include <limits>
#include <OTL/Optimizer/Metaheuristic.h>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Crossover/WithCrossover.h>
#include <OTL/Mutation/WithMutation.h>
#include <OTL/Utility/Relation/Pareto.h>
#include <OTL/Optimizer/NSGA-II/Offspring.h>
#include "Individual.h"

namespace otl
{
namespace optimizer
{
namespace ibea
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class IBEA : public Metaheuristic<std::vector<Individual<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>, public otl::crossover::WithCrossover<_TReal, _TDecision>, public otl::mutation::WithMutation<_TReal, _TDecision>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef Individual<TReal, TDecision> TIndividual;
	typedef std::vector<TIndividual> TSolutionSet;
	typedef Metaheuristic<TSolutionSet> TSuper;
	typedef typename TSuper::TProblem TProblem;
	typedef typename otl::crossover::WithCrossover<TReal, TDecision>::TCrossover TCrossover;
	typedef typename otl::mutation::WithMutation<TReal, TDecision>::TMutation TMutation;

	IBEA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const TReal scalingFactor = 0.05);
	virtual ~IBEA(void);
	TReal GetScalingFactor(void) const;
	TReal Evaluate(const TIndividual &individual1, const TIndividual &individual2);
	template <typename _TIterator, typename _TFetcher> void FitnessAssignment(_TIterator begin, _TIterator end, _TFetcher fetcher);
	template <typename _TIterator, typename _TFetcher> void FitnessUpdate(const TIndividual &selected, _TIterator begin, _TIterator end, _TFetcher fetcher);
	TSolutionSet MakeOffspring(const TSolutionSet &ancestor);
	template <typename _TPointer> void EnvironmentalSelection(std::list<_TPointer> &mix, TSolutionSet &solutionSet);
	template <typename _TPointer> void Reduce(size_t count, std::list<_TPointer> &population);
	static bool Dominate(const TIndividual &individual1, const TIndividual &individual2);

protected:
	static bool _Dominate(const TIndividual *individual1, const TIndividual *individual2);
	void _DoStep(void);
	static const TIndividual *_Compete(const std::vector<const TIndividual *> &competition);
	virtual TReal _DoEvaluate(const TIndividual &individual1, const TIndividual &individual2) = 0;

private:
	TReal scalingFactor_;
};

template <typename _TReal, typename _TDecision, typename _TRandom>
IBEA<_TReal, _TDecision, _TRandom>::IBEA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation,const TReal scalingFactor)
	: TSuper(problem)
	, otl::utility::WithRandom<TRandom>(random)
	, otl::crossover::WithCrossover<TReal, TDecision>(crossover)
	, otl::mutation::WithMutation<TReal, TDecision>(mutation)
	, scalingFactor_(scalingFactor)
{
	TSuper::solutionSet_.resize(initial.size());
	for (size_t i = 0; i < initial.size(); ++i)
	{
		TIndividual &individual = TSuper::solutionSet_[i];
		individual.decision_ = initial[i];
		TSuper::GetProblem()(individual);
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
IBEA<_TReal, _TDecision, _TRandom>::~IBEA(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal IBEA<_TReal, _TDecision, _TRandom>::GetScalingFactor(void) const
{
	return scalingFactor_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal IBEA<_TReal, _TDecision, _TRandom>::Evaluate(const TIndividual &individual1, const TIndividual &individual2)
{
	assert(individual1.objective_.size() == individual2.objective_.size());
	return _DoEvaluate(individual1, individual2);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TIterator, typename _TFetcher> void IBEA<_TReal, _TDecision, _TRandom>::FitnessAssignment(_TIterator begin, _TIterator end, _TFetcher fetcher)
{
	for (_TIterator individual = begin; individual != end; ++individual)
	{
		TIndividual &_individual = fetcher(*individual);
		_individual.fitness_ = 0;
		for (_TIterator remaining = begin; remaining != end; ++remaining)
		{
			if (remaining != individual)
			{
				const TIndividual &_remaining = fetcher(*remaining);
				const TReal metric = Evaluate(_individual, _remaining);
				_individual.fitness_ -= exp(-metric / scalingFactor_);
			}
		}
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TIterator, typename _TFetcher> void IBEA<_TReal, _TDecision, _TRandom>::FitnessUpdate(const TIndividual &selected, _TIterator begin, _TIterator end, _TFetcher fetcher)
{
	for (_TIterator update = begin; update != end; ++update)
	{
		TIndividual &_update = fetcher(*update);
		const TReal metric = Evaluate(_update, selected);
		_update.fitness_ += exp(-metric / scalingFactor_);
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
typename IBEA<_TReal, _TDecision, _TRandom>::TSolutionSet IBEA<_TReal, _TDecision, _TRandom>::MakeOffspring(const TSolutionSet &ancestor)
{
	TSolutionSet offspring = otl::optimizer::nsga_ii::MakeOffspring(ancestor.size(), ancestor.begin(), ancestor.end(), this->GetRandom(), &_Compete, this->GetCrossover());
	for (size_t i = 0; i < offspring.size(); ++i)
	{
		TIndividual &child = offspring[i];
		this->GetMutation()(child);
		TSuper::GetProblem()(child);
	}
	return offspring;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer> void IBEA<_TReal, _TDecision, _TRandom>::EnvironmentalSelection(std::list<_TPointer> &mix, TSolutionSet &solutionSet)
{
	FitnessAssignment(mix.begin(), mix.end(), [](_TPointer individual)->TIndividual &{return *individual;});
	assert(mix.size() > solutionSet.size());
	Reduce(mix.size() - solutionSet.size(), mix);
	assert(mix.size() == solutionSet.size());
	size_t index = 0;
	for (auto i = mix.begin(); i != mix.end(); ++i, ++index)
		solutionSet[index] = **i;
}

template <typename _TReal,typename _TDecision, typename _TRandom>
template <typename _TPointer> void IBEA<_TReal, _TDecision, _TRandom>::Reduce(size_t count, std::list<_TPointer> &population)
{
	assert(count < population.size());
	std::list<_TPointer> abandon;
	while (count)
	{
		auto worst = std::min_element(population.begin(), population.end(), [](_TPointer individual1, _TPointer individual2)->bool{return individual1->fitness_ < individual2->fitness_;});
		abandon.splice(abandon.end(), population, worst);
		FitnessUpdate(*abandon.back(), population.begin(), population.end(), [](_TPointer individual)->TIndividual &{return *individual;});
		--count;
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool IBEA<_TReal, _TDecision, _TRandom>::Dominate(const TIndividual &individual1, const TIndividual &individual2)
{
	return otl::utility::relation::Dominate(individual1.objective_, individual2.objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool IBEA<_TReal, _TDecision, _TRandom>::_Dominate(const TIndividual *individual1, const TIndividual *individual2)
{
	return Dominate(*individual1, *individual2);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void IBEA<_TReal, _TDecision, _TRandom>::_DoStep(void)
{
	TSolutionSet ancestor = TSuper::solutionSet_;
	TSolutionSet offspring = MakeOffspring(ancestor);
	typedef typename TSolutionSet::pointer _TPointer;
	std::list<_TPointer> mix;
	for (size_t i = 0; i < ancestor.size(); ++i)
		mix.push_back(&ancestor[i]);
	for (size_t i = 0; i < offspring.size(); ++i)
		mix.push_back(&offspring[i]);
	EnvironmentalSelection(mix, TSuper::solutionSet_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename IBEA<_TReal, _TDecision, _TRandom>::TIndividual *IBEA<_TReal, _TDecision, _TRandom>::_Compete(const std::vector<const TIndividual *> &competition)
{
	if (Dominate(*competition[0], *competition[1]))
		return competition[0];
	else if (Dominate(*competition[1], *competition[0]))
		return competition[1];
	assert(competition[0]->fitness_ <= 0);
	assert(competition[1]->fitness_ <= 0);
	return competition[0]->fitness_ < competition[1]->fitness_ ? competition[0] : competition[1];
}
}
}
}

