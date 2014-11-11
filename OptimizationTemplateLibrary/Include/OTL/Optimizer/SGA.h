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
#include <OTL/Solution.h>
#include <OTL/Optimizer/Metaheuristic.h>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Crossover/WithCrossover.h>
#include <OTL/Mutation/WithMutation.h>
#include <OTL/Optimizer/NSGA-II/Offspring.h>
#include <OTL/Selection/RouletteWheelSelection.h>

namespace otl
{
namespace optimizer
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class SGA : public Metaheuristic<std::vector<Solution<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>, public otl::crossover::WithCrossover<_TReal, _TDecision>, public otl::mutation::WithMutation<_TReal, _TDecision>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef Solution<TReal, TDecision> TIndividual;
	typedef std::vector<TIndividual> TSolutionSet;
	typedef Metaheuristic<TSolutionSet> TSuper;
	typedef typename TSuper::TProblem TProblem;
	typedef typename otl::crossover::WithCrossover<TReal, TDecision>::TCrossover TCrossover;
	typedef typename otl::mutation::WithMutation<TReal, TDecision>::TMutation TMutation;

	SGA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation);
	~SGA(void);
	size_t GetNumberOfElites(void) const;
	TSolutionSet MakeOffspring(const TSolutionSet &ancestor);

protected:
	void _DoStep(void);
	static const TIndividual *_Compete(const std::vector<const TIndividual *> &competition);
};

template <typename _TReal, typename _TDecision, typename _TRandom>
SGA<_TReal, _TDecision, _TRandom>::SGA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation)
	: TSuper(problem)
	, otl::utility::WithRandom<TRandom>(random)
	, otl::crossover::WithCrossover<TReal, TDecision>(crossover)
	, otl::mutation::WithMutation<TReal, TDecision>(mutation)
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
SGA<_TReal, _TDecision, _TRandom>::~SGA(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
typename SGA<_TReal, _TDecision, _TRandom>::TSolutionSet SGA<_TReal, _TDecision, _TRandom>::MakeOffspring(const TSolutionSet &ancestor)
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
void SGA<_TReal, _TDecision, _TRandom>::_DoStep(void)
{
	TSolutionSet ancestor = TSuper::solutionSet_;
	TSolutionSet offspring = MakeOffspring(ancestor);
	typedef typename TSolutionSet::pointer _TPointer;
	std::list<_TPointer> mix;
	for (size_t i = 0; i < ancestor.size(); ++i)
		mix.push_back(&ancestor[i]);
	for (size_t i = 0; i < offspring.size(); ++i)
		mix.push_back(&offspring[i]);
	auto elite = std::min_element(mix.begin(), mix.end(), [](_TPointer individual1, _TPointer individual2)->bool{return individual1->objective_[0] < individual2->objective_[0];});
	TSuper::solutionSet_[0] = **elite;
	mix.erase(elite);
	std::list<_TPointer> elites = otl::selection::RouletteWheelSelection<TReal>(TSuper::solutionSet_.size() - 1, this->GetRandom(), mix, [](_TPointer individual)->TReal{return individual->objective_[0];});
	assert(elites.size() == TSuper::solutionSet_.size() - 1);
	size_t index = 1;
	for (auto i = elites.begin(); i != elites.end(); ++i, ++index)
		TSuper::solutionSet_[index] = **i;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename SGA<_TReal, _TDecision, _TRandom>::TIndividual *SGA<_TReal, _TDecision, _TRandom>::_Compete(const std::vector<const TIndividual *> &competition)
{
	return competition[0]->objective_[0] < competition[1]->objective_[0] ? competition[0] : competition[1];
}
}
}
