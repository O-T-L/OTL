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
BOOST_AUTO_TEST_CASE(TestKMeans)
{
	int a;
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	struct _TUnit
	{
		size_t class_;
		std::vector<_TReal> coordiante_;
	};
	_TRandom random;
	std::vector<std::vector<_TUnit> > units(2, std::vector<_TUnit>(50));
	std::list<const _TUnit *> _pointers;
	for (size_t n = 0; n < units.size(); ++n)
	{
		const _TReal base = n * 2;
		for (size_t i = 0; i < units[n].size(); ++i)
		{
			_TUnit &unit = units[n][i];
			unit.class_ = n;
			unit.coordiante_.resize(3);
			std::uniform_real_distribution<_TReal> dist;
			for (size_t j = 0; j < unit.coordiante_.size(); ++j)
				unit.coordiante_[j] = base + dist(random);
			_pointers.push_back(&unit);
		}
	}
	std::vector<const _TUnit *> pointers(_pointers.begin(), _pointers.end());
	std::random_shuffle(pointers.begin(), pointers.end(), [&random](const size_t n)-> size_t{std::uniform_int_distribution<size_t> dist(0, n - 1);return dist(random);});
	std::vector<std::vector<_TReal> > centroids(units.size());
	auto clusters = otl::utility::clustering::KMeans(pointers, centroids, [](const _TUnit &unit)->const std::vector<_TReal> &{return unit.coordiante_;});
	BOOST_CHECK_EQUAL(clusters.size(), centroids.size());
	{
		std::list<const _TUnit *> merge;
		for (size_t n = 0; n < clusters.size(); ++n)
		{
			auto &cluster = clusters[n];
			for (auto i = cluster.begin(); i != cluster.end(); ++i)
			{
				BOOST_CHECK(std::find(merge.begin(), merge.end(), *i) == merge.end());
				const _TUnit &unit = **i;
				BOOST_CHECK_EQUAL(unit.class_, n);
			}
			merge.splice(merge.end(), cluster, cluster.begin(), cluster.end());
		}
		BOOST_CHECK_EQUAL(merge.size(), pointers.size());
	}
}
}
