#pragma once

#include <cassert>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <vector>

namespace otl
{
namespace optimizer
{
namespace spea2
{
/*!
 * \brief Calculate the strength value of individual
 * \param[in] individual The individual to be evaluated
 * \param[in] begin The begin iterator of the population
 * \param[in] end The end iterator of the population
 * \param[in] dominate The dominance function
 * \return The number of solutions individual dominates
 */
template <typename _TIterator, typename _TDominate>
size_t StrengthFitness(_TIterator individual, _TIterator begin, _TIterator end, _TDominate dominate)
{
	size_t count = 0;
	for (_TIterator i = begin; i != end; ++i)
	{
		if (i != individual)
		{
			if (dominate(*individual, *i))
				++count;
		}
	}
	return count;
}

/*!
 * \brief The raw fitness of individual
 * \param[in] individual The individual to be evaluated
 * \param[in] begin The begin iterator of the population
 * \param[in] end The end iterator of the population
 * \param[in] dominate The dominance function
 * \return The sum of the strength value of individual's dominators
 */
template <typename _TIterator, typename _TDominate>
size_t RawFitness(_TIterator individual, _TIterator begin, _TIterator end, _TDominate dominate)
{
	size_t rawFitness = 0;
	for (_TIterator dominator = begin; dominator != end; ++dominator)
	{
		if (dominator != individual)
		{
			if (dominate(*dominator, *individual))
				rawFitness += StrengthFitness(dominator, begin, end, dominate);
		}
	}
	return rawFitness;
}
}
}
}
