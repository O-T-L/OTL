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
  author = {David Wolfe Corne and Joshua D. Knowles},
  title = {Techniques for Highly Multiobjective Optimisation: Some Nondominated
	Points are Better than Others},
  booktitle = {Genetic and Evolutionary Computation Conference},
  year = {2007},
  pages = {773-780},
  address = {London, England, UK},
  month = {July 7-11},
  doi = {10.1145/1276958.1277115}
}
*/

#pragma once

#include <vector>
#include <cassert>
#include <boost/numeric/ublas/symmetric.hpp>
#include "TSP.h"

namespace otl
{
namespace problem
{
namespace tsp
{
template <typename _TReal>
void CorrelateAdjacencyMatrix(const _TReal correlationParameter, const boost::numeric::ublas::symmetric_matrix<_TReal> &prevAdjacencyMatrix, boost::numeric::ublas::symmetric_matrix<_TReal> &nextAdjacencyMatrix)
{
	assert(-1 <= correlationParameter && correlationParameter <= 1);
	assert(prevAdjacencyMatrix.size1() > 1 && prevAdjacencyMatrix.size1() == prevAdjacencyMatrix.size2());
	assert(prevAdjacencyMatrix.size1() == nextAdjacencyMatrix.size1());
	assert(prevAdjacencyMatrix.size2() == nextAdjacencyMatrix.size2());
	for (size_t i = 0; i < prevAdjacencyMatrix.size1(); ++i)
	{
		for (size_t j = 0; j <= i; ++j)
		{
			assert(prevAdjacencyMatrix(i, j) == prevAdjacencyMatrix(j, i));
			nextAdjacencyMatrix(i, j) = correlationParameter * prevAdjacencyMatrix(i, j) + (1 - correlationParameter) * nextAdjacencyMatrix(i, j);
			assert(nextAdjacencyMatrix(i, j) == nextAdjacencyMatrix(j, i));
		}
	}
}

template <typename _TReal>
void CorrelateAdjacencyMatrics(const std::vector<_TReal> correlationParameter, std::vector<boost::numeric::ublas::symmetric_matrix<_TReal> > &adjacencyMatrics)
{
	assert(correlationParameter.size() == adjacencyMatrics.size() - 1);
	for (size_t i = 0; i < correlationParameter.size(); ++i)
		CorrelateAdjacencyMatrix(correlationParameter[i], adjacencyMatrics[i], adjacencyMatrics[i + 1]);
}

template <typename _TReal>
class MOTSP : public Problem<_TReal, std::vector<size_t> >
{
public:
	typedef _TReal TReal;
	typedef std::vector<size_t> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef boost::numeric::ublas::symmetric_matrix<TReal> TAdjacencyMatrix;
	typedef std::vector<TAdjacencyMatrix> TAdjacencyMatrics;

	MOTSP(const TAdjacencyMatrics &adjacencyMatrics);
	~MOTSP(void);
	const TAdjacencyMatrics &GetAdjacencyMatrics(void) const;
	size_t GetNumberOfCities(void) const;

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective) const;

private:
	const TAdjacencyMatrics &adjacencyMatrics_;
};

template <typename _TReal>
MOTSP<_TReal>::MOTSP(const TAdjacencyMatrics &adjacencyMatrics)
	: TSuper(adjacencyMatrics.size())
	, adjacencyMatrics_(adjacencyMatrics)
{
}

template <typename _TReal>
MOTSP<_TReal>::~MOTSP(void)
{
}

template <typename _TReal>
const typename MOTSP<_TReal>::TAdjacencyMatrics &MOTSP<_TReal>::GetAdjacencyMatrics(void) const
{
	return adjacencyMatrics_;
}

template <typename _TReal>
size_t MOTSP<_TReal>::GetNumberOfCities(void) const
{
	assert(!adjacencyMatrics_.empty());
	assert(adjacencyMatrics_[0].size1() == adjacencyMatrics_[0].size2());
	return adjacencyMatrics_[0].size1();
}

template <typename _TReal>
size_t MOTSP<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void MOTSP<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}

template <typename _TReal>
void MOTSP<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective) const
{
	assert(adjacencyMatrics_.size() == TSuper::GetNumberOfObjectives());
	assert(decision.size() == GetNumberOfCities());
	objective.resize(TSuper::GetNumberOfObjectives());
	for (size_t i = 0; i < objective.size(); ++i)
		objective[i] = CalculatePathDistance(decision, adjacencyMatrics_[i]);
}
}
}
}
