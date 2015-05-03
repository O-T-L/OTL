#pragma once

#include <cassert>
#include <list>
#include <utility>
#include <limits>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/symmetric.hpp>

namespace otl
{
namespace optimizer
{
namespace spea2
{
template <typename _TIterator, typename _TReal>
std::pair<_TIterator, _TIterator> LocateMinEdge(_TIterator begin, _TIterator end, const boost::numeric::ublas::symmetric_matrix<_TReal> &adjacencyMatrix)
{
	_TReal minDistance = std::numeric_limits<_TReal>::max();
	std::pair<_TIterator, _TIterator> minEdge(end, end);
	for (_TIterator individual1 = begin; individual1 != end; ++individual1)
	{
		const size_t index1 = (**individual1).index_;
		for (_TIterator individual2 = ++_TIterator(individual1); individual2 != end; ++individual2)
		{
			const size_t index2 = (**individual2).index_;
			const _TReal distance = adjacencyMatrix(index1, index2);
			if (distance < minDistance)
			{
				minDistance = distance;
				minEdge.first = individual1;
				minEdge.second = individual2;
			}
		}
	}
	return minEdge;
}

template <typename _TReal, typename _TPointer, typename _TIterator>
_TReal NearestDistance(_TPointer individual, _TIterator begin, _TIterator end, const boost::numeric::ublas::symmetric_matrix<_TReal> &adjacencyMatrix)
{
	const size_t index = individual->index_;
	_TReal minDistance = std::numeric_limits<_TReal>::max();
	for (auto _individual = begin; _individual != end; ++_individual)
	{
		const size_t _index = (**_individual).index_;
		const _TReal distance = adjacencyMatrix(index, _index);
		if (distance < minDistance)
			minDistance = distance;
	}
	return minDistance;
}

template <typename _TReal, typename _TPointer>
void Trim(std::list<_TPointer> &population, const boost::numeric::ublas::symmetric_matrix<_TReal> &adjacencyMatrix)
{
	auto minEdge = LocateMinEdge(population.begin(), population.end(), adjacencyMatrix);
	_TPointer individual1 = *minEdge.first;
	_TPointer individual2 = *minEdge.second;
	population.erase(minEdge.first);
	population.erase(minEdge.second);
	const _TReal distance1 = NearestDistance<_TReal>(individual1, population.begin(), population.end(), adjacencyMatrix);
	const _TReal distance2 = NearestDistance<_TReal>(individual2, population.begin(), population.end(), adjacencyMatrix);
	if (distance1 < distance2)
		population.push_back(individual2);
	else
		population.push_back(individual1);
}
}
}
}
