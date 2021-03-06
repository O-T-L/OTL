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
@ARTICLE{,
  author = {Johannes Bader and Eckart Zitzler},
  title = {{HypE}: An Algorithm for Fast Hypervolume-Based Many-Objective Optimization},
  journal = {Evolutionary Computation},
  year = {2011},
  volume = {19},
  pages = {45-76},
  number = {1},
  doi = {10.1162/EVCO_a_00009}
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
#include "Individual.h"
#include "Fitness.h"
#include "ReferencePoint.h"

namespace otl
{
namespace optimizer
{
namespace hype
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class MonteCarloHypE : public Metaheuristic<std::vector<Individual<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>, public otl::crossover::WithCrossover<_TReal, _TDecision>, public otl::mutation::WithMutation<_TReal, _TDecision>
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

	MonteCarloHypE(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const size_t nSample, const TReal expand = 1);
	~MonteCarloHypE(void);
	size_t GetSampleSize(void) const;
	TReal GetExpand(void) const;
	TSolutionSet MakeOffspring(TSolutionSet &ancestor);
	static bool Dominate(const TIndividual &individual1, const TIndividual &individual2);

protected:
	static bool _Dominate(const TIndividual *individual1, const TIndividual *individual2);
	void _DoStep(void);
	template <typename _TPointer, typename _TIterator> _TIterator _SelectNoncritical(const std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	template <typename _TPointer, typename _TIterator> _TIterator _SelectCritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	static const TIndividual *_Compete(const std::vector<const TIndividual *> &competition);

private:
	const size_t nSample_;
	const TReal expand_;
};

template <typename _TReal, typename _TDecision, typename _TRandom>
MonteCarloHypE<_TReal, _TDecision, _TRandom>::MonteCarloHypE(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const size_t nSample, const TReal expand)
	: TSuper(problem)
	, otl::utility::WithRandom<TRandom>(random)
	, otl::crossover::WithCrossover<TReal, TDecision>(crossover)
	, otl::mutation::WithMutation<TReal, TDecision>(mutation)
	, nSample_(nSample)
	, expand_(expand)
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
MonteCarloHypE<_TReal, _TDecision, _TRandom>::~MonteCarloHypE(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
size_t MonteCarloHypE<_TReal, _TDecision, _TRandom>::GetSampleSize(void) const
{
	return nSample_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal MonteCarloHypE<_TReal, _TDecision, _TRandom>::GetExpand(void) const
{
	return expand_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
typename MonteCarloHypE<_TReal, _TDecision, _TRandom>::TSolutionSet MonteCarloHypE<_TReal, _TDecision, _TRandom>::MakeOffspring(TSolutionSet &ancestor)
{
	typedef typename TSolutionSet::pointer _TPointer;
	{
		std::list<_TPointer> _ancestor;
		for (size_t i = 0; i < ancestor.size(); ++i)
			_ancestor.push_back(&ancestor[i]);
		const auto lower = FindLower<TReal>(_ancestor.begin(), _ancestor.end());
		const auto upper = FindUpper<TReal>(_ancestor.begin(), _ancestor.end());
		const auto referencePoint = CalculateReferencePoint(lower, upper, expand_);
		FitnessEstimation(this->GetRandom(), _ancestor.begin(), _ancestor.end(), lower, referencePoint, nSample_, ancestor.size());
	}
	const TSolutionSet &_ancestor = ancestor;
	TSolutionSet offspring = otl::optimizer::nsga_ii::MakeOffspring(ancestor.size(), _ancestor.begin(), _ancestor.end(), this->GetRandom(), &_Compete, this->GetCrossover());
	for (size_t i = 0; i < offspring.size(); ++i)
	{
		TIndividual &child = offspring[i];
		this->GetMutation()(child);
		TSuper::GetProblem()(child);
	}
	return offspring;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool MonteCarloHypE<_TReal, _TDecision, _TRandom>::Dominate(const TIndividual &individual1, const TIndividual &individual2)
{
	return otl::utility::relation::Dominate(individual1.objective_, individual2.objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool MonteCarloHypE<_TReal, _TDecision, _TRandom>::_Dominate(const TIndividual *individual1, const TIndividual *individual2)
{
	return Dominate(*individual1, *individual2);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void MonteCarloHypE<_TReal, _TDecision, _TRandom>::_DoStep(void)
{
	TSolutionSet ancestor = TSuper::solutionSet_;
	TSolutionSet offspring = MakeOffspring(ancestor);
	typedef typename TSolutionSet::pointer _TPointer;
	std::list<_TPointer> mix;
	for (size_t i = 0; i < ancestor.size(); ++i)
		mix.push_back(&ancestor[i]);
	for (size_t i = 0; i < offspring.size(); ++i)
		mix.push_back(&offspring[i]);
	typedef typename TSolutionSet::iterator _TIterator;
	otl::selection::NondominateSelection(mix, TSuper::solutionSet_.begin(), TSuper::solutionSet_.end(), &_Dominate
		, [this](std::list<_TPointer> &front, _TIterator begin, _TIterator end)->_TIterator{return this->_SelectNoncritical(front, begin, end);}
		, [this](std::list<_TPointer> &front, _TIterator begin, _TIterator end)->_TIterator{return this->_SelectCritical(front, begin, end);}
	);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator MonteCarloHypE<_TReal, _TDecision, _TRandom>::_SelectNoncritical(const std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	_TIterator dest = begin;
	for (auto i = front.begin(); i != front.end(); ++i, ++dest)
		*dest = **i;
	return dest;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator MonteCarloHypE<_TReal, _TDecision, _TRandom>::_SelectCritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	const auto lower = FindLower<TReal>(front.begin(), front.end());
	const auto upper = FindUpper<TReal>(front.begin(), front.end());
	const auto referencePoint = CalculateReferencePoint(lower, upper, expand_);
	assert(front.size() >= std::distance(begin, end));
	for (size_t remove = front.size() - std::distance(begin, end); remove; --remove)
	{
		FitnessEstimation(this->GetRandom(), front.begin(), front.end(), lower, referencePoint, nSample_, remove);
		auto worst = std::min_element(front.begin(), front.end(), [](_TPointer individual1, _TPointer individual2)->bool{return individual1->fitness_ < individual2->fitness_;});
		front.erase(worst);
	}
	_TIterator dest = begin;
	for (auto i = front.begin(); i != front.end(); ++i, ++dest)
		*dest = **i;
	return dest;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename MonteCarloHypE<_TReal, _TDecision, _TRandom>::TIndividual *MonteCarloHypE<_TReal, _TDecision, _TRandom>::_Compete(const std::vector<const TIndividual *> &competition)
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
