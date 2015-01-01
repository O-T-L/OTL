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
  author = {Kalyanmoy Deb and Manikanth Mohan and Shikhar Mishra},
  title = {Evaluating the ε-Domination Based Multi-Objective Evolutionary Algorithm
	for a Quick Computation of Pareto-Optimal Solutions},
  journal = {Evolutionary Computation},
  year = {2005},
  volume = {13},
  pages = {501-525},
  number = {4},
  doi = {10.1162/106365605774666895}
}
*/

#pragma once

#include <vector>
#include <list>
#include <algorithm>
#include <random>
#include <OTL/Optimizer/Metaheuristic.h>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Utility/Nondominate.h>
#include <OTL/Utility/Relation/Pareto.h>
#include "Individual.h"

namespace otl
{
namespace optimizer
{
namespace epsilon_moea
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class Epsilon_MOEA : public Metaheuristic<std::list<Individual<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>
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

	Epsilon_MOEA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, const std::vector<TReal> &objectiveLower, const std::vector<TReal> &epsilon);
	~Epsilon_MOEA(void);
	const std::vector<TReal> &GetEpsilon(void) const;
	void Update(TIndividual &individual) const;
	void Update(const std::vector<TReal> &objective, std::vector<size_t> &epslionPoint) const;
	static bool Dominate(const TIndividual &individual1, const TIndividual &individual2);
	static bool EpslionDominate(const TIndividual &individual1, const TIndividual &individual2);

protected:
	TPopulation population_;

	static bool _Dominate(const TIndividual *individual1, const TIndividual *individual2);
	const TIndividual &_ArchiveSelection(const TSolutionSet &archive);
	template <typename _TIterator> _TIterator _RandLocate(_TIterator begin, _TIterator end);
	const TIndividual &_PopulationSelection(const TPopulation &population);
	void _ArchiveAcceptance(const TIndividual &elite, TSolutionSet &archive) const;
	bool _SameGridCompare(const TIndividual &individual1, const TIndividual &individual2) const;
	void _PopulationAcceptance(const TIndividual &elite, TPopulation &population);

private:
	std::vector<TReal> objectiveLower_;
	std::vector<TReal> epsilon_;
	std::uniform_real_distribution<TReal> dist_;
};

