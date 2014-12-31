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

#include <cassert>
#include <random>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/constants/constants.hpp>
#include <OTL/Problem/DTLZ/DTLZ2.h>
#include <OTL/Initial/UniformReal.h>
#include <OTL/Crossover/SimulatedBinaryCrossover.h>
#include <OTL/Crossover/CoupleCoupleCrossoverAdapter.h>
#include <OTL/Mutation/PolynomialMutation.h>
#include <OTL/Optimizer/HypE/HypE.h>
#include <OTL/Optimizer/HypE/FastHypE.h>

namespace hype
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

template <typename _TReal, typename _TIterator, typename _TRandom>
_TReal CalculateMonteCarloHV(const std::vector<_TReal> &referencePoint, _TIterator begin, _TIterator end, _TRandom &random, const size_t nSample)
{
	typedef bool _TDecision;
	typedef otl::optimizer::hype::Individual<_TReal, _TDecision> _TIndividual;
	std::vector<_TIndividual> population(std::distance(begin, end));
	std::vector<_TIndividual *> _population(population.size());
	{
		size_t index = 0;
		for (_TIterator i = begin; i != end; ++i)
		{
			population[index].objective_ = *i;
			_population[index] = &population[index];
			++index;
		}
	}
	otl::optimizer::hype::FitnessEstimation(random, _population.begin(), _population.end(), referencePoint, nSample, _population.size());
	_TReal hv = 0;
	for (size_t i = 0; i < population.size(); ++i)
		hv += population[i].fitness_;
	return hv;
}

BOOST_AUTO_TEST_CASE(HypE_Fitness)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef std::vector<_TReal> _TPoint;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	const _TReal radius = 1;
	const auto points = GenerateCirclePoints(radius, 100);
	const std::vector<_TReal> referencePoint(2, radius + 1);
	const _TReal hv = CalculateMonteCarloHV(referencePoint, points.begin(), points.end(), random, 10000000);
	BOOST_CHECK_CLOSE(hv, 3.2106680495668716, 0.01);
}

BOOST_AUTO_TEST_CASE(HypE)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::dtlz::DTLZ2<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::hype::HypE<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	_TProblem problem(nObjectives);
	const std::vector<_TDecision> initial = otl::initial::PopulationUniformReal(random, problem.GetBoundary(), populationSize);
	_TCrossover _crossover(random, 1, problem.GetBoundary(), 20);
	otl::crossover::CoupleCoupleCrossoverAdapter<_TReal, _TDecision, _TRandom &> crossover(_crossover, random);
	_TMutation mutation(random, 1 / (_TReal)problem.GetBoundary().size(), problem.GetBoundary(), 20);
	_TOptimizer optimizer(random, problem, initial, crossover, mutation, 10000);
	BOOST_CHECK(problem.GetNumberOfEvaluations() == populationSize);
	for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
	{
		optimizer();
		BOOST_CHECK_EQUAL(problem.GetNumberOfEvaluations(), (generation + 1) * initial.size());
	}
}

BOOST_AUTO_TEST_CASE(FastHypE)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::dtlz::DTLZ2<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::hype::FastHypE<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	_TProblem problem(nObjectives);
	const std::vector<_TDecision> initial = otl::initial::PopulationUniformReal(random, problem.GetBoundary(), populationSize);
	_TCrossover _crossover(random, 1, problem.GetBoundary(), 20);
	otl::crossover::CoupleCoupleCrossoverAdapter<_TReal, _TDecision, _TRandom &> crossover(_crossover, random);
	_TMutation mutation(random, 1 / (_TReal)problem.GetBoundary().size(), problem.GetBoundary(), 20);
	_TOptimizer optimizer(random, problem, initial, crossover, mutation, 10000);
	BOOST_CHECK(problem.GetNumberOfEvaluations() == populationSize);
	for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
	{
		optimizer();
		BOOST_CHECK_EQUAL(problem.GetNumberOfEvaluations(), (generation + 1) * initial.size());
	}
}
}
