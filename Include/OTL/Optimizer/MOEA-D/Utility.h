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
#include <boost/numeric/ublas/symmetric.hpp>

namespace otl
{
namespace optimizer
{
namespace moea_d
{
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
std::vector<std::vector<size_t> > InitNeighbors(const boost::numeric::ublas::symmetric_matrix<_TReal> &adjacencyMatrix, const size_t nNeighbors)
{
	assert(adjacencyMatrix.size1() == adjacencyMatrix.size2());
	std::vector<std::vector<size_t> > neighbors(adjacencyMatrix.size1());
	for (size_t i = 0; i < neighbors.size(); ++i)
	{
		boost::numeric::ublas::matrix_row<const boost::numeric::ublas::symmetric_matrix<_TReal> > adjacencyDistances(adjacencyMatrix, i);
		typedef std::pair<_TReal, size_t> _TAdjacency;
		std::vector<_TAdjacency> adjacency(adjacencyDistances.size());
		std::vector<const _TAdjacency *> _adjacency(adjacencyDistances.size());
		for (size_t j = 0; j < adjacencyDistances.size(); ++j)
		{
			adjacency[j].first = adjacencyDistances(j);
			adjacency[j].second = j;
			_adjacency[j] = &adjacency[j];
		}
		std::partial_sort(_adjacency.begin(), _adjacency.begin() + nNeighbors, _adjacency.end()
			, [](const _TAdjacency *adjacency1, const _TAdjacency *adjacency2)->bool{return adjacency1->first < adjacency2->first;}
		);
		std::vector<size_t> &_neighbors = neighbors[i];
		_neighbors.resize(nNeighbors);
		for (size_t j = 0; j < nNeighbors; ++j)
			_neighbors[j] = _adjacency[j]->second;
		assert(std::find(_neighbors.begin(), _neighbors.end(), i) != _neighbors.end());
	}
	return neighbors;
}

template <typename _TReal>
std::vector<_TReal> ComputeDirection(const std::vector<_TReal> &objective, const std::vector<_TReal> &ideal)
{
	assert(objective.size() == ideal.size());
	std::vector<_TReal> direction(objective.size());
	for (size_t i = 0; i < direction.size(); ++i)
	{
		direction[i] = objective[i] - ideal[i];
		assert(direction[i] >= 0);
	}
	return direction;
}
}
}
}
