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
#include <OTL/Utility/Nondominate.h>
#include <OTL/Utility/Relation/Pareto.h>

namespace nondominate
{
template <typename _TRandom, typename _TReal>
std::vector<_TReal> GeneratePoint(_TRandom &random, const std::vector<std::pair<_TReal, _TReal> > &boundary)
{
	std::vector<_TReal> point(boundary.size());
	for (size_t i = 0; i < point.size(); ++i)
	{
		std::uniform_real_distribution<_TReal> dist(boundary[i].first, boundary[i].second);
		point[i] = dist(random);
	}
	return point;
}

template <typename _TRandom, typename _TReal>
std::list<std::vector<_TReal> > GeneratePoints(_TRandom &random, const std::vector<std::pair<_TReal, _TReal> > &boundary, const size_t nPoints)
{
	typedef std::vector<_TReal> _TPoint;
	std::list<_TPoint> population;
	for (size_t i = 0; i < nPoints; ++i)
		population.push_back(GeneratePoint(random, boundary));
	return population;
}

template <typename _TIndividual, typename _TIterator, typename _TDominate>
bool ExistDominate(_TIndividual &individual, _TIterator begin, _TIterator end, _TDominate dominate)
{
	for (_TIterator i = begin; i != end; ++i)
	{
		if (dominate(*i, individual))
			return true;
	}
	return false;
}

BOOST_AUTO_TEST_CASE(Nondominate)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef std::vector<_TReal> _TPoint;
	typedef std::pair<_TReal, _TReal> _TMinMax;
	typedef std::vector<_TMinMax> _TBoundary;
	_TBoundary boundary(10, _TMinMax(0, 1));
	_TRandom random;
	const size_t points = 200;
	auto population = GeneratePoints(random, boundary, points);
	auto dominate = &otl::utility::relation::Dominate<_TReal>;
	auto nondominate = otl::utility::ExtractNondominate(population, dominate);
	BOOST_CHECK(!nondominate.empty());
	BOOST_CHECK(nondominate.size() + population.size() == points);
	for (auto elite = nondominate.begin(); elite != nondominate.end(); ++elite)
	{
		for (auto _elite = nondominate.begin(); _elite != nondominate.end(); ++_elite)
		{
			if (_elite != elite)
				BOOST_CHECK(!dominate(*_elite, *elite));
		}
		for (auto individual = population.begin(); individual != population.end(); ++individual)
			BOOST_CHECK(!dominate(*individual, *elite));
	}
	for (auto individual = population.begin(); individual != population.end(); ++individual)
		BOOST_CHECK(ExistDominate(*individual, nondominate.begin(), nondominate.end(), dominate));
}
}
