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
@ARTICLE{,
  author = {Changhe Li and Shengxiang Yang},
  title = {A General Framework of Multipopulation Methods With Clustering in
	Undetectable Dynamic Environments},
  journal = {IEEE Transactions on Evolutionary Computation},
  year = {2012},
  volume = {16},
  pages = {556-577},
  number = {4},
  month = {26 July},
  doi = {10.1109/TEVC.2011.2169966}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <list>
#include <limits>

namespace otl
{
namespace utility
{
template <typename _TReal, typename _TPoint>
class Cluster
{
public:
	std::vector<_TReal> center_;
	std::list<_TPoint> points_;
};

template <typename _TReal, typename _TClusterIterator, typename _TFetcher>
std::pair<_TClusterIterator, _TClusterIterator> LocateMinEdge(_TClusterIterator begin, _TClusterIterator end, _TFetcher fetcher)
{
	std::pair<_TClusterIterator, _TClusterIterator> minEdge(end, end);
	_TReal minDistance = std::numeric_limits<_TReal>::max();
	for (_TClusterIterator i = begin; i != end; ++i)
	{
		for (_TClusterIterator j = begin; j != end; ++j)
		{
			if (i != j)
			{
				const std::vector<_TReal> &point1 = fetcher(*i);
				const std::vector<_TReal> &point2 = fetcher(*j);
				const _TReal distance = sqrt(std::inner_product(point1.begin(), point1.end(), point2.begin(), (_TReal)0, std::plus<_TReal>()
					, [](_TReal x, _TReal y)->_TReal{_TReal t = x - y; return t * t;}));
				if (distance < minDistance)
				{
					minDistance = distance;
					minEdge.first = i;
					minEdge.second = j;
				}
			}
		}
	}
	return minEdge;
}

template <typename _TReal>
std::vector<_TReal> AverageCenter(const std::vector<_TReal> &center1, const std::vector<_TReal> &center2)
{
	assert(center1.size() == center2.size());
	std::vector<_TReal> center(center1.size());
	for (size_t i = 0; i < center.size(); ++i)
		center[i] = (center1[i] + center2[i]) / 2;
	return center;
}

template <typename _TReal, typename _TIterator, typename _TFetcher>
std::list<Cluster<_TReal, typename std::iterator_traits<_TIterator>::value_type> > Clustering(const size_t nClusters, _TIterator begin, _TIterator end, _TFetcher fetcher)
{
	typedef typename std::iterator_traits<_TIterator>::value_type _TPoint;
	typedef Cluster<_TReal, _TPoint> _TCluster;
	assert(nClusters < std::distance(begin, end));
	std::list<_TCluster> clusters;
	for (_TIterator i = begin; i != end; ++i)
	{
		_TCluster cluster;
		cluster.points_.push_back(*i);
		cluster.center_ = fetcher(*i);
		clusters.push_back(cluster);
	}
	for (size_t count = std::distance(begin, end) - nClusters; count; --count)
	{
		auto edge = LocateMinEdge<_TReal>(clusters.begin(), clusters.end(), fetcher);
		_TCluster &cluster1 = *edge.first;
		_TCluster &cluster2 = *edge.second;
		cluster1.center_ = AverageCenter(cluster1.center_, cluster2.center_);
		cluster1.points_.splice(cluster1.points_.end(), cluster2, cluster2.begin(), cluster2.end());
		clusters.erase(edge.second);
	}
	assert(clusters.size() == nClusters);
	return clusters;
}
}
}
