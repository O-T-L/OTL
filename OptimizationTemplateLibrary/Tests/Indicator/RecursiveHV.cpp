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
#include <OTL/Utility/Weight/NormalBoundaryIntersection.h>
#include <OTL/Problem/DTLZ/Shape/Utility.h>
#include <OTL/Problem/DTLZ/Shape/Concave.h>
#include <OTL/Indicator/Hypervolume/RecursiveHV.h>

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

template <typename _TReal>
std::vector<std::vector<_TReal> > GenerateSphere(const size_t dimension, const size_t count)
{
	typedef std::vector<_TReal> _TPoint;
	const std::list<_TPoint> decisions = otl::utility::weight::NormalBoundaryIntersection<_TReal>(std::vector<size_t>(dimension - 2, count));
	std::vector<_TPoint> points(decisions.size());
	typename std::list<_TPoint>::const_iterator src = decisions.begin();
	for (size_t i = 0; i < points.size(); ++i)
	{
		points[i].resize(dimension);
		_TPoint angle = *src;
		otl::problem::dtlz::shape::ConvertAngles<_TReal>(angle.begin(), angle.end());
		_TPoint &point = points[i];
		otl::problem::dtlz::shape::Concave(angle.begin(), angle.end(), point.begin(), point.end(), (_TReal)1);
		++src;
	}
	return points;
}

BOOST_AUTO_TEST_CASE(RecursiveHVInput2)
{
	typedef double _TReal;
	typedef otl::indicator::hypervolume::RecursiveHV<_TReal> _TIndicator;
	typedef _TIndicator::TMetric _TMetric;
	typedef _TIndicator::TPoint _TPoint;
	_TPoint point(2);
	point[0] = 1;
	point[1] = 1;
	_TIndicator indicator(point);
	std::vector<_TPoint> front;
	point[0] = 0.1;
	point[1] = 0.9;
	front.push_back(point);
	point[0] = 0.5;
	point[1] = 0.5;
	front.push_back(point);
	point[0] = 0.9;
	point[1] = 0.1;
	front.push_back(point);
	BOOST_CHECK_CLOSE(indicator(front), 0.1 * 0.9 + 0.4 * 0.5 + 0.4 * 0.1, 0.0001);
}

BOOST_AUTO_TEST_CASE(RecursiveHVInput3)
{
	typedef double _TReal;
	typedef otl::indicator::hypervolume::RecursiveHV<_TReal> _TIndicator;
	typedef _TIndicator::TMetric _TMetric;
	typedef _TIndicator::TPoint _TPoint;
	_TPoint point(3);
	point[0] = 1;
	point[1] = 1;
	point[2] = 1;
	_TIndicator indicator(point);
	std::vector<_TPoint> front;
	// Z = 0.7
	point[0] = 0.1;
	point[1] = 0.9;
	point[2] = 0.7;
	front.push_back(point);
	point[0] = 0.5;
	point[1] = 0.5;
	point[2] = 0.7;
	front.push_back(point);
	point[0] = 0.9;
	point[1] = 0.1;
	point[2] = 0.7;
	front.push_back(point);
	const _TMetric Volume1 = (0.1 * 0.9 + 0.4 * 0.5 + 0.4 * 0.1) * 0.3;
	BOOST_CHECK_CLOSE(indicator(front), Volume1, 0.0001);
	// Z = 0.5
	point[0] = 0.7;
	point[1] = 0.7;
	point[2] = 0.5;
	front.push_back(point);
	const _TMetric Volume2 = (0.3 * 0.3) * 0.2;
	BOOST_CHECK_CLOSE(indicator(front), Volume1 + Volume2, 0.0001);
}

BOOST_AUTO_TEST_CASE(RecursiveHVCircle)
{
	typedef double _TReal;
	typedef otl::indicator::hypervolume::RecursiveHV<_TReal> _TIndicator;
	typedef _TIndicator::TMetric _TMetric;
	typedef _TIndicator::TPoint _TPoint;
	const _TReal radius = 1;
	const std::list<_TPoint> _points = GenerateCirclePoints(radius, 100);
	const std::vector<_TPoint> points(_points.begin(), _points.end());
	const _TReal _referencePoint = radius + 1;
	const _TPoint referencePoint(points.front().size(), _referencePoint);
	_TIndicator indicator(referencePoint);
	BOOST_CHECK_CLOSE(indicator(points), 3.2106680495668716, 0.00001);
}

