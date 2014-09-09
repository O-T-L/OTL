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
  author = {Eckart Zitzler and Marco Laumanns and Lothar Thiele},
  title = {SPEA2: Improving the Strength Pareto Evolutionary Algorithm for Multiobjective
	Optimization},
  booktitle = {Evolutionary Methods for Design, Optimisation, and Control},
  year = {2002},
  pages = {95-100},
  publisher = {CIMNE, Barcelona, Spain}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <list>
#include <OTL/Optimizer/Metaheuristic.h>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Crossover/WithCrossover.h>
#include <OTL/Mutation/WithMutation.h>
#include <OTL/Utility/Relation/Pareto.h>
#include <OTL/Utility/Nondominate.h>
#include <OTL/Selection/NondominateSelection.h>
#include <OTL/Optimizer/NSGA-II/Offspring.h>
#include "RawFitness.h"
#include "Density.h"
#include "Utility.h"
#include "Truncation.h"
#include "Individual.h"

namespace otl
{
namespace optimizer
{
namespace spea2
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class SPEA2 : public Metaheuristic<std::vector<Individual<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>, public otl::crossover::WithCrossover<_TReal, _TDecision>, public otl::mutation::WithMutation<_TReal, _TDecision>
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

	SPEA2(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation);
	~SPEA2(void);
	TSolutionSet MakeOffspring(const TSolutionSet &ancestor);
	static bool Dominate(const TIndividual &individual1, const TIndividual &individual2);
	static TReal Distance(const TIndividual &individual1, const TIndividual &individual2);

protected:
	static bool _Dominate(const TIndividual *individual1, const TIndividual *individual2);
	static TReal _Distance(const TIndividual *individual1, const TIndividual *individual2);
	void _DoStep(void);
	template <typename _TPointer, typename _TIterator> static _TIterator _SelectNoncritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	template <typename _TPointer, typename _TIterator> static _TIterator _SelectCritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	template <typename _TPointer, typename _TIterator> static _TIterator _Truncation(std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	static const TIndividual *_Compete(const std::vector<const TIndividual *> &competition);
};

template <typename _TReal, typename _TDecision, typename _TRandom>
SPEA2<_TReal, _TDecision, _TRandom>::SPEA2(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation)
	: TSuper(problem)
	, otl::utility::WithRandom<TRandom>(random)
	, otl::crossover::WithCrossover<TReal, TDecision>(crossover)
	, otl::mutation::WithMutation<TReal, TDecision>(mutation)
{
	TSuper::solutionSet_.resize(initial.size());
	for (size_t i = 0; i < initial.size(); ++i)
	{
		TIndividual &individual = TSuper::solutionSet_[i];
		individual.decision_ = initial[i];
		TSuper::GetProblem()(individual);
	}
	for (auto i = TSuper::solutionSet_.begin(); i != TSuper::solutionSet_.end(); ++i)
	{
		i->rawFitness_ = RawFitness(i, TSuper::solutionSet_.begin(), TSuper::solutionSet_.end(), &Dominate);
		i->density_ = Density<TReal>(i, TSuper::solutionSet_.begin(), TSuper::solutionSet_.end(), &Distance);
		i->fitness_ = i->rawFitness_ + i->density_;
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
SPEA2<_TReal, _TDecision, _TRandom>::~SPEA2(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
typename SPEA2<_TReal, _TDecision, _TRandom>::TSolutionSet SPEA2<_TReal, _TDecision, _TRandom>::MakeOffspring(const TSolutionSet &ancestor)
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
bool SPEA2<_TReal, _TDecision, _TRandom>::Dominate(const TIndividual &individual1, const TIndividual &individual2)
{
	return otl::utility::relation::Dominate(individual1.objective_, individual2.objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal SPEA2<_TReal, _TDecision, _TRandom>::Distance(const TIndividual &individual1, const TIndividual &individual2)
{
	return sqrt(std::inner_product(individual1.objective_.begin(), individual1.objective_.end(), individual2.objective_.begin(), (_TReal)0
		, std::plus<TReal>()
		, [](TReal x, TReal y)->TReal{TReal t = x - y;return t * t;}
	));
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool SPEA2<_TReal, _TDecision, _TRandom>::_Dominate(const TIndividual *individual1, const TIndividual *individual2)
{
	return Dominate(*individual1, *individual2);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal SPEA2<_TReal, _TDecision, _TRandom>::_Distance(const TIndividual *individual1, const TIndividual *individual2)
{
	return Distance(*individual1, *individual2);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void SPEA2<_TReal, _TDecision, _TRandom>::_DoStep(void)
{
	TSolutionSet ancestor = TSuper::solutionSet_;
	TSolutionSet offspring = MakeOffspring(ancestor);
	typedef typename TSolutionSet::pointer _TPointer;
	std::list<_TPointer> mix;
	for (size_t i = 0; i < ancestor.size(); ++i)
		mix.push_back(&ancestor[i]);
	for (size_t i = 0; i < offspring.size(); ++i)
		mix.push_back(&offspring[i]);
	for (auto i = mix.begin(); i != mix.end(); ++i)
	{
		TIndividual &individual = **i;
		individual.rawFitness_ = RawFitness(i, mix.begin(), mix.end(), &_Dominate);
		individual.density_ = Density<TReal>(i, mix.begin(), mix.end(), &_Distance);
		individual.fitness_ = individual.rawFitness_ + individual.density_;
	}
	typedef typename TSolutionSet::iterator _TIterator;
	otl::selection::NondominateSelection(mix, TSuper::solutionSet_.begin(), TSuper::solutionSet_.end(), &_Dominate, &_SelectNoncritical<_TPointer, _TIterator>, &_SelectCritical<_TPointer, _TIterator>);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator SPEA2<_TReal, _TDecision, _TRandom>::_SelectNoncritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	assert(front.size() <= std::distance(begin, end));
	_TIterator dest = begin;
	for (auto i = front.begin(); i != front.end(); ++i, ++dest)
		*dest = **i;
	return dest;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator SPEA2<_TReal, _TDecision, _TRandom>::_SelectCritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	std::list<_TPointer> elites = SpliceElites(front);
	if (elites.size() > std::distance(begin, end))
		return _Truncation(elites, begin, end);
	else
	{
		_TIterator dest = begin;
		for (auto i = elites.begin(); i != elites.end(); ++i, ++dest)
			*dest = **i;
		std::vector<_TPointer> _front(front.begin(), front.end());
		std::partial_sort(_front.begin(), _front.begin() + std::distance(dest, end), _front.end()
			, [](_TPointer individual1, _TPointer individual2)->bool{return individual1->fitness_ < individual2->fitness_;}
		);
		for (size_t i = 0; dest != end; ++i, ++dest)
			*dest = *_front[i];
		return dest;
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator SPEA2<_TReal, _TDecision, _TRandom>::_Truncation(std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	assert(front.size() > std::distance(begin, end));
	Truncation<TReal>(front.size() - std::distance(begin, end), front, &_Distance);
	assert(front.size() == std::distance(begin, end));
	_TIterator dest = begin;
	for (auto i = front.begin(); i != front.end() && dest != end; ++i, ++dest)
		*dest = **i;
	return dest;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename SPEA2<_TReal, _TDecision, _TRandom>::TIndividual *SPEA2<_TReal, _TDecision, _TRandom>::_Compete(const std::vector<const TIndividual *> &competition)
{
	if (Dominate(*competition[0], *competition[1]))
		return competition[0];
	else if (Dominate(*competition[1], *competition[0]))
		return competition[1];
	assert(competition[0]->fitness_ >= 0);
	assert(competition[1]->fitness_ >= 0);
	return competition[0]->fitness_ < competition[1]->fitness_ ? competition[0] : competition[1];
}
}
}
}
