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
  Title                    = {Algorithm {AS} 136: A {K-Means} Clustering Algorithm},
  Author                   = {J. A. Hartigan and M. A. Wong},
  Journal                  = {Journal of the Royal Statistical Society},
  Year                     = {1979},
  Number                   = {1},
  Pages                    = {100-108},
  Volume                   = {28},
  Url                      = {www.jstor.org/stable/2346830}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <list>
#include <iterator>
#include <limits>

namespace otl
{
namespace utility
{
namespace clustering
{
template <typename _TReal>
size_t NearestCentroid(const std::vector<_TReal> &point, const std::vector<std::vector<_TReal> > &centroids)
{
	assert(centroids.size() > 0);
	size_t index = 0;
	_TReal distance = sqrt(std::inner_product(point.begin(), point.end(), centroids[index].begin(), (_TReal)0
		, std::plus<_TReal>()
		, [](_TReal x, _TReal y)->_TReal{_TReal t = x - y;return t * t;}
	));
	for (size_t i = 1; i < centroids.size(); ++i)
	{
		_TReal _distance = sqrt(std::inner_product(point.begin(), point.end(), centroids[i].begin(), (_TReal)0
			, std::plus<_TReal>()
			, [](_TReal x, _TReal y)->_TReal{_TReal t = x - y;return t * t;}
		));
		if (_distance < distance)
		{
			distance = _distance;
			index = i;
		}
	}
	return index;
}

template <typename _TReal, typename _TUnit, typename _TFetcher>
std::vector<_TReal> CalculateCentroid(const std::list<const _TUnit *> &cluster, _TFetcher fetcher)
{
	typedef std::vector<_TReal> _TCoordinate;
	_TCoordinate centroid = fetcher(*cluster.front());
	for (size_t n = 0; n < centroid.size(); ++n)
	{
		size_t count = 1;
		for (auto i = ++cluster.begin(); i != cluster.end(); ++i)
		{
			const _TCoordinate point = fetcher(**i);
			centroid[n] += point[n];
			++count;
		}
		centroid[n] /= count;
	}
	return centroid;
}

template <typename _TUnit, typename _TReal, typename _TFetcher>
std::vector<std::list<const _TUnit *> > KMeans(const std::vector<const _TUnit *> &units, std::vector<std::vector<_TReal> > &centroids, _TFetcher fetcher, size_t iteration = -1)
{
	typedef std::vector<_TReal> _TCoordinate;
	assert(centroids.size() > 0);
	std::vector<std::list<const _TUnit *> > clusters(centroids.size());
	{//Initialize
		assert(units.size() >= centroids.size());
		size_t index = 0;
		for (size_t n = 0; n < clusters.size(); ++n)
		{
			const _TUnit *unit = units[index];
			centroids[n] = fetcher(*unit);
			clusters[n].push_back(unit);
			++index;
		}
		while (index < units.size())
		{
			const _TUnit *unit = units[index];
			const size_t cluster = NearestCentroid(fetcher(*unit), centroids);
			clusters[cluster].push_back(unit);
			++index;
		}
	}
	while (iteration)
	{
		std::vector<_TCoordinate> _centroids(centroids.size());
		for (size_t n = 0; n < clusters.size(); ++n)
			_centroids[n] = CalculateCentroid<_TReal>(clusters[n], fetcher);
		if (_centroids == centroids)
			break;
		centroids = _centroids;
		for (size_t n = 0; n < clusters.size(); ++n)
			clusters[n].clear();
		for (size_t i = 0; i < units.size(); ++i)
		{
			const _TUnit *unit = units[i];
			const size_t cluster = NearestCentroid(fetcher(*unit), centroids);
			clusters[cluster].push_back(unit);
		}
		--iteration;
	}
	return clusters;
}
}
}
}
