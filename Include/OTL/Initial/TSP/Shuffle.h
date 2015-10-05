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

#include <algorithm>
#include <random>

namespace otl
{
namespace initial
{
namespace tsp
{
template <typename _TRandom>
std::vector<size_t> Shuffle(_TRandom &random, const size_t nCities)
{
	std::vector<size_t> decision(nCities);
	for (size_t i = 0; i < decision.size(); ++i)
		decision[i] = i;
	std::random_shuffle(decision.begin(), decision.end(), [&random](const size_t n)-> size_t{return std::uniform_int_distribution<size_t> (0, n - 1)(random);});
	return decision;
}

template <typename _TRandom>
std::vector<std::vector<size_t> > BatchShuffle(_TRandom &random, const size_t nCities, const size_t size)
{
	std::vector<std::vector<size_t> > initial(size);
	for (size_t i = 0; i < initial.size(); ++i)
		initial[i] = Shuffle(random, nCities);
	return initial;
}
}
}
}
