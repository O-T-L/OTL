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
#include <boost/math/constants/constants.hpp>
#include <OTL/Indicator/Hypervolume/MonteCarloHV.h>

namespace montecarlo_hv
{
template <typename _TReal>
std::list<std::vector<_TReal> > GenerateCirclePoints(const _TReal radius, const size_t nPoints)
{
	typedef std::vector<_TReal> _TPoint;
	std::list<_TPoint> population;
	for (size_t i = 0; i < nPoints; ++i)
	{
		const _TReal angle = i * boost::math::constants::pi<_TReal>() / 2 / (nPoints - 1);
		std::vector<_TReal> point(2);
		point[0] = radius * cos(angle);
		point[1] = radius * sin(angle);
		population.push_back(point);
	}
	return population;
}

BOOST_AUTO_TEST_CASE(MonteCarloHVCircle)
{
	typedef double _TReal;
	typedef std::mt19937 _TRandom;
	typedef otl::indicator::hypervolume::MonteCarloHV<_TReal, _TRandom &> _TIndicator;
	typedef _TIndicator::TMetric _TMetric;
	typedef _TIndicator::TPoint _TPoint;
	const _TReal radius = 1;
	const std::list<_TPoint> _points = GenerateCirclePoints(radius, 100);
	const std::vector<_TPoint> points(_points.begin(), _points.end());
	const _TReal referencePoint = radius + 1;
	_TRandom random;
	_TIndicator indicator(std::vector<_TReal>(2, referencePoint), random, 10000000);
	BOOST_CHECK_CLOSE(indicator(points), 3.2106680495668716, 0.1);
}
}
