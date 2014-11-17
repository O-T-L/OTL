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
  author = {Nicola Beume and Boris Naujoks and Michael Emmerich},
  title = {{SMS-EMOA}: Multiobjective selection based on dominated hypervolume},
  journal = {European Journal of Operational Research},
  year = {2007},
  volume = {181},
  pages = {1653-1669},
  number = {3},
  doi = {10.1016/j.ejor.2006.08.008}
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
#include <OTL/Optimizer/SPEA2/RawFitness.h>
#include "ContributionAssignment.h"
#include "CalculateReferencePoint.h"
#include "Individual.h"

namespace otl
{
namespace optimizer
{
namespace sms_emoa
{
template <typename _TReal, typename _TDecision, typename _TRandom, typename _TMakeHypervolume>
class SMS_EMOA : public Metaheuristic<std::vector<Individual<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef _TMakeHypervolume TMakeHypervolume;
	typedef Individual<TReal, TDecision> TIndividual;
	typedef std::vector<TIndividual> TSolutionSet;
	typedef Metaheuristic<TSolutionSet> TSuper;
	typedef typename TSuper::TProblem TProblem;

	SMS_EMOA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, _TMakeHypervolume makeHypervolume);
	~SMS_EMOA(void);
	std::vector<const TIndividual *> MatingSelection(const size_t offspringSize, const TSolutionSet &ancestor);
	static bool Dominate(const TIndividual &individual1, const TIndividual &individual2);

protected:
	static bool _Dominate(const TIndividual *individual1, const TIndividual *individual2);
	static const TIndividual *_Compete(const std::vector<const TIndividual *> &competition);
	void _Reduce(TSolutionSet &population, TSolutionSet &solutionSet);
	template <typename _TPointer, typename _TIterator> _TIterator _SelectNoncritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	template <typename _TPointer, typename _TIterator> _TIterator _SelectCritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end) const;

private:
	_TMakeHypervolume makeHypervolume_;
	std::list<TIndividual *> noncritical_;
};

