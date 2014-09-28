#pragma once

#include <cassert>
#include <list>
#include <boost/numeric/ublas/matrix.hpp>
#include <OTL/Optimizer/SPEA2/Utility.h>
#include "Trim.h"

namespace otl
{
namespace optimizer
{
namespace spea2_sde
{
template <typename _TReal, typename _TIterator, typename _TDistance>
boost::numeric::ublas::matrix<_TReal> CalculateAdjacencyMatrix(_TIterator begin, _TIterator end, _TDistance distance)
{
	const size_t size = std::distance(begin, end);
	boost::numeric::ublas::matrix<_TReal> adjacencyMatrix(size, size);
	for (size_t i = 0; i < size; ++i)
		adjacencyMatrix(i, i) = 0;
	size_t index1 = 0;
	for (_TIterator individual1 = begin; individual1 != end; ++individual1)
	{
		size_t index2 = 0;
		for (_TIterator individual2 = begin; individual2 != end; ++individual2)
		{
			if (individual2 != individual1)
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
	const auto adjacencyMatrix = otl::optimizer::spea2_sde::CalculateAdjacencyMatrix<_TReal>(population.begin(), population.end(), distance);
	otl::optimizer::spea2::Indexing(population.begin(), population.end());
	while (remove)
	{
		otl::optimizer::spea2_sde::Trim<_TReal>(population, adjacencyMatrix);
		--remove;
	}
}
}
}
}
