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

namespace otl
{
namespace problem
{
namespace community_discovery
{
namespace metric
{
template <typename _TReal, typename _TMatrix>
class Metric
{
public:
	typedef _TReal TReal;
	typedef _TMatrix TMatrix;

	const bool maximize_;

	Metric(const bool maximize);
	virtual ~Metric(void);
	TReal operator ()(const TMatrix &graph, const std::vector<std::set<size_t> > &communities);

protected:
	virtual TReal _DoEvaluate(const TMatrix &graph, const std::vector<std::set<size_t> > &communities) = 0;
};

template <typename _TReal, typename _TMatrix>
Metric<_TReal, _TMatrix>::Metric(const bool maximize)
	: maximize_(maximize)
{
}

template <typename _TReal, typename _TMatrix>
Metric<_TReal, _TMatrix>::~Metric(void)
{
}

template <typename _TReal, typename _TMatrix>
_TReal Metric<_TReal, _TMatrix>::operator ()(const TMatrix &graph, const std::vector<std::set<size_t> > &communities)
{
	return _DoEvaluate(graph, communities);
}
}
}
}
}
