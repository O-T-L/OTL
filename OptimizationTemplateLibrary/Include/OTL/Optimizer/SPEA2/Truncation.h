#pragma once

#include <cassert>
#include <list>
#include <boost/numeric/ublas/symmetric.hpp>
#include "Trim.h"
#include "Utility.h"

namespace otl
{
namespace optimizer
{
namespace spea2
{
template <typename _TReal, typename _TIterator, typename _TDistance>
boost::numeric::ublas::symmetric_matrix<_TReal> CalculateAdjacencyMatrix(_TIterator begin, _TIterator end, _TDistance distance)
{
	const size_t size = std::distance(begin, end);
	boost::numeric::ublas::symmetric_matrix<_TReal> adjacencyMatrix(size, size);
	for (size_t i = 0; i < size; ++i)
		adjacencyMatrix(i, i) = 0;
	size_t index1 = 0;
	for (_TIterator individual1 = begin; individual1 != end; ++individual1)
	{
		size_t index2 = index1 + 1;
		for (_TIterator individual2 = ++_TIterator(individual1); individual2 != end; ++individual2)
		{
			adjacencyMatrix(index1, index2) = distance(*individual1, *individual2);
			++index2;
		}
		++index1;
	}
	return adjacencyMatrix;
}

template <typename _TReal, typename _TPointer, typename _TDistance>
void Truncation(size_t remove, std::list<_TPointer> &population, _TDistance distance)
{
	const auto adjacencyMatrix = CalculateAdjacencyMatrix<_TReal>(population.begin(), population.end(), distance);
	Indexing(population.begin(), population.end());
	while (remove)
	{
		Trim<_TReal>(population, adjacencyMatrix);
		--remove;
	}
}
}
}
}
