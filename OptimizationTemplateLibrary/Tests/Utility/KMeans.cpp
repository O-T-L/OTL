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

#include <cmath>
#include <vector>
#include <list>
#include <random>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <OTL/Utility/Clustering/KMeans.h>

namespace clustering
{
BOOST_AUTO_TEST_CASE(TestK_Means)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef std::vector<_TReal> _TPoint;
	typedef const _TPoint * _TConstPointer;
	_TRandom random;
	const size_t nPoints = 100;
	std::vector<_TPoint> points(nPoints);
	std::vector<_TConstPointer> _points(points.size());
	for (size_t i = 0; i < points.size(); ++i)
	{
		_TPoint &point = points[i];
		point.resize(3);
		std::uniform_real_distribution<_TReal> dist;
		for (size_t j = 0; j < point.size(); ++j)
			point[j] = dist(random);
		_points[i] = &point;
	}
	std::random_shuffle(_points.begin(), _points.end(), [&random](const size_t n)-> size_t{std::uniform_int_distribution<size_t> dist(0, n - 1);return dist(random);});
	std::vector<_TPoint> centroids(5);
	std::vector<std::list<_TConstPointer> > clusters = otl::utility::clustering::KMeans(_points.begin(), _points.end(), centroids);
	BOOST_CHECK_EQUAL(clusters.size(), centroids.size());
	{
		std::list<_TConstPointer> merge;
		for (size_t index = 0; index < clusters.size(); ++index)
		{
			std::list<_TConstPointer> &cluster = clusters[index];
			for (auto i = cluster.begin(); i != cluster.end(); ++i)
				BOOST_CHECK(std::find(merge.begin(), merge.end(), *i) == merge.end());
			merge.splice(merge.end(), cluster, cluster.begin(), cluster.end());
		}
		BOOST_CHECK_EQUAL(merge.size(), nPoints);
	}
}
}
