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
  author = {Qingfu Zhang and Hui Li},
  title = {{MOEA/D}: A Multiobjective Evolutionary Algorithm Based on Decomposition},
  journal = {IEEE Transactions on Evolutionary Computation},
  year = {2007},
  volume = {11},
  pages = {712-731},
  number = {6},
  doi = {10.1109/TEVC.2007.892759}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <list>
#include <utility>
#include <limits>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <OTL/Optimizer/Metaheuristic.h>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Utility/Nondominate.h>
#include <OTL/Utility/Relation/Pareto.h>
#include <OTL/Selection/NondominateSelection.h>
#include "Individual.h"
#include "Utility.h"

namespace otl
{
namespace optimizer
{
namespace moea_d
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class MOEA_D : public Metaheuristic<std::vector<Individual<_TReal, _TDecision> > >, public otl::utility::WithRandom<_TRandom>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef Individual<TReal, TDecision> TIndividual;
	typedef std::vector<TIndividual> TSolutionSet;
	typedef Metaheuristic<TSolutionSet> TSuper;
	typedef typename TSuper::TProblem TProblem;
	typedef std::vector<TReal> TPoint;
	typedef std::vector<size_t> TNeighbors;

	MOEA_D(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, const std::vector<TPoint> &weightVectors, const size_t nNeighbors);
	virtual ~MOEA_D(void);
	const std::vector<TPoint> &GetWeightVectors(void) const;
	const std::vector<TReal> &GetReferencePoint(void) const;
	const std::vector<TNeighbors> &GetNeighbors(void) const;
	TReal Aggregation(const TPoint &objective, const TPoint &weight);

protected:
	std::vector<TReal> referencePoint_;
	std::vector<TNeighbors> neighbors_;

	void _UpdateReference(const TIndividual &individual);
	void _UpdateNeighbors(const TIndividual &individual, const TNeighbors &neighbors);
	virtual TReal _DoAggregation(const TPoint &objective, const TPoint &weight) = 0;

private:
	std::vector<TPoint> weightVectors_;
};

template <typename _TReal, typename _TDecision, typename _TRandom>
MOEA_D<_TReal, _TDecision, _TRandom>::MOEA_D(TRandom random, TProblem &problem, const std::vector<TDecision> &initial, const std::vector<TPoint> &weightVectors, const size_t nNeighbors)
	: TSuper(problem)
	, otl::utility::WithRandom<TRandom>(random)
	, weightVectors_(weightVectors)
{
	assert(weightVectors.size() == initial.size());
	TSuper::solutionSet_.resize(initial.size());
	for (size_t i = 0; i < initial.size(); ++i)
	{
		TIndividual &individual = TSuper::solutionSet_[i];
		individual.decision_ = initial[i];
		TSuper::GetProblem()(individual);
	}

	referencePoint_ = TSuper::solutionSet_[0].objective_;
	for (size_t i = 1; i < TSuper::solutionSet_.size(); ++i)
		_UpdateReference(TSuper::solutionSet_[i]);
	neighbors_ = InitNeighbors(CalculateAdjacencyMatrix<TReal>(weightVectors.begin(), weightVectors.end()), nNeighbors);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
MOEA_D<_TReal, _TDecision, _TRandom>::~MOEA_D(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const std::vector<typename MOEA_D<_TReal, _TDecision, _TRandom>::TPoint> &MOEA_D<_TReal, _TDecision, _TRandom>::GetWeightVectors(void) const
{
	return weightVectors_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const std::vector<_TReal> &MOEA_D<_TReal, _TDecision, _TRandom>::GetReferencePoint(void) const
{
	return referencePoint_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
const std::vector<typename MOEA_D<_TReal, _TDecision, _TRandom>::TNeighbors> &MOEA_D<_TReal, _TDecision, _TRandom>::GetNeighbors(void) const
{
	return neighbors_;
}

template <typename _TReal, typename _TDecision, typename _TRandom>
_TReal MOEA_D<_TReal, _TDecision, _TRandom>::Aggregation(const TPoint &objective, const TPoint &weight)
{
	return _DoAggregation(objective, weight);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void MOEA_D<_TReal, _TDecision, _TRandom>::_UpdateReference(const TIndividual &individual)
{
	assert(referencePoint_.size() == individual.objective_.size());
	for (size_t i = 0; i < referencePoint_.size(); ++i)
	{
		if (individual.objective_[i] < referencePoint_[i])
			referencePoint_[i] = individual.objective_[i];
	}
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void MOEA_D<_TReal, _TDecision, _TRandom>::_UpdateNeighbors(const TIndividual &individual, const TNeighbors &neighbors)
{
	for (size_t i = 0; i < neighbors.size(); ++i)
	{
		const size_t index = neighbors[i];
		const TPoint &weight = weightVectors_[index];
		TIndividual &_individual = TSuper::solutionSet_[index];
		if (Aggregation(individual.objective_, weight) < Aggregation(_individual.objective_, weight))
			_individual = individual;
	}
}
}
}
}
