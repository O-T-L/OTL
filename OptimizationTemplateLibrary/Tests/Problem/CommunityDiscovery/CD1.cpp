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

#include <vector>
#include <list>
#include <random>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <OTL/Problem/CommunityDiscovery/CD1/Decode.h>

namespace community_discovery
{
namespace cd1
{
BOOST_AUTO_TEST_CASE(TestDecode)
{
	typedef double _TReal;
	std::vector<std::vector<_TReal> > _graph = {
		{1, 1, 1, 1, 0, 0, 0, 0},
		{1, 1, 1, 1, 0, 0, 0, 0},
		{1, 1, 1, 1, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 1, 1, 1, 1, 0},
		{0, 0, 0, 1, 1, 1, 1, 1},
		{0, 0, 0, 1, 1, 1, 1, 1},
		{0, 0, 0, 1, 0, 1, 1, 1},
	};
	boost::numeric::ublas::symmetric_matrix<_TReal> graph(_graph.size());
	for (size_t i = 0; i < graph.size1(); ++i)
	{
		for (size_t j = 0; j < graph.size2(); ++j)
			graph(i, j) = _graph[i][j];
	}
	auto list = otl::problem::community_discovery::cd1::MakeOrderedNeighborList(graph);
	std::vector<size_t> decision = {1, 0, 0, 4, 6, 7, 4, 3};
	auto communities = otl::problem::community_discovery::cd1::Decode(list, decision);
	BOOST_CHECK_EQUAL(communities.size(), 2);
	BOOST_CHECK_EQUAL(communities.front().size(), 3);
	BOOST_CHECK_EQUAL(communities.back().size(), 5);
}
}
}
