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

#include <vector>
#include <cassert>
#include <numeric>
#include <boost/numeric/ublas/symmetric.hpp>
#include <OTL/Problem/Problem.h>

namespace otl
{
namespace problem
{
namespace tsp
{
bool CheckPathTSP(std::vector<size_t> path);

template <typename _TReal, typename _TIterator, typename _TDistance>
boost::numeric::ublas::symmetric_matrix<_TReal> CalculateAdjacencyMatrix(_TIterator begin, _TIterator end, _TDistance distance)
{
	const size_t cities = std::distance(begin, end);
	boost::numeric::ublas::symmetric_matrix<_TReal> adjacencyMatrix(cities, cities);
	for (size_t i = 0; i < adjacencyMatrix.size1(); ++i)
		adjacencyMatrix(i, i) = 0;
	size_t aIndex = 0;
	for (_TIterator aIter = begin; aIter != end; ++aIter, ++aIndex)
	{
		size_t bIndex = 0;
		for (_TIterator bIter = begin; bIter != aIter; ++bIter, ++bIndex)
			adjacencyMatrix(aIndex, bIndex) = distance(*aIter, *bIter);
	}
	return adjacencyMatrix;
}

template <typename _TReal, typename _TIterator>
boost::numeric::ublas::symmetric_matrix<_TReal> CalculateAdjacencyMatrix(_TIterator begin, _TIterator end)
{
	const size_t count = std::distance(begin, end);
	boost::numeric::ublas::symmetric_matrix<_TReal> adjacencyMatrix(count, count);
	for (size_t i = 0; i < count; ++i)
		adjacencyMatrix(i, i) = 0;
	size_t i = 0;
	for (_TIterator point1 = begin; point1 != end; ++point1)
	{
		size_t j = i + 1;
		for (_TIterator point2 = ++_TIterator(point1); point2 != end; ++point2)
		{
			const std::vector<_TReal> &_point1 = (*point1);
			const std::vector<_TReal> &_point2 = (*point2);
			adjacencyMatrix(i, j) = sqrt(std::inner_product(_point1.begin(), _point1.end(), _point2.begin(), (_TReal)0, std::plus<_TReal>()
				, [](_TReal x, _TReal y)->_TReal{_TReal t = x - y;return t * t;}));
			++j;
		}
		++i;
	}
	return adjacencyMatrix;
}

template <typename _TReal>
_TReal CalculatePathDistance(const std::vector<size_t> &path, const boost::numeric::ublas::symmetric_matrix<_TReal> &adjacencyMatrix)
{
	assert(CheckPathTSP(path));
	assert(adjacencyMatrix.size1() > 1 && adjacencyMatrix.size1() == adjacencyMatrix.size2());
	const size_t cities = adjacencyMatrix.size1();
	assert(path.size() == cities);
	_TReal pathDistance = 0;
	assert(0 <= path.front() && path.front() < (size_t)cities);
	for (size_t travel = 1; travel < path.size(); ++travel)
	{
		assert(0 <= path[travel] && path[travel] < (size_t)cities);
		pathDistance += adjacencyMatrix(path[travel - 1], path[travel]);
	}
	assert(0 <= path.back() && path.back() < (size_t)cities);
	return pathDistance + adjacencyMatrix(path.back(), path.front());
}

template <typename _TReal>
class TSP : public Problem<_TReal, std::vector<size_t> >
{
public:
	typedef _TReal TReal;
	typedef std::vector<size_t> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef boost::numeric::ublas::symmetric_matrix<TReal> TAdjacencyMatrix;
	typedef std::vector<TAdjacencyMatrix> TAdjacencyMatrics;

	TSP(const TAdjacencyMatrix &adjacencyMatrix);
	~TSP(void);
	const TAdjacencyMatrix &GetAdjacencyMatrix(void) const;
	size_t GetNumberOfCities(void) const;

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);

private:
	const TAdjacencyMatrix &adjacencyMatrix_;
};

template <typename _TReal>
TSP<_TReal>::TSP(const TAdjacencyMatrix &adjacencyMatrix)
	: TSuper(1)
	, adjacencyMatrix_(adjacencyMatrix)
{
}

template <typename _TReal>
TSP<_TReal>::~TSP(void)
{
}

template <typename _TReal>
const typename TSP<_TReal>::TAdjacencyMatrix &TSP<_TReal>::GetAdjacencyMatrix(void) const
{
	return adjacencyMatrix_;
}

template <typename _TReal>
size_t TSP<_TReal>::GetNumberOfCities(void) const
{
	assert(adjacencyMatrix_.size1() == adjacencyMatrix_.size2());
	return adjacencyMatrix_.size1();
}

template <typename _TReal>
size_t TSP<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void TSP<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}

template <typename _TReal>
void TSP<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	assert(TSuper::GetNumberOfObjectives() == 1);
	assert(decision.size() == GetNumberOfCities());
	objective.resize(1);
	objective[0] = CalculatePathDistance(decision, adjacencyMatrix_);
}
}
}
}
