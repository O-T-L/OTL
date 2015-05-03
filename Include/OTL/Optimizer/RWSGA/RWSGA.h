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
#include <OTL/Optimizer/NSGA-II/Offspring.h>
#include <OTL/Selection/RouletteWheelSelection.h>
#include "Individual.h"

namespace otl
{
namespace optimizer
{
namespace rwsga
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class RWSGA : public Metaheuristic<std::vector<Individual<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>, public otl::crossover::WithCrossover<_TReal, _TDecision>, public otl::mutation::WithMutation<_TReal, _TDecision>
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

	RWSGA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation);
	~RWSGA(void);
	TSolutionSet MakeOffspring(const TSolutionSet &ancestor);

protected:
	void _DoStep(void);
	static const TIndividual *_Compete(const std::vector<const TIndividual *> &competition);
};

template <typename _TReal, typename _TDecision, typename _TRandom>
RWSGA<_TReal, _TDecision, _TRandom>::RWSGA(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation)
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
RWSGA<_TReal, _TDecision, _TRandom>::~RWSGA(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
typename RWSGA<_TReal, _TDecision, _TRandom>::TSolutionSet RWSGA<_TReal, _TDecision, _TRandom>::MakeOffspring(const TSolutionSet &ancestor)
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
void RWSGA<_TReal, _TDecision, _TRandom>::_DoStep(void)
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

	const _TReal maxFitness = (**std::max_element(mix.begin(), mix.end(), [](_TPointer individual1, _TPointer individual2)->bool{return individual1->objective_[0] < individual2->objective_[0];})).objective_[0];
	for (auto i = mix.begin(); i != mix.end(); ++i)
		(**i).fitness_ = maxFitness - (**i).objective_[0];
	otl::selection::RouletteWheelSelection<TReal>(this->GetRandom(), mix.begin(), mix.end(), ++TSuper::solutionSet_.begin(), TSuper::solutionSet_.end()
		, [](_TPointer individual)->TReal{return individual->fitness_;}
		, [](_TPointer src, TIndividual &dest){dest = *src;}
	);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const typename RWSGA<_TReal, _TDecision, _TRandom>::TIndividual *RWSGA<_TReal, _TDecision, _TRandom>::_Compete(const std::vector<const TIndividual *> &competition)
{
	return competition[0]->objective_[0] < competition[1]->objective_[0] ? competition[0] : competition[1];
}
}
}
}
