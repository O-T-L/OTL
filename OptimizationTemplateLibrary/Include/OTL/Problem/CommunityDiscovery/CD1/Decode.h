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
#include <list>
#include <set>
#include <algorithm>
#include <random>
#include <boost/numeric/ublas/symmetric.hpp>

namespace otl
{
namespace problem
{
namespace community_discovery
{
namespace cd1
{
template <typename _TReal>
std::vector<std::vector<size_t> > MakeOrderedNeighborList(const boost::numeric::ublas::symmetric_matrix<_TReal> &graph)
{
	assert(graph.size1() == graph.size2());
	std::vector<std::vector<size_t> > list(graph.size1());
	for (size_t i = 0; i < list.size(); ++i)
	{
		std::vector<size_t> &neighbors = list[i];
		for (size_t j = 0; j < list.size(); ++j)
		{
			if (i != j && graph(i, j) > 0)
				neighbors.push_back(j);
		}
		std::sort(neighbors.begin(), neighbors.end());
	}
	return list;
}

template <typename _TRandom>
void Fix(const std::vector<std::vector<size_t> > &list, std::vector<size_t> &decision, _TRandom &random)
{
	for (size_t i = 0; i < decision.size(); ++i)
	{
		assert(0 <= decision[i] && decision[i] < decision.size());
		if (decision[i] > list[i].size())
		{
			std::uniform_int_distribution<size_t> dist(0, list[i].size() - 1);
			decision[i] = dist(random);
		}
	}
}

bool IsLegal(const std::vector<std::vector<size_t> > &list, const std::vector<size_t> &decision);
std::list<std::set<size_t> > Decode(const std::vector<std::vector<size_t> > &list, const std::vector<size_t> &decision);
}
}
}
}