template <typename _TReal, typename _TDecision, typename _TRandom>
Epsilon_MOEA<_TReal, _TDecision, _TRandom>::Epsilon_MOEA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, const std::vector<TReal> &objectiveLower, const std::vector<TReal> &epsilon)
	: TSuper(problem)
	, otl::utility::WithRandom<TRandom>(random)
	, objectiveLower_(objectiveLower)
	, epsilon_(epsilon)
	, dist_(0, 1)
{
	assert(TSuper::GetProblem().GetNumberOfObjectives() == objectiveLower_.size());
	assert(objectiveLower_.size() == epsilon_.size());
	population_.resize(initial.size());
	for (size_t i = 0; i < initial.size(); ++i)
	{
		population_[i].decision_ = initial[i];
		TSuper::GetProblem()(population_[i]);
		Update(population_[i]);
	}
	typedef typename TPopulation::pointer _TPointer;
	std::list<_TPointer> population;
	for (size_t i = 0; i < population_.size(); ++i)
		population.push_back(&population_[i]);
	std::list<_TPointer> nondominate = otl::utility::ExtractNondominate(population, &_Dominate);
	for (typename std::list<_TPointer>::iterator i = nondominate.begin(); i != nondominate.end(); ++i)
		TSuper::solutionSet_.push_back(**i);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
Epsilon_MOEA<_TReal, _TDecision, _TRandom>::~Epsilon_MOEA(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const std::vector<typename Epsilon_MOEA<_TReal, _TDecision, _TRandom>::TReal> &Epsilon_MOEA<_TReal, _TDecision, _TRandom>::GetEpsilon(void) const
{
	return epsilon_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void Epsilon_MOEA<_TReal, _TDecision, _TRandom>::Update(TIndividual &individual) const
{
	Update(individual.objective_, individual.point_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void Epsilon_MOEA<_TReal, _TDecision, _TRandom>::Update(const std::vector<TReal> &objective, std::vector<size_t> &epslionPoint) const
{
	assert(objective.size() == epsilon_.size());
	assert(objectiveLower_.size() == epsilon_.size());
	epslionPoint.resize(objective.size());
	for (size_t i = 0; i < objective.size(); ++i)
	{
		assert(objectiveLower_[i] <= objective[i]);
		epslionPoint[i] = (size_t)floor((objective[i] - objectiveLower_[i]) / epsilon_[i]);
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool Epsilon_MOEA<_TReal, _TDecision, _TRandom>::Dominate(const TIndividual &individual1, const TIndividual &individual2)
{
	return otl::utility::relation::Dominate(individual1.objective_, individual2.objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool Epsilon_MOEA<_TReal, _TDecision, _TRandom>::EpslionDominate(const TIndividual &individual1, const TIndividual &individual2)
{
	return otl::utility::relation::Dominate(individual1.point_, individual2.point_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool Epsilon_MOEA<_TReal, _TDecision, _TRandom>::_Dominate(const TIndividual *individual1, const TIndividual *individual2)
{
	return Dominate(*individual1, *individual2);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename Epsilon_MOEA<_TReal, _TDecision, _TRandom>::TIndividual &Epsilon_MOEA<_TReal, _TDecision, _TRandom>::_ArchiveSelection(const TSolutionSet &archive)
{
	return *_RandLocate(archive.begin(), archive.end());
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TIterator> _TIterator Epsilon_MOEA<_TReal, _TDecision, _TRandom>::_RandLocate(_TIterator begin, _TIterator end)
{
	std::uniform_int_distribution<size_t> dist(0, std::distance(begin, end) - 1);
	_TIterator elite = begin;
	for (size_t count = dist(this->GetRandom()); count; --count)
		++elite;
	return elite;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename Epsilon_MOEA<_TReal, _TDecision, _TRandom>::TIndividual &Epsilon_MOEA<_TReal, _TDecision, _TRandom>::_PopulationSelection(const TPopulation &population)
{
	std::vector<size_t> indexList(population.size());
	for (size_t i = 0; i < indexList.size(); ++i)
		indexList[i] = i;
	std::random_shuffle(indexList.begin(), indexList.end(), [this](const size_t n)-> size_t{return std::uniform_int_distribution<size_t> (0, n - 1)(this->GetRandom());});
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
void Epsilon_MOEA<_TReal, _TDecision, _TRandom>::_ArchiveAcceptance(const TIndividual &elite, TSolutionSet &archive) const
{
	for (typename TSolutionSet::iterator i = archive.begin(); i != archive.end();)
	{
		if (EpslionDominate(elite, *i))
			i = archive.erase(i);
		else if (EpslionDominate(*i, elite))
			return;
		else if (elite.point_ == i->point_)
		{
			if (_SameGridCompare(elite, *i))
				*i = elite;
			return;
		}
		else
			++i;
	}
	archive.push_front(elite);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool Epsilon_MOEA<_TReal, _TDecision, _TRandom>::_SameGridCompare(const TIndividual &individual1, const TIndividual &individual2) const
{
	if (Dominate(individual1, individual2))
		return true;
	else if (Dominate(individual2, individual1))
		return false;
	else
	{
		assert(individual1.objective_.size() == individual2.objective_.size());
		assert(individual1.objective_.size() == epsilon_.size());
		assert(objectiveLower_.size() == epsilon_.size());
		assert(individual1.objective_.size() == individual1.point_.size());
		assert(individual2.objective_.size() == individual2.point_.size());
		TReal delta1 = 0;
		TReal delta2 = 0;
		for (size_t i = 0; i < individual1.objective_.size(); ++i)
		{
			TReal temp;
			temp = (individual1.objective_[i] - objectiveLower_[i] - individual1.point_[i]) / epsilon_[i];
			delta1 += temp * temp;
			temp = (individual2.objective_[i] - objectiveLower_[i] - individual2.point_[i]) / epsilon_[i];
			delta2 += temp * temp;
		}
		return delta1 < delta2;
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void Epsilon_MOEA<_TReal, _TDecision, _TRandom>::_PopulationAcceptance(const TIndividual &elite, TPopulation &population)
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