BOOST_AUTO_TEST_CASE(RecursiveHVCircle2)
{
	typedef double _TReal;
	typedef otl::indicator::hypervolume::RecursiveHV<_TReal> _TIndicator;
	typedef _TIndicator::TMetric _TMetric;
	typedef _TIndicator::TPoint _TPoint;
	const _TReal radius1 = 1;
	const _TReal radius2 = 2;
	std::list<_TPoint> points1 = GenerateCirclePoints(radius1, 100);
	std::list<_TPoint> points2 = GenerateCirclePoints(radius2, 100);
	std::list<_TPoint> _points;
	_points.splice(_points.end(), points1, points1.begin());
	_points.splice(_points.end(), points2, points2.begin());
	const std::vector<_TPoint> points(_points.begin(), _points.end());
	const _TReal _referencePoint = std::max(radius1, radius2) + 1;
	const _TPoint referencePoint(points.front().size(), _referencePoint);
	_TIndicator indicator(referencePoint);
	BOOST_CHECK_CLOSE(indicator(points), 6, 0.00001);
}

BOOST_AUTO_TEST_CASE(RecursiveHVFlat3)
{
	typedef double _TReal;
	typedef otl::indicator::hypervolume::RecursiveHV<_TReal> _TIndicator;
	typedef _TIndicator::TMetric _TMetric;
	typedef _TIndicator::TPoint _TPoint;
	const size_t nObjectives = 3;
	const std::list<_TPoint> _front = otl::utility::weight::NormalBoundaryIntersection<_TReal>(std::vector<size_t>(nObjectives - 1, 10));
	const std::vector<_TPoint> front(_front.begin(), _front.end());
	const _TPoint refPoint(front.front().size(), 2);
	_TIndicator indicator(refPoint);
	BOOST_CHECK_CLOSE(indicator(front), 7.7800000000000002, 0.00001);
}

BOOST_AUTO_TEST_CASE(RecursiveHVSphere3)
{
	typedef double _TReal;
	typedef otl::indicator::hypervolume::RecursiveHV<_TReal> _TIndicator;
	typedef _TIndicator::TMetric _TMetric;
	typedef _TIndicator::TPoint _TPoint;
	const size_t nObjectives = 3;
	const std::vector<_TPoint> front = GenerateSphere<_TReal>(nObjectives - 1, 10);
	const _TPoint refPoint(front.front().size(), 2);
	_TIndicator indicator(refPoint);
	BOOST_CHECK_CLOSE(indicator(front), 2, 0.00001);
}

BOOST_AUTO_TEST_CASE(RecursiveHVFlat4)
{
	typedef double _TReal;
	typedef otl::indicator::hypervolume::RecursiveHV<_TReal> _TIndicator;
	typedef _TIndicator::TMetric _TMetric;
	typedef _TIndicator::TPoint _TPoint;
	const size_t nObjectives = 4;
	const std::list<_TPoint> _front = otl::utility::weight::NormalBoundaryIntersection<_TReal>(std::vector<size_t>(nObjectives - 1, 5));
	const std::vector<_TPoint> front(_front.begin(), _front.end());
	const _TPoint refPoint(front.front().size(), 2);
	_TIndicator indicator(refPoint);
	BOOST_CHECK_CLOSE(indicator(front), 15.888000000000002, 0.00001);
}

BOOST_AUTO_TEST_CASE(RecursiveHVFlat5)
{
	typedef double _TReal;
	typedef otl::indicator::hypervolume::RecursiveHV<_TReal> _TIndicator;
	typedef _TIndicator::TMetric _TMetric;
	typedef _TIndicator::TPoint _TPoint;
	const size_t nObjectives = 5;
	const std::list<_TPoint> _front = otl::utility::weight::NormalBoundaryIntersection<_TReal>(std::vector<size_t>(nObjectives - 1, 3));
	const std::vector<_TPoint> front(_front.begin(), _front.end());
	const _TPoint refPoint(front.front().size(), 2);
	_TIndicator indicator(refPoint);
	BOOST_CHECK_CLOSE(indicator(front), 31.913580246913583, 0.00001);
}

BOOST_AUTO_TEST_CASE(RecursiveHVSamePoints)
{
	typedef double _TReal;
	typedef otl::indicator::hypervolume::RecursiveHV<_TReal> _TIndicator;
	typedef _TIndicator::TMetric _TMetric;
	typedef _TIndicator::TPoint _TPoint;
	_TPoint point(3);
	point[0] = 0.46291356661423427;
	point[1] = 1.244693272766431;
	point[2] = 1.9401231443048559;
	const std::vector<_TPoint> points(3, point);
	_TPoint referencePoint(point.size());
	for (size_t i = 0; i < point.size(); ++i)
		referencePoint[i] = point[i] + 1;
	_TIndicator indicator(referencePoint);
	BOOST_CHECK_CLOSE(indicator(points), 1, 0.00001);
}
