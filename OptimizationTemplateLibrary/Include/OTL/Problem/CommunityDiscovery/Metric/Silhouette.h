/*!
Copyright (C) 2014, 林继森 (Jisen Lin)

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
class Silhouette : public Metric<_TReal, _TMatrix>
{
public:
	typedef _TReal TReal;
	typedef _TMatrix TMatrix;
	typedef Metric<TReal, TMatrix> TSuper;

	Silhouette(void);
	~Silhouette(void);

protected:
	TReal _DoEvaluate(const TMatrix &graph, const std::vector<std::set<size_t> > &communities);
};

template <typename _TReal, typename _TMatrix>
Silhouette<_TReal, _TMatrix>::Silhouette(void)
	: TSuper(true)
{
}

template <typename _TReal, typename _TMatrix>
Silhouette<_TReal, _TMatrix>::~Silhouette(void)
{
}

template <typename _TReal, typename _TMatrix>
_TReal Silhouette<_TReal, _TMatrix>::_DoEvaluate(const TMatrix &graph, const std::vector<std::set<size_t> > &communities)
{
//	const TReal degree = CommunityDegree(graph);
	const TReal temp = 0;
	TReal gs = 0;
	for (size_t i = 0; i < communities.size(); ++i)
	{
		const std::set<size_t> &community = communities[i];
		if (community.find(point) == community.end())
		{
			for (auto j = community.begin(); j != community.end(); ++j)
			{
				assert(0 <= *j && *j < graph.size1());
				const TReal inner = AverageInnerDegree(graph, community, j);
				const TReal outer = AverageOuterDegree(graph, communities, j);
				temp += inner - outer;
				temp /= std::max(inner, outer);
			}
		}
		gs += 1.0 / community.end() * temp;
	}
	gs /= communities.size();
	return gs;
}
}
}
}
}
