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
#include <OTL/Crossover/SimulatedBinaryCrossover.h>

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

BOOST_AUTO_TEST_CASE(SBX)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef _TCrossover::TDecision _TDecision;
	typedef _TCrossover::TSolution _TSolution;
	typedef _TCrossover::TBoundary _TBoundary;
	typedef _TCrossover::TMinMax _TMinMax;
	_TBoundary boundary(10, _TMinMax(0, 1));
	_TRandom random;
	_TSolution parent1, parent2, child1, child2;
	parent1.decision_ = GeneratePoint(random, boundary);
	parent2.decision_ = GeneratePoint(random, boundary);
	_TDecision middle(boundary.size());
	for (size_t i = 0; i < middle.size(); ++i)
		middle[i] = (parent1.decision_[i] + parent2.decision_[i]) / 2;
	_TCrossover crossover(random, 1, boundary, 20);
	for (size_t repeat = 0; repeat < 5; ++repeat)
	{
		crossover(parent1, parent2, child1, child2);
		for (size_t i = 0; i < middle.size(); ++i)
			BOOST_CHECK_CLOSE(middle[i], (parent1.decision_[i] + parent2.decision_[i]) / 2, 0.001);
	}
}
