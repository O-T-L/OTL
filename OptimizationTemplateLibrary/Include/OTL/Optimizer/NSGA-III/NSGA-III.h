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
#include "Normalize.h"
#include "Associate.h"
#include "Niching.h"

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

	NSGA_III(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const std::vector<TPoint> &referenceSet, const TReal epsilon = 1e-6);
	~NSGA_III(void);
	const std::vector<TPoint> &GetReferenceSet(void) const;
	TSolutionSet MakeOffspring(const TSolutionSet &ancestor);
	static bool Dominate(const TIndividual &individual1, const TIndividual &individual2);

protected:
	static bool _Dominate(const TIndividual *individual1, const TIndividual *individual2);
	void _DoStep(void);
	template <typename _TPointer, typename _TIterator> _TIterator _SelectNoncritical(const std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	template <typename _TPointer, typename _TIterator> _TIterator _SelectCritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	static const TIndividual *_Compete(const std::vector<const TIndividual *> &competition);
	template <typename _TIterator> _TIterator _RandomSelection(_TIterator begin, _TIterator end);

private:
	std::vector<TPoint> referenceSet_;
	std::list<TIndividual *> noncritical_;
	TReal epsilon_;
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
	typedef typename TSolutionSet::iterator _TIterator;
	otl::selection::NondominateSelection(mix, TSuper::solutionSet_.begin(), TSuper::solutionSet_.end(), &_Dominate
		, [this](std::list<_TPointer> &front, _TIterator begin, _TIterator end)->_TIterator{return this->_SelectNoncritical(front, begin, end);}
		, [this](std::list<_TPointer> &front, _TIterator begin, _TIterator end)->_TIterator{return this->_SelectCritical(front, begin, end);}
	);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator NSGA_III<_TReal, _TDecision, _TRandom>::_SelectNoncritical(const std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	noncritical_.insert(noncritical_.end(), front.begin(), front.end());
	_TIterator dest = begin;
	for (auto i = front.begin(); i != front.end(); ++i, ++dest)
		*dest = **i;
	return dest;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator NSGA_III<_TReal, _TDecision, _TRandom>::_SelectCritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	assert(front.size() >= std::distance(begin, end));
	if (front.size() == std::distance(begin, end))
		return _SelectNoncritical(front, begin, end);
	else
	{
		std::list<TIndividual *> allFront(noncritical_.begin(), noncritical_.end());
		allFront.insert(allFront.end(), front.begin(), front.end());
		Normalize(allFront.begin(), allFront.end(), epsilon_);
		std::vector<size_t> counter = CountAssociation(Associate(noncritical_.begin(), noncritical_.end(), referenceSet_));
		auto association = Associate(front.begin(), front.end(), referenceSet_);
		typedef decltype(association.front().begin()) _TAssociationIterator;
		Niching(counter, front, association, begin, end, [this](_TAssociationIterator begin, _TAssociationIterator end)->_TAssociationIterator{return this->_RandomSelection(begin, end);});
		return end;
	}
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

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TIterator> _TIterator NSGA_III<_TReal, _TDecision, _TRandom>::_RandomSelection(_TIterator begin, _TIterator end)
{
	std::uniform_int_distribution<size_t> dist(0, std::distance(begin, end) - 1);
	_TIterator select = begin;
	for (size_t count = dist(this->GetRandom()); count; --count)
		++select;
	return select;
}
}
}
}
