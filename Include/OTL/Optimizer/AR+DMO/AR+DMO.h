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
  author = {Salem F. Adra and Peter J. Fleming},
  title = {A Diversity Management Operator for Evolutionary Many-Objective Optimisation},
  booktitle = {Evolutionary Multi-Criterion Optimization},
  year = {2009},
  pages = {81-94},
  month = {7-10 April},
  publisher = {Springer},
  doi = {10.1007/978-3-642-01020-0_11}
}

@Conference{,
  Title                    = {Enhancing Diversity for Average Ranking Method in Evolutionary Many-Objective Optimization},
  Author                   = {Miqing Li and Jinhua Zheng and Ke Li and Qizhao Yuan and Ruimin Shen},
  Booktitle                = {Parallel Problem Solving from Nature},
  Year                     = {2010},
  Month                    = {September 11-15},
  Pages                    = {647-656},
  Publisher                = {Springer},
  Doi                      = {10.1007/978-3-642-15844-5_65}
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
#include <OTL/Optimizer/AR/AverageRanking.h>
#include <OTL/Optimizer/NSGA-II/CrowdingDistanceAssignment.h>
#include <OTL/Indicator/MS/MaximumSpread1.h>
#include "Individual.h"

namespace otl
{
namespace optimizer
{
namespace ar_dmo
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class AR_DMO : public Metaheuristic<std::vector<Individual<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>, public otl::crossover::WithCrossover<_TReal, _TDecision>, public otl::mutation::WithMutation<_TReal, _TDecision>
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
	typedef otl::indicator::ms::MaximumSpread1<TReal> TMaximumSpread;
	typedef typename TMaximumSpread::TFront TFront;
	typedef typename TMaximumSpread::TRange TRange;
	typedef typename TMaximumSpread::TBoundary TBoundary;

	AR_DMO(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const TBoundary &boundary);
	~AR_DMO(void);
	TSolutionSet MakeOffspring(const TSolutionSet &ancestor);
	static bool Dominate(const TIndividual &individual1, const TIndividual &individual2);

protected:
	static bool _Dominate(const TIndividual *individual1, const TIndividual *individual2);
	void _DoStep(void);
	template <typename _TPointer, typename _TIterator> static _TIterator _SelectNoncritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	template <typename _TPointer, typename _TIterator> _TIterator _SelectCritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end, const bool randomSelection);
	static const TIndividual *_Compete(const std::vector<const TIndividual *> &competition);
	template <typename _TIterator> bool _ShouldRandomSelect(_TIterator begin, _TIterator end);

private:
	TMaximumSpread maximumSpread_;
};

template <typename _TReal, typename _TDecision, typename _TRandom>
AR_DMO<_TReal, _TDecision, _TRandom>::AR_DMO(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const TBoundary &boundary)
	: TSuper(problem)
	, otl::utility::WithRandom<TRandom>(random)
	, otl::crossover::WithCrossover<TReal, TDecision>(crossover)
	, otl::mutation::WithMutation<TReal, TDecision>(mutation)
	, maximumSpread_(boundary)
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
	std::vector<_TPointer> _solutionSet(solutionSet.begin(), solutionSet.end());
	otl::optimizer::ar::AverageRanking(_solutionSet.begin(), _solutionSet.end());
	while (!solutionSet.empty())
	{
		std::list<_TPointer> nondominate = otl::utility::ExtractNondominate(solutionSet, &_Dominate);
		std::vector<_TPointer> _nondominate(nondominate.begin(), nondominate.end());
		otl::optimizer::nsga_ii::CrowdingDistanceAssignment<TReal>(_nondominate.begin(), _nondominate.end());
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
AR_DMO<_TReal, _TDecision, _TRandom>::~AR_DMO(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
typename AR_DMO<_TReal, _TDecision, _TRandom>::TSolutionSet AR_DMO<_TReal, _TDecision, _TRandom>::MakeOffspring(const TSolutionSet &ancestor)
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
bool AR_DMO<_TReal, _TDecision, _TRandom>::Dominate(const TIndividual &individual1, const TIndividual &individual2)
{
	return otl::utility::relation::Dominate(individual1.objective_, individual2.objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool AR_DMO<_TReal, _TDecision, _TRandom>::_Dominate(const TIndividual *individual1, const TIndividual *individual2)
{
	return Dominate(*individual1, *individual2);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void AR_DMO<_TReal, _TDecision, _TRandom>::_DoStep(void)
{
	TSolutionSet ancestor = TSuper::solutionSet_;
	TSolutionSet offspring = MakeOffspring(ancestor);
	typedef typename TSolutionSet::pointer _TPointer;
	const bool randomSelection = _ShouldRandomSelect(TSuper::solutionSet_.begin(), TSuper::solutionSet_.end());
	std::list<_TPointer> mix;
	for (size_t i = 0; i < ancestor.size(); ++i)
		mix.push_back(&ancestor[i]);
	for (size_t i = 0; i < offspring.size(); ++i)
		mix.push_back(&offspring[i]);
	std::vector<_TPointer> _mix(mix.begin(), mix.end());
	otl::optimizer::ar::AverageRanking(_mix.begin(), _mix.end());
	typedef typename TSolutionSet::iterator _TIterator;
	otl::selection::NondominateSelection(mix, TSuper::solutionSet_.begin(), TSuper::solutionSet_.end(), &_Dominate, &_SelectNoncritical<_TPointer, _TIterator>
		, [this, randomSelection](std::list<_TPointer> &front, _TIterator begin, _TIterator end)->_TIterator{return this->_SelectCritical(front, begin, end, randomSelection);}
	);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator AR_DMO<_TReal, _TDecision, _TRandom>::_SelectNoncritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	std::vector<_TPointer> _front(front.begin(), front.end());
	otl::optimizer::nsga_ii::CrowdingDistanceAssignment<TReal>(_front.begin(), _front.end());
	_TIterator dest = begin;
	for (size_t i = 0; i < _front.size() && dest != end; ++i, ++dest)
		*dest = *_front[i];
	return dest;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator AR_DMO<_TReal, _TDecision, _TRandom>::_SelectCritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end, const bool randomSelection)
{
	if (randomSelection)
	{
		std::vector<_TPointer> _front(front.begin(), front.end());
		otl::optimizer::nsga_ii::CrowdingDistanceAssignment<TReal>(_front.begin(), _front.end());
		std::random_shuffle(_front.begin(), _front.end(), [this](const size_t n)-> size_t{return std::uniform_int_distribution<size_t> (0, n - 1)(this->GetRandom());});
		_TIterator dest = begin;
		for (size_t i = 0; dest != end; ++i, ++dest)
			*dest = *_front[i];
		return dest;
	}
	else
	{
		std::vector<_TPointer> _front(front.begin(), front.end());
		otl::optimizer::nsga_ii::CrowdingDistanceAssignment<TReal>(_front.begin(), _front.end());
		std::partial_sort(_front.begin(), _front.begin() + std::distance(begin, end), _front.end()
			, [](_TPointer individual1, _TPointer individual2)->bool{return individual1->crowdingDistance_ > individual2->crowdingDistance_;}
		);
		_TIterator dest = begin;
		for (size_t i = 0; i < _front.size() && dest != end; ++i, ++dest)
			*dest = *_front[i];
		return dest;
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename AR_DMO<_TReal, _TDecision, _TRandom>::TIndividual *AR_DMO<_TReal, _TDecision, _TRandom>::_Compete(const std::vector<const TIndividual *> &competition)
{
	if (Dominate(*competition[0], *competition[1]))
		return competition[0];
	else if (Dominate(*competition[1], *competition[0]))
		return competition[1];
	assert(competition[0]->averageRank_ != -1);
	assert(competition[1]->averageRank_ != -1);
	if (competition[0]->averageRank_ < competition[1]->averageRank_)
		return competition[0];
	else if (competition[1]->averageRank_ < competition[0]->averageRank_)
		return competition[1];
	assert(competition[0]->crowdingDistance_ >= 0);
	assert(competition[1]->crowdingDistance_ >= 0);
	return competition[0]->crowdingDistance_ > competition[1]->crowdingDistance_ ? competition[0] : competition[1];
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TIterator> bool AR_DMO<_TReal, _TDecision, _TRandom>::_ShouldRandomSelect(_TIterator begin, _TIterator end)
{
	TFront front(std::distance(begin, end));
	size_t index = 0;
	for (_TIterator i = begin; i != end; ++i)
	{
		front[index] = &i->objective_;
		++index;
	}
	return maximumSpread_(front) > 1;
}
}
}
}
