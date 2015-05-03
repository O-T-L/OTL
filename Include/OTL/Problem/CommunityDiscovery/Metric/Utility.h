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
#include <set>
#include <boost/numeric/ublas/symmetric.hpp>

namespace otl
{
namespace problem
{
namespace community_discovery
{
namespace metric
{
template <typename _TReal>
_TReal InnerDegree(const boost::numeric::ublas::symmetric_matrix<_TReal> &graph, const std::set<size_t> &community, const size_t point)
{
	assert(graph.size1() == graph.size2());
	_TReal degree = 0;
	for (auto i = community.begin(); i != community.end(); ++i)
	{
		assert(0 <= *i && *i < graph.size1());
		if (*i != point)
			degree += graph(point, *i);
	}
	return degree;
}

template <typename _TReal>
_TReal OuterDegree(const boost::numeric::ublas::symmetric_matrix<_TReal> &graph, const std::vector<std::set<size_t> > &communities, const size_t point)
{
	assert(graph.size1() == graph.size2());
	_TReal degree = 0;
	for (size_t i = 0; i < communities.size(); ++i)
	{
		const std::set<size_t> &community = communities[i];
		if (community.find(point) == community.end())
		{
			for (auto j = community.begin(); j != community.end(); ++j)
			{
				assert(0 <= *j && *j < graph.size1());
				degree += graph(point, *j);
			}
		}
	}
	return degree;
}

template <typename _TReal>
_TReal CorrelationDegree(const boost::numeric::ublas::symmetric_matrix<_TReal> &graph, const std::set<size_t> &community, const size_t point)
{
	assert(graph.size1() == graph.size2());
	assert(community.find(point) == community.end());
	_TReal degree = 0;
	for (auto i = community.begin(); i != community.end(); ++i)
	{
		assert(0 <= *i && *i < graph.size1());
		degree += graph(point, *i);
	}
	return degree;
}

template <typename _TReal>
_TReal CommunityInnerDegree(const boost::numeric::ublas::symmetric_matrix<_TReal> &graph, const std::set<size_t> &community)
{
	assert(graph.size1() == graph.size2());
	_TReal degree = 0;
	for (auto i = community.begin(); i != community.end(); ++i)
	{
		for (auto j = community.begin(); j != community.end(); ++j)
		{
			assert(0 <= *j && *j < graph.size1());
			if (i != j)
				degree += graph(*i, *j);
		}
	}
	return degree;
}

template <typename _TReal>
_TReal CommunityOuterDegree(const boost::numeric::ublas::symmetric_matrix<_TReal> &graph, const std::vector<std::set<size_t> > &communities, const size_t index)
{
	assert(graph.size1() == graph.size2());
	_TReal degree = 0;
	const std::set<size_t> &community = communities[index];
	for (auto i = community.begin(); i != community.end(); ++i)
	{
		for (size_t _index = 0; _index < communities.size(); ++_index)
		{
			if (_index != index)
			{
				const std::set<size_t> &_community = communities[_index];
				for (auto j = _community.begin(); j != _community.end(); ++j)
				{
					assert(*i != *j);
					degree += graph(*i, *j);
				}
			}
		}
	}
	return degree;
}

template <typename _TReal>
_TReal CommunityCorrelationDegree(const boost::numeric::ublas::symmetric_matrix<_TReal> &graph, const std::set<size_t> &community1, const std::set<size_t> &community2)
{
	assert(graph.size1() == graph.size2());
	_TReal degree = 0;
	for (auto i = community1.begin(); i != community1.end(); ++i)
	{
		for (auto j = community2.begin(); j != community2.end(); ++j)
		{
			assert(*i != *j);
			degree += graph(*i, *j);
		}
	}
	return degree;
}

template <typename _TReal>
_TReal CommunityDegree(const boost::numeric::ublas::symmetric_matrix<_TReal> &graph)
{
	assert(graph.size1() == graph.size2());
	_TReal degree = 0;
	for (size_t i = 0; i < graph.size1(); ++i)
	{
		for (size_t j = 0; j < graph.size1(); ++j)
		{
			if (i != j)
				degree += graph(i, j);
		}
	}
	return degree;
}

template <typename _TReal>
_TReal AverageInnerDegree(const boost::numeric::ublas::symmetric_matrix<_TReal> &graph, const std::set<size_t> &community, const size_t point)
{
	assert(graph.size1() == graph.size2());
	_TReal degree = 0;
	for (auto i = community.begin(); i != community.end(); ++i)
	{
		assert(0 <= *i && *i < graph.size1());
		if (*i != point)
			degree += graph(point, *i);
	}
	degree /= community.size() + 1;
	return degree;
}

template <typename _TReal>
_TReal AverageOuterDegree(const boost::numeric::ublas::symmetric_matrix<_TReal> &graph, const std::vector<std::set<size_t> > &communities, const size_t point)
{
	assert(graph.size1() == graph.size2());
	_TReal degree = 0;
	_TReal min = 1;
	for (size_t i = 0; i < communities.size(); ++i)
	{
		const std::set<size_t> &community = communities[i];
		if (community.find(point) == community.end())
		{
			for (auto j = community.begin(); j != community.end(); ++j)
			{
				assert(0 <= *j && *j < graph.size1());
				degree += graph(point, *j);
			}
			degree /= community.size() + 1;
		}
		if (min > degree)
			min = degree;
	}
	return min;
}

template <typename _TReal>
_TReal Ai(const boost::numeric::ublas::symmetric_matrix<_TReal> &graph, const std::set<size_t> &community, const size_t point)
{
	assert(graph.size1() == graph.size2());
	_TReal ai = 0;
	for (auto i = community.begin(); i != community.end(); ++i)
	{
		assert(0 <= *i && *i < graph.size1());
		if (*i != point)
			ai += graph(point, *i);
	}
	ai /= community.size() - 1;
	return ai;
}


template <typename _TReal>
_TReal Bi(const boost::numeric::ublas::symmetric_matrix<_TReal> &graph, const std::vector<std::set<size_t> > &communities,const std::set<size_t> &community)
{
	assert(graph.size1() == graph.size2());
	_TReal bi = 0;
	for (auto k = community.begin(); k != community.end(); ++k)
		for (size_t i = 0; i < communities.size(); ++i)
		{
			_TReal degree = 0;
			const std::set<size_t> &community1 = communities[i];
			if (community1.find(*k) == community1.end())
			{
				for (auto j = community1.begin(); j != community1.end(); ++j)
				{
					assert(0 <= *j && *j < graph.size1());
					degree += graph(*k, *j);
				}
				degree /= community1.size();
			}
			assert(0 <= degree && degree <= 1);
			if(bi < degree)
			   bi = degree;
		}
	return bi;
}


template <typename _TReal>
_TReal E(const boost::numeric::ublas::symmetric_matrix<_TReal> &graph, const std::vector<std::set<size_t> > &communities)
{
	assert(graph.size1() == graph.size2());
	_TReal e = 0;

	for (size_t i = 0; i < communities.size(); ++i)
	{
		const std::set<size_t> &community = communities[i];
		for (auto j = community.begin(); j != community.end(); ++j)
		{
			for (size_t k = 0; k < communities.size(); ++k)
			{
				const std::set<size_t> &community1 = communities[k];
//				if (community.find(*j) == community.end())
					for (auto m = community1.begin(); m != community1.end(); ++m)
						{
							assert(0 <= *m && *m < graph.size1());
							if (*j != *m)
							   e += graph(*j, *m);
						}
			}
		}
//	e /= 2;
	return e;
}
}
}
}
}
}
