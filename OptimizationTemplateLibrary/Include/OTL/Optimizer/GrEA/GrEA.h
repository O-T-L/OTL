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
  Title                    = {A Grid-Based Evolutionary Algorithm for Many-Objective Optimization},
  Author                   = {Shengxiang Yang and Miqing Li and Xiaohui Liu and Jinhua Zheng},
  Journal                  = {IEEE Transactions on Evolutionary Computation},
  Year                     = {2013},
  Month                    = {January},
  Number                   = {5},
  Pages                    = {721-736},
  Volume                   = {17},
  Doi                      = {10.1109/TEVC.2012.2227145}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <list>
#include <algorithm>
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

namespace otl
{
namespace optimizer
{
namespace grea
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class GrEA : public Metaheuristic<std::vector<Individual<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>, public otl::crossover::WithCrossover<_TReal, _TDecision>, public otl::mutation::WithMutation<_TReal, _TDecision>
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

	GrEA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const std::vector<size_t> &dividion);
	~GrEA(void);
	const std::vector<size_t> &GetDividion(void) const;
	TSolutionSet MakeOffspring(const TSolutionSet &ancestor);
	template <typename _TIterator> std::vector<std::pair<TReal, TReal> > CalculateRanges(_TIterator begin, _TIterator end) const;
	template <typename _TIterator> void AssignFitness(_TIterator begin, _TIterator end) const;
	static bool Dominate(const TIndividual &individual1, const TIndividual &individual2);
	static bool GridDominate(const TIndividual &individual1, const TIndividual &individual2);

protected:
	static bool _Dominate(const TIndividual *individual1, const TIndividual *individual2);
	static bool _GridDominate(const TIndividual *individual1, const TIndividual *individual2);
	void _DoStep(void);
	template <typename _TPointer, typename _TIterator> static _TIterator _SelectNoncritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	template <typename _TPointer, typename _TIterator> static _TIterator _SelectCritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	static const TIndividual *_Compete(const std::vector<const TIndividual *> &competition);

private:
	std::vector<size_t> dividion_;
};

template <typename _TReal, typename _TDecision, typename _TRandom>
GrEA<_TReal, _TDecision, _TRandom>::GrEA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const std::vector<size_t> &dividion)
	: TSuper(problem)
	, otl::utility::WithRandom<TRandom>(random)
	, otl::crossover::WithCrossover<TReal, TDecision>(crossover)
	, otl::mutation::WithMutation<TReal, TDecision>(mutation)
	, dividion_(dividion)
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
	AssignFitness(solutionSet.begin(), solutionSet.end());
}

