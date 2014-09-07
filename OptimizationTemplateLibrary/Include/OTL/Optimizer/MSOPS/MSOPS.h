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
#include <numeric>
#include <OTL/Optimizer/Metaheuristic.h>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Crossover/WithCrossover.h>
#include <OTL/Mutation/WithMutation.h>
#include <OTL/Utility/Relation/Pareto.h>
#include <OTL/Optimizer/NSGA-II/Offspring.h>
#include "Individual.h"
#include "Fitness.h"

namespace otl
{
namespace optimizer
{
namespace msops
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class MSOPS : public Metaheuristic<std::vector<Individual<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>, public otl::crossover::WithCrossover<_TReal, _TDecision>, public otl::mutation::WithMutation<_TReal, _TDecision>
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
	typedef std::vector<TPoint> TTargets;

	MSOPS(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const TTargets &targets, const TReal factor);
	~MSOPS(void);
	const TTargets &GetTargets(void) const;
	TReal GetFactor(void) const;
	TSolutionSet MakeOffspring(const TSolutionSet &ancestor);
	template <typename _TIterator> void FitnessAssignment(_TIterator begin, _TIterator end);
	template <typename _TPointer, typename _TIterator> void EnvironmentalSelection(std::list<_TPointer> &front, _TIterator begin, _TIterator end);
	static bool Dominate(const TIndividual &individual1, const TIndividual &individual2);

protected:
	void _DoStep(void);
	static const TIndividual *_Compete(const std::vector<const TIndividual *> &competition);

private:
	TTargets targets_;
	TReal factor_;
};

template <typename _TReal, typename _TDecision, typename _TRandom>
MSOPS<_TReal, _TDecision, _TRandom>::MSOPS(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const TTargets &targets, const TReal factor)
	: TSuper(problem)
	, otl::utility::WithRandom<TRandom>(random)
	, otl::crossover::WithCrossover<TReal, TDecision>(crossover)
	, otl::mutation::WithMutation<TReal, TDecision>(mutation)
	, targets_(targets)
	, factor_(factor)
{
	TSuper::solutionSet_.resize(initial.size());
	for (size_t i = 0; i < initial.size(); ++i)
	{
		TIndividual &individual = TSuper::solutionSet_[i];
		individual.decision_ = initial[i];
		TSuper::GetProblem()(individual);
		individual.fitness_.resize(targets_.size() * 2);
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
MSOPS<_TReal, _TDecision, _TRandom>::~MSOPS(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename MSOPS<_TReal, _TDecision, _TRandom>::TTargets &MSOPS<_TReal, _TDecision, _TRandom>::GetTargets(void) const
{
	return targets_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal MSOPS<_TReal, _TDecision, _TRandom>::GetFactor(void) const
{
	return factor_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
typename MSOPS<_TReal, _TDecision, _TRandom>::TSolutionSet MSOPS<_TReal, _TDecision, _TRandom>::MakeOffspring(const TSolutionSet &ancestor)
{
	TSolutionSet offspring = otl::optimizer::nsga_ii::MakeOffspring(ancestor.size(), ancestor.begin(), ancestor.end(), this->GetRandom(), &_Compete, this->GetCrossover());
	for (size_t i = 0; i < offspring.size(); ++i)
	{
		TIndividual &child = offspring[i];
		this->GetMutation()(child);
		TSuper::GetProblem()(child);
		child.fitness_.resize(targets_.size() * 2);
	}
	return offspring;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TIterator> void MSOPS<_TReal, _TDecision, _TRandom>::FitnessAssignment(_TIterator begin, _TIterator end)
{
	auto scores = CalculateScores(targets_, begin, end, &WeightedMinMax<TReal>);
	auto _scores = CalculateScores(targets_, begin, end, [this](const std::vector<TReal> &objective, const std::vector<TReal> &target)->TReal{return VectorAngleDistanceScaling(objective, target, this->factor_);});
	scores.splice(scores.end(), _scores, _scores.begin(), _scores.end());
	AssignRanks(scores);
	SortFitness(begin, end);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
template <typename _TPointer, typename _TIterator> void MSOPS<_TReal, _TDecision, _TRandom>::EnvironmentalSelection(std::list<_TPointer> &front, _TIterator begin, _TIterator end)
{
	std::vector<_TPointer> _front(front.begin(), front.end());
	FitnessAssignment(_front.begin(), _front.end());
	std::partial_sort(_front.begin(), _front.begin() + std::distance(begin, end), _front.end(), [](_TPointer individual1, _TPointer individual2)->bool{return individual1->fitness_ < individual2->fitness_;});
	for (_TIterator i = begin; i != end; ++i)
		*i = *_front[i - begin];
}

template <typename _TReal, typename _TDecision, typename _TRandom>
bool MSOPS<_TReal, _TDecision, _TRandom>::Dominate(const TIndividual &individual1, const TIndividual &individual2)
{
	return otl::utility::relation::Dominate(individual1.objective_, individual2.objective_);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void MSOPS<_TReal, _TDecision, _TRandom>::_DoStep(void)
{
	TSolutionSet ancestor = TSuper::solutionSet_;
	TSolutionSet offspring = MakeOffspring(ancestor);
	typedef typename TSolutionSet::pointer _TPointer;
	std::list<_TPointer> mix;
	for (size_t i = 0; i < ancestor.size(); ++i)
		mix.push_back(&ancestor[i]);
	for (size_t i = 0; i < offspring.size(); ++i)
		mix.push_back(&offspring[i]);
	EnvironmentalSelection(mix, TSuper::solutionSet_.begin(), TSuper::solutionSet_.end());
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename MSOPS<_TReal, _TDecision, _TRandom>::TIndividual *MSOPS<_TReal, _TDecision, _TRandom>::_Compete(const std::vector<const TIndividual *> &competition)
{
	if (Dominate(*competition[0], *competition[1]))
		return competition[0];
	else if (Dominate(*competition[1], *competition[0]))
		return competition[1];
	return competition[0]->fitness_ < competition[1]->fitness_ ? competition[0] : competition[1];
}
}
}
}
