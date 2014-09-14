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

/*
@Article{,
  Title                    = {Finding and evaluating community structure in networks},
  Author                   = {M. E. J. Newman and M. Girvan},
  Journal                  = {Physical Review E},
  Year                     = {2004},
  Number                   = {2},
  Pages                    = {026113},
  Volume                   = {69},
  Doi                      = {10.1103/PhysRevE.69.026113}
}
*/

#pragma once

#include <cassert>
#include "Metric.h"
#include "Utility.h"

namespace otl
{
namespace problem
{
namespace community_discovery
{
namespace metric
{
template <typename _TReal, typename _TMatrix>
class Q : public Metric<_TReal, _TMatrix>
{
public:
	typedef _TReal TReal;
	typedef _TMatrix TMatrix;
	typedef Metric<TReal, TMatrix> TSuper;

	Q(void);
	~Q(void);

protected:
	TReal _DoEvaluate(const TMatrix &graph, const std::vector<std::set<size_t> > &communities);
};

template <typename _TReal, typename _TMatrix>
Q<_TReal, _TMatrix>::Q(void)
	: TSuper(true)
{
}

template <typename _TReal, typename _TMatrix>
Q<_TReal, _TMatrix>::~Q(void)
{
}

template <typename _TReal, typename _TMatrix>
_TReal Q<_TReal, _TMatrix>::_DoEvaluate(const TMatrix &graph, const std::vector<std::set<size_t> > &communities)
{
	const TReal degree = CommunityDegree(graph);
	TReal q = 0;
	for (size_t i = 0; i < communities.size(); ++i)
	{
		const std::set<size_t> &community = communities[i];
		const TReal innerDegree = CommunityInnerDegree(graph, community);
		const TReal outerDegree = CommunityOuterDegree(graph, communities, i);
		const TReal temp = outerDegree / degree;
		q += innerDegree / degree - temp * temp;
	}
	return q;
}
}
}
}
}
