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

	GrEA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const std::vector<size_t> &division);
	~GrEA(void);
	const std::vector<size_t> &GetDivision(void) const;
	TSolutionSet MakeOffspring(const TSolutionSet &ancestor);
	template <typename _TIterator> void GridSetting(_TIterator begin, _TIterator end);
	template <typename _TIterator> void CalculateFitness(_TIterator begin, _TIterator end) const;
	static bool Dominate(const TIndividual &individual1, const TIndividual &individual2);

protected:
	static bool _Dominate(const TIndividual *individual1, const TIndividual *individual2);
	void _DoStep(void);
	template <typename _TPointer, typename _TIterator> static _TIterator _SelectNoncritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	template <typename _TPointer, typename _TIterator> _TIterator _SelectCritical(std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	static const TIndividual *_Compete(const std::vector<const TIndividual *> &competition);

private:
	std::vector<size_t> division_;
	std::vector<std::pair<TReal, TReal> > boundary_;
};

template <typename _TReal, typename _TDecision, typename _TRandom>
GrEA<_TReal, _TDecision, _TRandom>::GrEA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const std::vector<size_t> &division)
	: TSuper(problem)
	, otl::utility::WithRandom<TRandom>(random)
	, otl::crossover::WithCrossover<TReal, TDecision>(crossover)
	, otl::mutation::WithMutation<TReal, TDecision>(mutation)
	, division_(division)
	, boundary_(problem.GetNumberOfObjectives())
{
	TSuper::solutionSet_.resize(initial.size());
	for (size_t i = 0; i < initial.size(); ++i)
	{
		TIndividual &individual = TSuper::solutionSet_[i];
		individual.decision_ = initial[i];
		TSuper::GetProblem()(individual);
	}
#if 1
	typedef typename TSolutionSet::pointer _TPointer;
	std::list<_TPointer> solutionSet;
	for (size_t i = 0; i < TSuper::solutionSet_.size(); ++i)
		solutionSet.push_back(&TSuper::solutionSet_[i]);
	GridSetting(solutionSet.begin(), solutionSet.end());
	CalculateFitness(solutionSet.begin(), solutionSet.end());
	CalculateGCD(solutionSet.begin(), solutionSet.end());
#endif
}

template <typename _TReal, typename _TDecision, typename _TRandom>
GrEA<_TReal, _TDecision, _TRandom>::~GrEA(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const std::vector<size_t> &GrEA<_TReal, _TDecision, _TRandom>::GetDivision(void) const
{
	return division_;
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
template <typename _TIterator> void GrEA<_TReal, _TDecision, _TRandom>::GridSetting(_TIterator begin, _TIterator end)
{
	for (size_t i = 0; i < boundary_.size(); ++i)
	{
		auto minmax = std::minmax_element(begin, end, [i](const TIndividual *individual1, const TIndividual *individual2)->bool{return individual1->objective_[i] < individual2->objective_[i];});
#if 1
		boundary_[i] = ExpandHalfBox((**minmax.first).objective_[i], (**minmax.second).objective_[i], division_[i]);
#else
		auto &range = boundary_[i];
		range.first = (**minmax.first).objective_[i];
		range.second = ((**minmax.second).objective_[i] - range.first) / division_[i];
#endif
	}
	for (_TIterator i = begin; i != end; ++i)
		(**i).gridCoordinate_ = GridCoordinate(boundary_, (**i).objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TIterator> void GrEA<_TReal, _TDecision, _TRandom>::CalculateFitness(_TIterator begin, _TIterator end) const
{
	for (_TIterator i = begin; i != end; ++i)
	{
		TIndividual &individual = **i;
		individual.gr_ = std::accumulate(individual.gridCoordinate_.begin(), individual.gridCoordinate_.end(), (TReal)0);
		individual.gcpd_ = GridCoordinatePointDistance(boundary_, individual);
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool GrEA<_TReal, _TDecision, _TRandom>::Dominate(const TIndividual &individual1, const TIndividual &individual2)
{
	return otl::utility::relation::Dominate(individual1.objective_, individual2.objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool GrEA<_TReal, _TDecision, _TRandom>::_Dominate(const TIndividual *individual1, const TIndividual *individual2)
{
	return Dominate(*individual1, *individual2);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void GrEA<_TReal, _TDecision, _TRandom>::_DoStep(void)
{
	TSolutionSet ancestor = TSuper::solutionSet_;
	typedef typename TSolutionSet::pointer _TPointer;
	std::list<_TPointer> mix;
	for (size_t i = 0; i < ancestor.size(); ++i)
		mix.push_back(&ancestor[i]);
	GridSetting(mix.begin(), mix.end());
	CalculateGCD(mix.begin(), mix.end());
	TSolutionSet offspring = MakeOffspring(ancestor);
	for (size_t i = 0; i < offspring.size(); ++i)
		mix.push_back(&offspring[i]);
	typedef typename TSolutionSet::iterator _TIterator;
	otl::selection::NondominateSelection(mix, TSuper::solutionSet_.begin(), TSuper::solutionSet_.end(), &_Dominate
		, &_SelectNoncritical<_TPointer, _TIterator>
		, [this](std::list<_TPointer> &front, _TIterator begin, _TIterator end)->_TIterator{return _SelectCritical(front, begin, end);}
	);
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
	GridSetting(front.begin(), front.end());
	CalculateFitness(front.begin(), front.end());
	for (auto i = front.begin(); i != front.end(); ++i)
		(**i).gcd_ = 0;
	for (_TIterator dest = begin; dest != end; ++dest)
	{
		auto _elite = FindoutBest(front.begin(), front.end());
		_TPointer elite = *_elite;
		front.erase(_elite);
		*dest = *elite;
		AdjustGCD(*elite, front.begin(), front.end());
		AdjustGR(*elite, front);
#if 0
		for (_TIterator i = begin; i != dest; ++i)
		{
			auto &objective = i->objective_;
			for (size_t j = 0; j < objective.size(); ++j)
				std::cout << objective[j] << '\t';
			std::cout << 'v' << '\t' << i->gr_ << '\t' << i->gcd_ << '\t' << i->gcpd_ << std::endl;
		}
		{
			_TIterator i = dest;
			auto &objective = i->objective_;
			for (size_t j = 0; j < objective.size(); ++j)
				std::cout << objective[j] << '\t';
			std::cout << '*' << '\t' << i->gr_ << '\t' << i->gcd_ << '\t' << i->gcpd_ << std::endl;
		}
		for (auto t = front.begin(); t != front.end(); ++t)
		{
			auto i = *t;
			auto &objective = i->objective_;
			for (size_t j = 0; j < objective.size(); ++j)
				std::cout << objective[j] << '\t';
			std::cout << 'o' << '\t' << i->gr_ << '\t' << i->gcd_ << '\t' << i->gcpd_ << std::endl;
		}
		std::cout << std::endl;
#endif
	}
	return end;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename GrEA<_TReal, _TDecision, _TRandom>::TIndividual *GrEA<_TReal, _TDecision, _TRandom>::_Compete(const std::vector<const TIndividual *> &competition)
{
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
