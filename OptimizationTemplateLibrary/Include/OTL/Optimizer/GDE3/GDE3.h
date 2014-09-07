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
  author = {Saku Kukkonen and Jouni Lampinen},
  title = {{GDE3}: The third Evolution Step of Generalized Differential Evolution},
  booktitle = {IEEE Congress on Evolutionary Computation},
  year = {2005},
  volume = {1},
  pages = {443-450},
  address = {Edinburgh, Scotland},
  month = {September},
  publisher = {IEEE Service Center},
  doi = {10.1109/CEC.2005.1554717}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <list>
#include <random>
#include <OTL/Optimizer/Metaheuristic.h>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Crossover/WithXTripleCrossover.h>
#include <OTL/Mutation/WithMutation.h>
#include <OTL/Utility/Relation/Pareto.h>
#include <OTL/Utility/Nondominate.h>
#include <OTL/Selection/NondominateSelection.h>
#include <OTL/Optimizer/NSGA-II/Offspring.h>
#include <OTL/Optimizer/NSGA-II/CrowdingDistanceAssignment.h>
#include <OTL/Optimizer/NSGA-II/Individual.h>

namespace otl
{
namespace optimizer
{
namespace gde3
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class GDE3 : public Metaheuristic<std::vector<otl::optimizer::nsga_ii::Individual<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>, public otl::crossover::WithXTripleCrossover<_TReal, _TDecision>, public otl::mutation::WithMutation<_TReal, _TDecision>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef otl::optimizer::nsga_ii::Individual<TReal, TDecision> TIndividual;
	typedef std::vector<TIndividual> TSolutionSet;
	typedef std::list<TIndividual> TOffspring;
	typedef Metaheuristic<TSolutionSet> TSuper;
	typedef typename TSuper::TProblem TProblem;
	typedef typename otl::crossover::WithXTripleCrossover<TReal, TDecision>::TCrossover TCrossover;
	typedef typename otl::mutation::WithMutation<TReal, TDecision>::TMutation TMutation;

