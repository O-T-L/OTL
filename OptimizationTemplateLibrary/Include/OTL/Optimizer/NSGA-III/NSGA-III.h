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
@Article{,
  Title                    = {An Evolutionary Many-Objective Optimization Algorithm Using Reference-point Based Non-dominated Sorting Approach, Part {I}: Solving Problems with Box Constraints},
  Author                   = {Kalyanmoy Deb and Himanshu Jain},
  Journal                  = {IEEE Transactions on Evolutionary Computation},
  Year                     = {2014},
  Month                    = {16 September},
  Number                   = {4},
  Pages                    = {577-601},
  Volume                   = {18},
  Doi                      = {10.1109/TEVC.2013.2281535}
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
#include <OTL/Optimizer/NSGA-III/Utility.h>
#include "Individual.h"
#include "Normalize.h"
#include "Utility.h"

namespace otl
{
namespace optimizer
{
namespace nsga_iii
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class NSGA_III : public Metaheuristic<std::vector<Individual<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>, public otl::crossover::WithCrossover<_TReal, _TDecision>, public otl::mutation::WithMutation<_TReal, _TDecision>
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
	typedef std::vector<TReal> TPoint;
	typedef std::pair<size_t, std::list<const TIndividual *> > TNiche;

	NSGA_III(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const std::vector<TPoint> &referenceSet, const TReal epsilon = 1e-6);
	~NSGA_III(void);
	const std::vector<TPoint> &GetReferenceSet(void) const;
	TReal GetEpsilon(void) const;
	TSolutionSet MakeOffspring(const TSolutionSet &ancestor);
	static bool Dominate(const TIndividual &individual1, const TIndividual &individual2);

protected:
	static bool _Dominate(const TIndividual *individual1, const TIndividual *individual2);
	void _DoStep(void);
	template <typename _TPointer, typename _TIterator> _TIterator _SelectNoncritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	template <typename _TPointer, typename _TIterator> _TIterator _SelectCritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	size_t _NearestReferencePoint(TIndividual &individual) const;
	template <typename _TIterator> std::vector<std::list<const TIndividual *> > _Associate(_TIterator begin, _TIterator end) const;
	template <typename _TIterator> _TIterator _Niching(std::list<TNiche> &niches, _TIterator begin, _TIterator end);
	template <typename _TIterator> _TIterator _SmallestNiche(_TIterator begin, _TIterator end);
	static const TIndividual *_Compete(const std::vector<const TIndividual *> &competition);

private:
	std::vector<TPoint> referenceSet_;
	std::list<TIndividual *> noncritical_;
	const TReal epsilon_;
};

template <typename _TReal, typename _TDecision, typename _TRandom>
NSGA_III<_TReal, _TDecision, _TRandom>::NSGA_III(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const std::vector<TPoint> &referenceSet, const TReal epsilon)
	: TSuper(problem)
	, otl::utility::WithRandom<TRandom>(random)
	, otl::crossover::WithCrossover<TReal, TDecision>(crossover)
	, otl::mutation::WithMutation<TReal, TDecision>(mutation)
	, referenceSet_(referenceSet)
	, epsilon_(epsilon)
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
NSGA_III<_TReal, _TDecision, _TRandom>::~NSGA_III(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const std::vector<typename NSGA_III<_TReal, _TDecision, _TRandom>::TPoint> &NSGA_III<_TReal, _TDecision, _TRandom>::GetReferenceSet(void) const
{
	return referenceSet_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal NSGA_III<_TReal, _TDecision, _TRandom>::GetEpsilon(void) const
{
	return epsilon_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
typename NSGA_III<_TReal, _TDecision, _TRandom>::TSolutionSet NSGA_III<_TReal, _TDecision, _TRandom>::MakeOffspring(const TSolutionSet &ancestor)
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
bool NSGA_III<_TReal, _TDecision, _TRandom>::Dominate(const TIndividual &individual1, const TIndividual &individual2)
{
	return otl::utility::relation::Dominate(individual1.objective_, individual2.objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool NSGA_III<_TReal, _TDecision, _TRandom>::_Dominate(const TIndividual *individual1, const TIndividual *individual2)
{
	return Dominate(*individual1, *individual2);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void NSGA_III<_TReal, _TDecision, _TRandom>::_DoStep(void)
{
	TSolutionSet ancestor = TSuper::solutionSet_;
	TSolutionSet offspring = MakeOffspring(ancestor);
	typedef typename TSolutionSet::pointer _TPointer;
	std::list<_TPointer> mix;
	for (size_t i = 0; i < ancestor.size(); ++i)
		mix.push_back(&ancestor[i]);
	for (size_t i = 0; i < offspring.size(); ++i)
		mix.push_back(&offspring[i]);
	noncritical_.clear();
	typedef typename TSolutionSet::iterator _TIterator;
	otl::selection::NondominateSelection(mix, TSuper::solutionSet_.begin(), TSuper::solutionSet_.end(), &_Dominate
		, [this](std::list<_TPointer> &front, _TIterator begin, _TIterator end)->_TIterator{return this->_SelectNoncritical(front, begin, end);}
		, [this](std::list<_TPointer> &front, _TIterator begin, _TIterator end)->_TIterator{return this->_SelectCritical(front, begin, end);}
	);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator NSGA_III<_TReal, _TDecision, _TRandom>::_SelectNoncritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	_TIterator dest = begin;
	for (auto i = front.begin(); i != front.end(); ++i, ++dest)
		*dest = **i;
	noncritical_.splice(noncritical_.end(), front, front.begin(), front.end());
	return dest;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator NSGA_III<_TReal, _TDecision, _TRandom>::_SelectCritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	assert(front.size() >= std::distance(begin, end));
	if (front.size() == std::distance(begin, end))
		return _SelectNoncritical(front, begin, end);
	{
		std::list<_TPointer> population(noncritical_.begin(), noncritical_.end());
		population.insert(population.end(), front.begin(), front.end());
		Normalize(population.begin(), population.end(), epsilon_);
	}
	auto association1 = _Associate(noncritical_.begin(), noncritical_.end());
	assert(association1.size() == referenceSet_.size());
	auto association2 = _Associate(front.begin(), front.end());
	assert(association2.size() == referenceSet_.size());
	std::list<TNiche> niches;
	for (size_t i = 0; i < referenceSet_.size(); ++i)
	{
		auto &individuals = association2[i];
		if (!individuals.empty())
		{
			niches.push_back(TNiche());
			TNiche &niche = niches.back();
			niche.first = association1[i].size();
			niche.second.splice(niche.second.end(), individuals, individuals.begin(), individuals.end());
		}
	}
	return _Niching(niches, begin, end);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
size_t NSGA_III<_TReal, _TDecision, _TRandom>::_NearestReferencePoint(TIndividual &individual) const
{
	size_t index = 0;
	_TReal minDistance = Distance(referenceSet_[index], individual.translatedObjective_);
	for (size_t i = 1; i < referenceSet_.size(); ++i)
	{
		const _TReal distance = Distance(referenceSet_[i], individual.translatedObjective_);
		if (distance < minDistance)
		{
			index = i;
			minDistance = distance;
		}
	}
	individual.minDistance_ = minDistance;
	return index;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TIterator> std::vector<std::list<const typename NSGA_III<_TReal, _TDecision, _TRandom>::TIndividual *> > NSGA_III<_TReal, _TDecision, _TRandom>::_Associate(_TIterator begin, _TIterator end) const
{
	std::vector<std::list<const TIndividual *> > association(referenceSet_.size());
	for (_TIterator i = begin; i != end; ++i)
	{
		const size_t index = _NearestReferencePoint(**i);
		association[index].push_back(*i);
	}
	return association;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TIterator> _TIterator NSGA_III<_TReal, _TDecision, _TRandom>::_Niching(std::list<TNiche> &niches, _TIterator begin, _TIterator end)
{
	_TIterator dest = begin;
	while (dest != end)
	{
		auto _niche = _SmallestNiche(niches.begin(), niches.end());
		TNiche &niche = *_niche;
		assert(!niche.second.empty());
		auto elite = niche.first ? RandomSelect(this->GetRandom(), niche.second.begin(), niche.second.end()) : std::min_element(niche.second.begin(), niche.second.end(), [](const TIndividual *individual1, const TIndividual *individual2)->bool{return individual1->minDistance_ < individual2->minDistance_;});
		*dest = **elite;
		niche.second.erase(elite);
		if (niche.second.empty())
			niches.erase(_niche);
		else
			++niche.first;
		++dest;
	}
	return dest;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TIterator> _TIterator NSGA_III<_TReal, _TDecision, _TRandom>::_SmallestNiche(_TIterator begin, _TIterator end)
{
	assert(begin != end);
	const size_t size = std::min_element(begin, end, [](const TNiche &niche1, const TNiche &niche2)->bool{return niche1.first < niche2.first;})->first;
	std::list<_TIterator> niches;
	for (_TIterator i = begin; i != end; ++i)
	{
		assert(i->first >= size);
		assert(!i->second.empty());
		if (i->first == size)
			niches.push_back(i);
	}
	assert(!niches.empty());
	return *RandomSelect(this->GetRandom(), niches.begin(), niches.end());
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename NSGA_III<_TReal, _TDecision, _TRandom>::TIndividual *NSGA_III<_TReal, _TDecision, _TRandom>::_Compete(const std::vector<const TIndividual *> &competition)
{
	if (Dominate(*competition[0], *competition[1]))
		return competition[0];
	else if (Dominate(*competition[1], *competition[0]))
		return competition[1];
	return competition[0];
}
}
}
}
