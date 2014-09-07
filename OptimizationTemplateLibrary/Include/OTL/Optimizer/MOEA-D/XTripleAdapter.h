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

#include <random>
#include <OTL/Crossover/WithXTripleCrossover.h>
#include <OTL/Mutation/WithMutation.h>

namespace otl
{
namespace optimizer
{
namespace moea_d
{
template <typename _TMOEA_D>
class XTripleAdapter : public _TMOEA_D, public otl::crossover::WithXTripleCrossover<typename _TMOEA_D::TReal, typename _TMOEA_D::TDecision>, public otl::mutation::WithMutation<typename _TMOEA_D::TReal, typename _TMOEA_D::TDecision>
{
public:
	typedef _TMOEA_D TMOEA_D;
	typedef TMOEA_D TSuper;
	typedef typename TSuper::TReal TReal;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TRandom TRandom;
	typedef typename TSuper::TIndividual TIndividual;
	typedef typename TSuper::TSolutionSet TSolutionSet;
	typedef typename TSuper::TProblem TProblem;
	typedef typename otl::crossover::WithXTripleCrossover<TReal, TDecision>::TCrossover TCrossover;
	typedef typename otl::mutation::WithMutation<TReal, TDecision>::TMutation TMutation;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TNeighbors TNeighbors;

	XTripleAdapter(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const std::vector<TPoint> &weightVectors, const size_t nNeighbors);
	~XTripleAdapter(void);

protected:
	void _DoStep(void);
};

template <typename _TMOEA_D>
XTripleAdapter<_TMOEA_D>::XTripleAdapter(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, TCrossover &crossover, TMutation &mutation, const std::vector<TPoint> &weightVectors, const size_t nNeighbors)
	: TSuper(random, problem, initial, weightVectors, nNeighbors)
	, otl::crossover::WithXTripleCrossover<TReal, TDecision>(crossover)
	, otl::mutation::WithMutation<TReal, TDecision>(mutation)
{
}

template <typename _TMOEA_D>
XTripleAdapter<_TMOEA_D>::~XTripleAdapter(void)
{
}

template <typename _TMOEA_D>
void XTripleAdapter<_TMOEA_D>::_DoStep(void)
{
    for(size_t i = 0; i < TSuper::GetSolutionSet().size(); ++i)
	{
    	const TNeighbors &neighbors = TSuper::neighbors_[i];
    	assert(neighbors.size() > 0);
    	std::uniform_int_distribution<size_t> dist(0, neighbors.size() - 1);
		const TIndividual &parent1 = TSuper::GetSolutionSet()[neighbors[dist(this->GetRandom())]];
		const TIndividual &parent2 = TSuper::GetSolutionSet()[neighbors[dist(this->GetRandom())]];
		const TIndividual &parent3 = TSuper::GetSolutionSet()[neighbors[dist(this->GetRandom())]];
		TIndividual child;
		this->GetCrossover()(TSuper::GetSolutionSet()[i], parent1, parent2, parent3, child);

		this->GetMutation()(child);
		TSuper::GetProblem()(child);
		TSuper::_UpdateReference(child);
		TSuper::_UpdateNeighbors(child, neighbors);
	}
}
}
}
}
