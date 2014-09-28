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
  author = {İbrahim Karahan and Murat Köksalan},
  title = {A Territory Defining Multiobjective Evolutionary Algorithms and Preference
	Incorporation},
  journal = {IEEE Transactions on Evolutionary Computation},
  year = {2010},
  volume = {14},
  pages = {636-664},
  number = {4},
  doi = {10.1109/TEVC.2009.2033586}
}
*/

#pragma once

#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include <random>
#include <OTL/Optimizer/Metaheuristic.h>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Utility/Nondominate.h>
#include <OTL/Utility/Relation/Pareto.h>
#include <OTL/Utility/WithSpaceBoundary.h>
#include "Individual.h"

namespace otl
{
namespace optimizer
{
namespace tdea
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class TDEA : public Metaheuristic<std::list<Individual<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef Individual<TReal, TDecision> TIndividual;
	typedef std::list<TIndividual> TSolutionSet;
	typedef std::vector<TIndividual> TPopulation;
	typedef Metaheuristic<TSolutionSet> TSuper;
	typedef typename TSuper::TProblem TProblem;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TMinMax TMinMax;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TBoundary TBoundary;

	TDEA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, const TBoundary &objectiveBoundary, const TReal territorySize);
	~TDEA(void);
	const TBoundary &GetObjectiveBoundary(void) const;
	TReal GetTerritorySize(void) const;
	static bool Dominate(const TIndividual &individual1, const TIndividual &individual2);

protected:
	TPopulation population_;

	static bool _Dominate(const TIndividual *individual1, const TIndividual *individual2);
	void _Scaling(TIndividual &individual) const;
	void _Scaling(const std::vector<TReal> &objective, std::vector<TReal> &scaledObjective) const;
	const TIndividual &_ArchiveSelection(const TSolutionSet &archive);
	template <typename _TIterator> _TIterator _RandLocate(_TIterator begin, _TIterator end);
	const TIndividual &_PopulationSelection(const TPopulation &population);
	void _UpdateArchive(const TIndividual &elite, TSolutionSet &archive) const;
	static TReal _GetDist(const TIndividual &elite, const TIndividual &individual);
	void _UpdatePopulation(const TIndividual &elite, TPopulation &population);

private:
	const TBoundary &objectiveBoundary_;
	TReal territorySize_;
	std::uniform_real_distribution<TReal> dist_;
};