	GDE3(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation);
	~GDE3(void);
	TOffspring MakeOffspring(const TSolutionSet &ancestor);
	void MakeOffspring(const TIndividual &individual, const TSolutionSet &ancestor);
	void EnvironmentalSelection(TOffspring &offspring, TSolutionSet &solutionSet);
	static bool Dominate(const TIndividual &individual1, const TIndividual &individual2);

protected:
	static bool _Dominate(const TIndividual *individual1, const TIndividual *individual2);
	void _DoStep(void);
	template <typename _TPointer, typename _TIterator> static _TIterator _SelectNoncritical(const std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	template <typename _TPointer, typename _TIterator> static _TIterator _SelectCritical(const std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	static const TIndividual *_Compete(const std::vector<const TIndividual *> &competition);
};

template <typename _TReal, typename _TDecision, typename _TRandom>
GDE3<_TReal, _TDecision, _TRandom>::GDE3(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation)
	: TSuper(problem)
	, otl::utility::WithRandom<TRandom>(random)
	, otl::crossover::WithXTripleCrossover<TReal, TDecision>(crossover)
	, otl::mutation::WithMutation<TReal, TDecision>(mutation)
{
	TSuper::solutionSet_.resize(initial.size());
	for (size_t i = 0; i < initial.size(); ++i)
	{
		TIndividual &individual = TSuper::solutionSet_[i];
		individual.decision_ = initial[i];
		TSuper::GetProblem()(individual);
	}

	typedef typename TSolutionSet::pointer _TPointer;
	std::list<_TPointer> solutionSet;
	for (size_t i = 0; i < TSuper::solutionSet_.size(); ++i)
		solutionSet.push_back(&TSuper::solutionSet_[i]);
	while (!solutionSet.empty())
	{
		std::list<_TPointer> nondominate = otl::utility::ExtractNondominate(solutionSet, &_Dominate);
		std::vector<_TPointer> _nondominate(nondominate.begin(), nondominate.end());
		otl::optimizer::nsga_ii::CrowdingDistanceAssignment<TReal>(_nondominate.begin(), _nondominate.end());
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
GDE3<_TReal, _TDecision, _TRandom>::~GDE3(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
typename GDE3<_TReal, _TDecision, _TRandom>::TOffspring GDE3<_TReal, _TDecision, _TRandom>::MakeOffspring(const TSolutionSet &ancestor)
{
	typedef typename TSolutionSet::const_pointer _TConstPointer;
	std::vector<_TConstPointer> _ancestor(ancestor.size());
	for (size_t i = 0; i < ancestor.size(); ++i)
		_ancestor[i] = &ancestor[i];
	TOffspring offspring;
	for (size_t i = 0; i < ancestor.size();)
	{
		std::random_shuffle(_ancestor.begin(), _ancestor.end(), [this](const size_t n)-> size_t{std::uniform_int_distribution<size_t> dist(0, n - 1);return dist(this->GetRandom());});
		for (size_t parent1 = 0; parent1 < ancestor.size() && i < ancestor.size(); parent1 += 3, ++i)
		{
			const size_t parent2 = (parent1 + 1) % ancestor.size();
			const size_t parent3 = (parent2 + 1) % ancestor.size();
			const TIndividual &parent = ancestor[i];
			TIndividual child;
			this->GetCrossover()(parent, *_ancestor[parent1], *_ancestor[parent2], *_ancestor[parent3], child);
			this->GetMutation()(child);
			TSuper::GetProblem()(child);
			if (Dominate(parent, child))
				offspring.push_back(parent);
			else if(Dominate(child, parent))
				offspring.push_back(child);
			else
			{
				offspring.push_back(parent);
				offspring.push_back(child);
			}
		}
	}
	return offspring;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void GDE3<_TReal, _TDecision, _TRandom>::EnvironmentalSelection(TOffspring &offspring, TSolutionSet &solutionSet)
{
	typedef typename TOffspring::pointer _TPointer;
	std::list<_TPointer> _offspring;
	for (auto i = offspring.begin(); i != offspring.end(); ++i)
		_offspring.push_back(&*i);
	typedef typename TSolutionSet::iterator _TIterator;
	otl::selection::NondominateSelection(_offspring, solutionSet.begin(), solutionSet.end(), &_Dominate, &_SelectNoncritical<_TPointer, _TIterator>, &_SelectCritical<_TPointer, _TIterator>);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool GDE3<_TReal, _TDecision, _TRandom>::Dominate(const TIndividual &individual1, const TIndividual &individual2)
{
	return otl::utility::relation::Dominate(individual1.objective_, individual2.objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool GDE3<_TReal, _TDecision, _TRandom>::_Dominate(const TIndividual *individual1, const TIndividual *individual2)
{
	return Dominate(*individual1, *individual2);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void GDE3<_TReal, _TDecision, _TRandom>::_DoStep(void)
{
	TOffspring offspring = MakeOffspring(TSuper::solutionSet_);
	EnvironmentalSelection(offspring, TSuper::solutionSet_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator GDE3<_TReal, _TDecision, _TRandom>::_SelectNoncritical(const std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	std::vector<_TPointer> _front(front.begin(), front.end());
	otl::optimizer::nsga_ii::CrowdingDistanceAssignment<TReal>(_front.begin(), _front.end());
	assert(_front.size() <= std::distance(begin, end));
	_TIterator dest = begin;
	for (size_t i = 0; i < _front.size(); ++i, ++dest)
		*dest = *_front[i];
	return dest;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator GDE3<_TReal, _TDecision, _TRandom>::_SelectCritical(const std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	std::vector<_TPointer> _front(front.begin(), front.end());
	otl::optimizer::nsga_ii::CrowdingDistanceAssignment<TReal>(_front.begin(), _front.end());
	std::partial_sort(_front.begin(), _front.begin() + std::distance(begin, end), _front.end()
		, [](_TPointer individual1, _TPointer individual2)->bool{return individual1->crowdingDistance_ > individual2->crowdingDistance_;}
	);
	assert(_front.size() >= std::distance(begin, end));
	_TIterator dest = begin;
	for (size_t i = 0; dest != end; ++i, ++dest)
		*dest = *_front[i];
	return dest;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename GDE3<_TReal, _TDecision, _TRandom>::TIndividual *GDE3<_TReal, _TDecision, _TRandom>::_Compete(const std::vector<const TIndividual *> &competition)
{
	if (Dominate(*competition[0], *competition[1]))
		return competition[0];
	else if (Dominate(*competition[1], *competition[0]))
		return competition[1];
	assert(competition[0]->crowdingDistance_ >= 0);
	assert(competition[1]->crowdingDistance_ >= 0);
	return competition[0]->crowdingDistance_ > competition[1]->crowdingDistance_ ? competition[0] : competition[1];
}
}
}
}
