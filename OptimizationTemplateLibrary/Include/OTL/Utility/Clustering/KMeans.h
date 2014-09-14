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
template <typename _TPoint>
size_t NearestCentroid(const _TPoint &point, const std::vector<_TPoint> &centroids)
{
	typedef typename _TPoint::value_type _TReal;
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

template <typename _TIterator>
typename std::iterator_traits<typename std::iterator_traits<_TIterator>::value_type>::value_type CalculateCentroid(_TIterator begin, _TIterator end)
{
	typedef typename std::iterator_traits<_TIterator>::value_type _TPointer;
	typedef typename std::iterator_traits<_TPointer>::value_type _TPoint;
	typedef typename _TPoint::value_type _TReal;
	_TPoint centroid = **begin;
	for (size_t n = 0; n < centroid.size(); ++n)
	{
		size_t count = 1;
		for (_TIterator i = ++begin; i != end; ++i)
		{
			centroid[n] += (**i)[n];
			++count;
		}
		centroid[n] /= count;
	}
	return centroid;
}

template <typename _TIterator, typename _TPoint>
std::vector<std::list<const _TPoint *> > KMeans(_TIterator begin, _TIterator end, std::vector<_TPoint> &centroids, size_t iteration = -1)
{
	typedef const _TPoint * _TPointer;
	typedef typename _TPoint::value_type _TReal;
	assert(centroids.size() > 0);
	std::vector<std::list<_TPointer> > clusters(centroids.size());
	{//Initialize
		assert(std::distance(begin, end) >= centroids.size());
		_TIterator src = begin;
		for (size_t n = 0; n < clusters.size(); ++n)
		{
			_TPointer pointer = *src;
			centroids[n] = *pointer;
			clusters[n].push_back(pointer);
			++src;
		}
		while (src != end)
		{
			_TPointer pointer = *src;
			const size_t index = NearestCentroid(*pointer, centroids);
			clusters[index].push_back(pointer);
			++src;
		}
	}
	while (iteration)
	{
		std::vector<_TPoint> _centroids(centroids.size());
		for (size_t n = 0; n < clusters.size(); ++n)
			_centroids[n] = CalculateCentroid(clusters[n].begin(), clusters[n].end());
		if (_centroids == centroids)
			break;
		centroids = _centroids;
		for (size_t n = 0; n < clusters.size(); ++n)
			clusters[n].clear();
		for (_TIterator i = begin; i != end; ++i)
		{
			_TPointer pointer = *i;
			const size_t index = NearestCentroid(*pointer, centroids);
			clusters[index].push_back(pointer);
		}
		--iteration;
	}
	return clusters;
}
}
}
}