template <typename _TReal, typename _TDecision, typename _TRandom>
TDEA<_TReal, _TDecision, _TRandom>::TDEA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, const TBoundary &objectiveBoundary, const TReal territorySize)
	: TSuper(problem)
	, otl::utility::WithRandom<TRandom>(random)
	, objectiveBoundary_(objectiveBoundary)
	, territorySize_(territorySize)
	, dist_(0, 1)
{
	assert(TSuper::GetProblem().GetNumberOfObjectives() == objectiveBoundary_.size());
	population_.resize(initial.size());
	for (size_t i = 0; i < initial.size(); ++i)
	{
		population_[i].decision_ = initial[i];
		TSuper::GetProblem()(population_[i]);
		_Scaling(population_[i]);
	}
	typedef typename TPopulation::pointer _TPointer;
	std::list<_TPointer> population;
	for (size_t i = 0; i < population_.size(); ++i)
		population.push_back(&population_[i]);
	std::list<_TPointer> archive = otl::utility::ExtractNondominate(population, &_Dominate);
	for (typename std::list<_TPointer>::iterator i = archive.begin(); i != archive.end(); ++i)
		TSuper::solutionSet_.push_back(**i);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
TDEA<_TReal, _TDecision, _TRandom>::~TDEA(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename TDEA<_TReal, _TDecision, _TRandom>::TBoundary &TDEA<_TReal, _TDecision, _TRandom>::GetObjectiveBoundary(void) const
{
	return objectiveBoundary_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal TDEA<_TReal, _TDecision, _TRandom>::GetTerritorySize(void) const
{
	return territorySize_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool TDEA<_TReal, _TDecision, _TRandom>::Dominate(const TIndividual &individual1, const TIndividual &individual2)
{
	return otl::utility::relation::Dominate(individual1.objective_, individual2.objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool TDEA<_TReal, _TDecision, _TRandom>::_Dominate(const TIndividual *individual1, const TIndividual *individual2)
{
	return Dominate(*individual1, *individual2);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void TDEA<_TReal, _TDecision, _TRandom>::_Scaling(TIndividual &individual) const
{
	_Scaling(individual.objective_, individual.scaledObjective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void TDEA<_TReal, _TDecision, _TRandom>::_Scaling(const std::vector<TReal> &objective, std::vector<TReal> &scaledObjective) const
{
	const TReal factor = 0.137315;
	scaledObjective.resize(objective.size());
	for (size_t i = 0; i < objective.size(); ++i)
	{
		const auto &minMax = objectiveBoundary_[i];
		assert(minMax.first <= minMax.second);
		scaledObjective[i] = objective[i] - minMax.first;
		const TReal range = minMax.second - minMax.first;
		if (scaledObjective[i] <= range)
			scaledObjective[i] = (scaledObjective[i] / range) * (1.0 / (1 + exp(-1 * range / factor)) - 0.5) * 2.0;
		else
			scaledObjective[i] = (1.0 / (1 + exp(-1 * scaledObjective[i] / factor)) - 0.5) * 2.0;
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename TDEA<_TReal, _TDecision, _TRandom>::TIndividual &TDEA<_TReal, _TDecision, _TRandom>::_ArchiveSelection(const TSolutionSet &archive)
{
	return *_RandLocate(archive.begin(), archive.end());
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TIterator> _TIterator TDEA<_TReal, _TDecision, _TRandom>::_RandLocate(_TIterator begin, _TIterator end)
{
	std::uniform_int_distribution<size_t> dist(0, std::distance(begin, end) - 1);
	_TIterator elite = begin;
	for (size_t count = dist(this->GetRandom()); count; --count)
		++elite;
	return elite;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename TDEA<_TReal, _TDecision, _TRandom>::TIndividual &TDEA<_TReal, _TDecision, _TRandom>::_PopulationSelection(const TPopulation &population)
{
	std::vector<size_t> indexList(population.size());
	for (size_t i = 0; i < indexList.size(); ++i)
		indexList[i] = i;
	std::random_shuffle(indexList.begin(), indexList.end(), [this](const size_t n)-> size_t{std::uniform_int_distribution<size_t> dist(0, n - 1);return dist(this->GetRandom());});
	const TIndividual &individual1 = population[indexList[0]];
	const TIndividual &individual2 = population[indexList[1]];
	if (Dominate(individual1, individual2))
		return individual1;
	else if (Dominate(individual2, individual1))
		return individual2;
	if (dist_(this->GetRandom()) < 0.5)
		return individual1;
	else
		return individual2;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void TDEA<_TReal, _TDecision, _TRandom>::_UpdateArchive(const TIndividual &elite, TSolutionSet &archive) const
{
	assert(territorySize_ > 0);
	for (auto i = archive.begin(); i != archive.end();)
	{
		if (Dominate(elite, *i))
			i = archive.erase(i);
		else if (Dominate(*i, elite))
			return;
		else
			++i;
	}
	if (archive.empty())
		archive.push_front(elite);
	else
	{
		TIndividual *nearestIndividual = &archive.front();
		TReal minDist = _GetDist(elite, *nearestIndividual);
		for (auto i = ++archive.begin(); i != archive.end(); ++i)
		{
			const TReal dist = _GetDist(elite, *i);
			if (dist < minDist)
			{
				nearestIndividual = &*i;
				minDist = dist;
			}
		}
		TReal maxDelta = std::fabs(nearestIndividual->scaledObjective_[0] - elite.scaledObjective_[0]);
		for (size_t i = 1; i < elite.scaledObjective_.size(); ++i)
		{
			const TReal delta = std::fabs(nearestIndividual->scaledObjective_[i] - elite.scaledObjective_[i]);
			if (delta > maxDelta)
				maxDelta = delta;
		}
		if (maxDelta > territorySize_)
			archive.push_back(elite);
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
typename TDEA<_TReal, _TDecision, _TRandom>::TReal TDEA<_TReal, _TDecision, _TRandom>::_GetDist(const TIndividual &elite, const TIndividual &individual)
{
	assert(!elite.scaledObjective_.empty());
	assert(elite.scaledObjective_.size() == individual.scaledObjective_.size());
	TReal dist = 0;
	for (size_t i = 0; i < elite.scaledObjective_.size(); ++i)
		dist += std::fabs(elite.scaledObjective_[i] - individual.scaledObjective_[i]);
	return dist;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void TDEA<_TReal, _TDecision, _TRandom>::_UpdatePopulation(const TIndividual &elite, TPopulation &population)
{
	for (size_t i = 0; i < population.size(); ++i)
	{
		if (Dominate(elite, population[i]))
		{
			population[i] = elite;
			return;
		}
		else if (Dominate(population[i], elite))
			return;
	}
	std::uniform_int_distribution<size_t> dist(0, population.size() - 1);
	population[dist(this->GetRandom())] = elite;
}
}
}
}
