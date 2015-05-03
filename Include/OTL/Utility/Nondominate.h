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
#include <list>

namespace otl
{
namespace utility
{
template <typename _TIterator, typename _TIndividual, typename _TDominate>
bool IdentifyElite(_TIterator individual, std::list<_TIndividual> &population, std::list<_TIndividual> &nondominate, _TDominate dominate)
{
	for (_TIterator elite = nondominate.begin(); elite != nondominate.end();)
	{
		if (dominate(*individual, *elite))
		{
			typename std::list<_TIndividual>::iterator move = elite;
			++elite;
			population.splice(population.begin(), nondominate, move);
		}
		else if (dominate(*elite, *individual))
			return false;
		else
			++elite;
	}
	return true;
}

template <typename _TIndividual, typename _TDominate>
std::list<_TIndividual> ExtractNondominate(std::list<_TIndividual> &population, _TDominate dominate)
{
	typedef typename std::list<_TIndividual>::iterator _TIterator;
	assert(!population.empty());
	std::list<_TIndividual> nondominate;
	nondominate.splice(nondominate.end(), population, population.begin());
	for (_TIterator individual = population.begin(); individual != population.end();)
	{
		assert(!nondominate.empty());
		if (IdentifyElite(individual, population, nondominate, dominate))
		{
			typename std::list<_TIndividual>::iterator move = individual;
			++individual;
			nondominate.splice(nondominate.begin(), population, move);
		}
		else
			++individual;
	}
	return nondominate;
}
}
}
