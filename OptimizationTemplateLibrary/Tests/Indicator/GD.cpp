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
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/constants/constants.hpp>
#include <OTL/Indicator/GD/FrontGD.h>
#include <OTL/Indicator/GD/DTLZ1GD.h>
#include <OTL/Indicator/GD/DTLZ2GD.h>
#include <OTL/Indicator/GD/WFG3GD.h>
#include <OTL/Indicator/GD/WFG4GD.h>

namespace gd
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

BOOST_AUTO_TEST_CASE(FrontGD)
{
	typedef double _TReal;
	typedef otl::indicator::gd::FrontGD<_TReal> _TIndicator;
	typedef _TIndicator::TMetric _TMetric;
	typedef _TIndicator::TPoint _TPoint;
	const std::list<_TPoint> _pf = GenerateCirclePoints<_TReal>(1, 10000);
	const std::vector<_TPoint> pf(_pf.begin(), _pf.end());
	const std::list<_TPoint> _front = GenerateCirclePoints<_TReal>(2, 100);
	const std::vector<_TPoint> front(_front.begin(), _front.end());
	_TIndicator indicator(pf);
	indicator(front);
}

BOOST_AUTO_TEST_CASE(DTLZ1GD)
{
	typedef double _TReal;
	typedef otl::indicator::gd::DTLZ1GD<_TReal> _TIndicator;
	typedef _TIndicator::TMetric _TMetric;
	typedef _TIndicator::TPoint _TPoint;
	const std::list<_TPoint> _front = GenerateCirclePoints<_TReal>(4, 100);
	const std::vector<_TPoint> front(_front.begin(), _front.end());
	_TIndicator indicator;
	indicator(front);
}

BOOST_AUTO_TEST_CASE(DTLZ2GD)
{
	typedef double _TReal;
	typedef otl::indicator::gd::DTLZ2GD<_TReal> _TIndicator;
	typedef _TIndicator::TMetric _TMetric;
	typedef _TIndicator::TPoint _TPoint;
	const std::list<_TPoint> _front = GenerateCirclePoints<_TReal>(4, 100);
	const std::vector<_TPoint> front(_front.begin(), _front.end());
	_TIndicator indicator;
	indicator(front);
}

BOOST_AUTO_TEST_CASE(WFG3GD)
{
	typedef double _TReal;
	typedef otl::indicator::gd::WFG3GD<_TReal> _TIndicator;
	typedef _TIndicator::TMetric _TMetric;
	typedef _TIndicator::TPoint _TPoint;
	const std::list<_TPoint> _front = GenerateCirclePoints<_TReal>(4, 100);
	const std::vector<_TPoint> front(_front.begin(), _front.end());
	_TIndicator indicator;
	indicator(front);
}

BOOST_AUTO_TEST_CASE(WFG4GD)
{
	typedef double _TReal;
	typedef otl::indicator::gd::WFG4GD<_TReal> _TIndicator;
	typedef _TIndicator::TMetric _TMetric;
	typedef _TIndicator::TPoint _TPoint;
	const std::list<_TPoint> _front = GenerateCirclePoints<_TReal>(4, 100);
	const std::vector<_TPoint> front(_front.begin(), _front.end());
	_TIndicator indicator;
	indicator(front);
}
}