template <typename _TReal, typename _TDecision, typename _TRandom, typename _TMakeHypervolume>
SMS_EMOA<_TReal, _TDecision, _TRandom, _TMakeHypervolume>::SMS_EMOA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, _TMakeHypervolume makeHypervolume)
	: TSuper(problem)
	, otl::utility::WithRandom<TRandom>(random)
	, makeHypervolume_(makeHypervolume)
{
#ifndef NDEBUG
	nondominateRank_ = -1;
#endif
	TSuper::solutionSet_.resize(initial.size());
	for (size_t i = 0; i < initial.size(); ++i)
	{
		TIndividual &individual = TSuper::solutionSet_[i];
		individual.decision_ = initial[i];
		TSuper::GetProblem()(individual);
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom, typename _TMakeHypervolume>
SMS_EMOA<_TReal, _TDecision, _TRandom, _TMakeHypervolume>::~SMS_EMOA(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom, typename _TMakeHypervolume>
std::vector<const typename SMS_EMOA<_TReal, _TDecision, _TRandom, _TMakeHypervolume>::TIndividual *> SMS_EMOA<_TReal, _TDecision, _TRandom, _TMakeHypervolume>::MatingSelection(const size_t offspringSize, const TSolutionSet &ancestor)
{
	return otl::optimizer::nsga_ii::MatingSelection(offspringSize, ancestor.begin(), ancestor.end(), this->GetRandom(), &_Compete);
}

template <typename _TReal, typename _TDecision, typename _TRandom, typename _TMakeHypervolume>
bool SMS_EMOA<_TReal, _TDecision, _TRandom, _TMakeHypervolume>::Dominate(const TIndividual &individual1, const TIndividual &individual2)
{
	return otl::utility::relation::Dominate(individual1.objective_, individual2.objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom, typename _TMakeHypervolume>
bool SMS_EMOA<_TReal, _TDecision, _TRandom, _TMakeHypervolume>::_Dominate(const TIndividual *individual1, const TIndividual *individual2)
{
	return Dominate(*individual1, *individual2);
}

template <typename _TReal, typename _TDecision, typename _TRandom, typename _TMakeHypervolume>
const typename SMS_EMOA<_TReal, _TDecision, _TRandom, _TMakeHypervolume>::TIndividual *SMS_EMOA<_TReal, _TDecision, _TRandom, _TMakeHypervolume>::_Compete(const std::vector<const TIndividual *> &competition)
{
	if (Dominate(*competition[0], *competition[1]))
		return competition[0];
	else if (Dominate(*competition[1], *competition[0]))
		return competition[1];
	return competition[0];
}

template <typename _TReal, typename _TDecision, typename _TRandom, typename _TMakeHypervolume>
void SMS_EMOA<_TReal, _TDecision, _TRandom, _TMakeHypervolume>::_Reduce(TSolutionSet &population, TSolutionSet &solutionSet)
{
	typedef typename TSolutionSet::value_type _TIndividual;
	typedef typename TSolutionSet::pointer _TPointer;
	typedef typename TSolutionSet::iterator _TIterator;
	assert(solutionSet.size() < population.size());
	std::list<_TPointer> _population;
	for (size_t i = 0; i < population.size(); ++i)
		_population.push_back(&population[i]);
	noncritical_.clear();
	otl::selection::NondominateSelection(_population, solutionSet.begin(), solutionSet.end(), &_Dominate
		, [this](std::list<_TPointer> &front, _TIterator begin, _TIterator end)->_TIterator{return this->_SelectNoncritical(front, begin, end);}
		, [this](std::list<_TPointer> &front, _TIterator begin, _TIterator end)->_TIterator{return this->_SelectCritical(front, begin, end);}
	);
}

template <typename _TReal, typename _TDecision, typename _TRandom, typename _TMakeHypervolume>
template <typename _TPointer, typename _TIterator> _TIterator SMS_EMOA<_TReal, _TDecision, _TRandom, _TMakeHypervolume>::_SelectNoncritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	assert(front.size() <= std::distance(begin, end));
	_TIterator dest = begin;
	for (auto i = front.begin(); i != front.end(); ++i, ++dest)
		*dest = **i;
	noncritical_.splice(noncritical_.end(), front, front.begin(), front.end());
	return dest;
}

template <typename _TReal, typename _TDecision, typename _TRandom, typename _TMakeHypervolume>
template <typename _TPointer, typename _TIterator> _TIterator SMS_EMOA<_TReal, _TDecision, _TRandom, _TMakeHypervolume>::_SelectCritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end) const
{
	if (noncritical_.empty())
	{
		const std::vector<_TReal> referencePoint = CalculateUpperReferencePoint<_TReal>(front.begin(), front.end());
		ContributionAssignment(front.begin(), front.end(), referencePoint, makeHypervolume_);
		std::vector<_TPointer> _front(front.begin(), front.end());
		std::partial_sort(_front.begin(), _front.begin() + std::distance(begin, end), _front.end()
			, [](_TPointer individual1, _TPointer individual2)->bool{return individual1->hvContribution_ > individual2->hvContribution_;}
		);
		assert(_front.size() >= std::distance(begin, end));
		_TIterator dest = begin;
		for (size_t i = 0; dest != end; ++i, ++dest)
			*dest = *_front[i];
		return dest;
	}
	else
	{
		auto population = noncritical_;
		population.insert(population.end(), front.begin(), front.end());
		for (auto i = front.begin(); i != front.end(); ++i)
			(**i).dominatingPoints_ = otl::optimizer::spea2::RawFitness(i, population.begin(), population.end(), &_Dominate);
		std::vector<_TPointer> _front(front.begin(), front.end());
		std::partial_sort(_front.begin(), _front.begin() + std::distance(begin, end), _front.end()
			, [](_TPointer individual1, _TPointer individual2)->bool{return individual1->dominatingPoints_ > individual2->dominatingPoints_;}
		);
		_TIterator dest = begin;
		for (size_t i = 0; dest != end; ++i, ++dest)
			*dest = *_front[i];
		return dest;
	}
}
}
}
}