template <typename _TReal, typename _TDecision, typename _TRandom>
GrEA<_TReal, _TDecision, _TRandom>::~GrEA(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const std::vector<size_t> &GrEA<_TReal, _TDecision, _TRandom>::GetDividion(void) const
{
	return dividion_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
typename GrEA<_TReal, _TDecision, _TRandom>::TSolutionSet GrEA<_TReal, _TDecision, _TRandom>::MakeOffspring(const TSolutionSet &ancestor)
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
template <typename _TIterator> std::vector<std::pair<_TReal, _TReal> > GrEA<_TReal, _TDecision, _TRandom>::CalculateRanges(_TIterator begin, _TIterator end) const
{
	std::vector<std::pair<TReal, TReal> > ranges(TSuper::GetProblem().GetNumberOfObjectives());
	for (size_t i = 0; i < ranges.size(); ++i)
	{
		auto minmax = std::minmax_element(begin, end, [i](const TIndividual *individual1, const TIndividual *individual2)->bool{return individual1->objective_[i] < individual2->objective_[i];});
		ranges[i] = ExpandHalfBox((**minmax.first).objective_[i], (**minmax.second).objective_[i], dividion_[i]);
	}
	return ranges;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TIterator> void GrEA<_TReal, _TDecision, _TRandom>::AssignFitness(_TIterator begin, _TIterator end) const
{
	const auto ranges = CalculateRanges(begin, end);
	for (_TIterator i = begin; i != end; ++i)
	{
		TIndividual &individual = **i;
		individual.gridCoordinate_ = GridCoordinate(ranges, individual.objective_);
		individual.gcd_ = 0;
	}
	for (_TIterator i = begin; i != end; ++i)
	{
		TIndividual &individual = **i;
		individual.gr_ = std::accumulate(individual.gridCoordinate_.begin(), individual.gridCoordinate_.end(), (TReal)0);
		for (_TIterator j = ++_TIterator(i); j != end; ++j)
		{
			TIndividual &_individual = **j;
			const size_t gd = GridDifference(individual.gridCoordinate_, _individual.gridCoordinate_);
			if (gd < individual.gridCoordinate_.size())
			{
				const size_t gcd = individual.gridCoordinate_.size() - gd;
				individual.gcd_ += gcd;
				_individual.gcd_ += gcd;
			}
		}
		individual.gcpd_ = GridCoordinatePointDistance(ranges, individual);
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool GrEA<_TReal, _TDecision, _TRandom>::Dominate(const TIndividual &individual1, const TIndividual &individual2)
{
	return otl::utility::relation::Dominate(individual1.objective_, individual2.objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool GrEA<_TReal, _TDecision, _TRandom>::GridDominate(const TIndividual &individual1, const TIndividual &individual2)
{
	return otl::utility::relation::Dominate(individual1.gridCoordinate_, individual2.gridCoordinate_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool GrEA<_TReal, _TDecision, _TRandom>::_Dominate(const TIndividual *individual1, const TIndividual *individual2)
{
	return Dominate(*individual1, *individual2);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool GrEA<_TReal, _TDecision, _TRandom>::_GridDominate(const TIndividual *individual1, const TIndividual *individual2)
{
	return GridDominate(*individual1, *individual2);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void GrEA<_TReal, _TDecision, _TRandom>::_DoStep(void)
{
	TSolutionSet ancestor = TSuper::solutionSet_;
	TSolutionSet offspring = MakeOffspring(ancestor);
	typedef typename TSolutionSet::pointer _TPointer;
	std::list<_TPointer> mix;
	for (size_t i = 0; i < ancestor.size(); ++i)
		mix.push_back(&ancestor[i]);
	for (size_t i = 0; i < offspring.size(); ++i)
		mix.push_back(&offspring[i]);
	AssignFitness(mix.begin(), mix.end());
	typedef typename TSolutionSet::iterator _TIterator;
	otl::selection::NondominateSelection(mix, TSuper::solutionSet_.begin(), TSuper::solutionSet_.end(), &_Dominate, &_SelectNoncritical<_TPointer, _TIterator>, &_SelectCritical<_TPointer, _TIterator>);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator GrEA<_TReal, _TDecision, _TRandom>::_SelectNoncritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	assert(front.size() <= std::distance(begin, end));
	_TIterator dest = begin;
	for (auto i = front.begin(); i != front.end(); ++i, ++dest)
		*dest = **i;
	return dest;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator GrEA<_TReal, _TDecision, _TRandom>::_SelectCritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	for (_TIterator dest = begin; dest != end; ++dest)
	{
		auto best = LocateBest(front.begin(), front.end());
		_TPointer elite = *best;
		front.erase(best);
		*dest = *elite;
		AdjustGCD(*elite, front.begin(), front.end());
		AdjustGR(*elite, front, &GridDominate);
	}
	return end;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename GrEA<_TReal, _TDecision, _TRandom>::TIndividual *GrEA<_TReal, _TDecision, _TRandom>::_Compete(const std::vector<const TIndividual *> &competition)
{
	assert(competition[0]->gr_ != -1);
	assert(competition[1]->gr_ != -1);
	assert(competition[0]->gcd_ != -1);
	assert(competition[1]->gcd_ != -1);
	if (Dominate(*competition[0], *competition[1]) || GridDominate(*competition[0], *competition[1]))
		return competition[0];
	else if (Dominate(*competition[1], *competition[0]) || GridDominate(*competition[1], *competition[0]))
		return competition[1];
	return competition[0]->gcd_ < competition[1]->gcd_ ? competition[0] : competition[1];
}
}
}
}
