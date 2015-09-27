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
#include <utility>
#include <random>

namespace otl
{
namespace initial
{
namespace real
{
template <typename _TRandom, typename _TReal>
std::vector<_TReal> Uniform(_TRandom &random, const std::vector<std::pair<_TReal, _TReal> > &boundary)
{
	std::vector<_TReal> initial(boundary.size());
	for (size_t i = 0; i < initial.size(); ++i)
	{
		assert(boundary[i].first < boundary[i].second);
		std::uniform_real_distribution<_TReal> dist(boundary[i].first, boundary[i].second);
		initial[i] = dist(random);
	}
	return initial;
}

template <typename _TRandom, typename _TReal>
std::vector<std::vector<_TReal> > BatchUniform(_TRandom &random, const std::vector<std::pair<_TReal, _TReal> > &boundary, const size_t size)
{
	std::vector<std::vector<_TReal> > initial(size);
	for (size_t i = 0; i < initial.size(); ++i)
		initial[i] = Uniform(random, boundary);
	return initial;
}
}
}
}
