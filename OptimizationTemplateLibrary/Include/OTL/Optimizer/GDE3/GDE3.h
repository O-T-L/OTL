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
class GDE3 : public Metaheuristic<std::vector<otl::optimizer::nsga_ii::Individual<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>, public otl::crossover::WithXTripleCrossover<_TReal, _TDecision>
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

	GDE3(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover);
	~GDE3(void);
	TIndividual MakeOffspring(const TSolutionSet &population, const size_t index);
	static bool Dominate(const TIndividual &individual1, const TIndividual &individual2);

protected:
	static bool _Dominate(const TIndividual *individual1, const TIndividual *individual2);
	void _DoStep(void);
	template <typename _TPointer, typename _TIterator> static _TIterator _SelectNoncritical(const std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	template <typename _TPointer, typename _TIterator> static _TIterator _SelectCritical(const std::list<_TPointer> &front, _TIterator begin, _TIterator end);

private:
	std::uniform_int_distribution<size_t> dist_;
};

template <typename _TReal, typename _TDecision, typename _TRandom>
GDE3<_TReal, _TDecision, _TRandom>::GDE3(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover)
	: TSuper(problem)
	, otl::utility::WithRandom<TRandom>(random)
	, otl::crossover::WithXTripleCrossover<TReal, TDecision>(crossover)
	, dist_(0, initial.size() - 1)
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
GDE3<_TReal, _TDecision, _TRandom>::~GDE3(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
typename GDE3<_TReal, _TDecision, _TRandom>::TIndividual GDE3<_TReal, _TDecision, _TRandom>::MakeOffspring(const TSolutionSet &population, const size_t index)
{
	size_t p1, p2, p3;
	do
	{
		p1 = dist_(this->GetRandom());
	} while (p1 == index);
	do
	{
		p2 = dist_(this->GetRandom());
	} while (p2 == index || p2 == p1);
	do
	{
		p3 = dist_(this->GetRandom());
	} while (p3 == index || p3 == p1 || p3 == p2);
	TIndividual child;
	this->GetCrossover()(population[index], population[p1], population[p2], population[p3], child);
	TSuper::GetProblem()(child);
	return child;
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
	TSolutionSet population = TSuper::solutionSet_;
	std::list<TIndividual> offspring;
	typedef typename TSolutionSet::pointer _TPointer;
	std::list<_TPointer> mix;
	for (size_t i = 0; i < population.size(); ++i)
	{
		TIndividual &individual = population[i];
		TIndividual child = MakeOffspring(population, i);
		if (Dominate(individual, child))
			mix.push_back(&individual);
		else if(Dominate(child, individual))
		{
			offspring.push_back(child);
			mix.push_back(&offspring.back());
		}
		else
		{
			mix.push_back(&individual);
			offspring.push_back(child);
			mix.push_back(&offspring.back());
		}
	}
	typedef typename TSolutionSet::iterator _TIterator;
	otl::selection::NondominateSelection(mix, TSuper::solutionSet_.begin(), TSuper::solutionSet_.end(), &_Dominate, &_SelectNoncritical<_TPointer, _TIterator>, &_SelectCritical<_TPointer, _TIterator>);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> _TIterator GDE3<_TReal, _TDecision, _TRandom>::_SelectNoncritical(const std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	_TIterator dest = begin;
	for (auto i = front.begin(); i != front.end(); ++i)
		*dest = **i;
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
}
}
}
