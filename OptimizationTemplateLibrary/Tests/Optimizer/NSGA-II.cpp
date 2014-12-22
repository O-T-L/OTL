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
#include <OTL/Problem/ZDT/ZDT5.h>
#include <OTL/Problem/TSP/TSP.h>
#include <OTL/Problem/TSP/MOTSP.h>
#include <OTL/Problem/Knapsack/Knapsack.h>
#include <OTL/Problem/Knapsack/GreedyRepairAdapter.h>
#include <OTL/Initial/UniformReal.h>
#include <OTL/Initial/UniformInteger.h>
#include <OTL/Initial/ShuffleTSP.h>
#include <OTL/Initial/DynamicBitset.h>
#include <OTL/Crossover/SimulatedBinaryCrossover.h>
#include <OTL/Crossover/DifferentialEvolution.h>
#include <OTL/Crossover/SinglePointCrossover.h>
#include <OTL/Crossover/OrderBasedCrossover.h>
#include <OTL/Crossover/BitsetSinglePointCrossover.h>
#include <OTL/Crossover/CoupleCoupleCrossoverAdapter.h>
#include <OTL/Crossover/XTripleCrossoverAdapter.h>
#include <OTL/Mutation/PolynomialMutation.h>
#include <OTL/Mutation/BitwiseMutation.h>
#include <OTL/Mutation/InversionMutation.h>
#include <OTL/Mutation/BitsetBitwiseMutation.h>
#include <OTL/Optimizer/NSGA-II/NSGA-II.h>
#include <OTL/Optimizer/NSGA-II/ConstrainedNSGA-II.h>

template <typename _TRandom, typename _TReal>
std::vector<_TReal> GenerateCity(_TRandom &random, const std::vector<std::pair<_TReal, _TReal> > &boundary)
{
	std::vector<_TReal> city(boundary.size());
	for (size_t i = 0; i < city.size(); ++i)
	{
		std::uniform_real_distribution<_TReal> dist(boundary[i].first, boundary[i].second);
		city[i] = dist(random);
	}
	return city;
}

template <typename _TRandom, typename _TReal>
std::vector<std::vector<_TReal> > GenerateCities(_TRandom &random, const std::vector<std::pair<_TReal, _TReal> > &boundary, const size_t nCities)
{
	typedef std::vector<_TReal> _TPoint;
	std::vector<_TPoint> cities(nCities);
	for (size_t i = 0; i < cities.size(); ++i)
		cities[i] = GenerateCity(random, boundary);
	return cities;
}

template <typename _TRandom, typename _TReal>
std::vector<boost::numeric::ublas::symmetric_matrix<_TReal> > GenerateAdjacencyMatrics(_TRandom &random, const std::vector<std::pair<_TReal, _TReal> > &boundary, const size_t nCities, const size_t nObjectives)
{
	std::vector<boost::numeric::ublas::symmetric_matrix<_TReal> > adjacencyMatrics(nObjectives);
	for (size_t i = 0; i < adjacencyMatrics.size(); ++i)
	{
		auto cities = GenerateCities(random, boundary, nCities);
		adjacencyMatrics[i] = otl::problem::tsp::CalculateAdjacencyMatrix<_TReal>(cities.begin(), cities.end());
	}
	return adjacencyMatrics;
}

template <typename _TRandom, typename _TReal>
boost::numeric::ublas::matrix<_TReal> GenerateMatrix(_TRandom &random, const std::pair<_TReal, _TReal> &minMax, const size_t row, const size_t col)
{
	std::uniform_real_distribution<_TReal> dist(minMax.first, minMax.second);
	boost::numeric::ublas::matrix<_TReal> matrix(row, col);
	for (size_t i = 0; i < matrix.size1(); ++i)
	{
		for (size_t j = 0; j < matrix.size2(); ++j)
			matrix(i, j) = dist(random);
	}
	return matrix;
}

template <typename _TRandom, typename _TReal>
std::vector<_TReal> GenerateVector(_TRandom &random, const std::pair<_TReal, _TReal> &minMax, const size_t size)
{
	void std::uniform_real_distribution;
	std::uniform_real_distribution<_TReal> dist(minMax.first, minMax.second);
	std::vector<_TReal> v(size);
	for (size_t i = 0; i < v.size(); ++i)
		v[i] = dist(random);
	return v;
}

BOOST_AUTO_TEST_CASE(NSGA_II_Real)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::dtlz::DTLZ2<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	_TProblem problem(nObjectives);
	const std::vector<_TDecision> initial = otl::initial::PopulationUniformReal(random, problem.GetBoundary(), populationSize);
	_TCrossover _crossover(random, 1, problem.GetBoundary(), 20);
	otl::crossover::CoupleCoupleCrossoverAdapter<_TReal, _TDecision, _TRandom &> crossover(_crossover, random);
	_TMutation mutation(random, 1 / (_TReal)problem.GetBoundary().size(), problem.GetBoundary(), 20);
	_TOptimizer optimizer(random, problem, initial, crossover, mutation);
	BOOST_CHECK(problem.GetNumberOfEvaluations() == populationSize);
	for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
	{
		optimizer();
		BOOST_CHECK_EQUAL(problem.GetNumberOfEvaluations(), (generation + 1) * initial.size());
	}
}

BOOST_AUTO_TEST_CASE(NSGA_II_DE)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::dtlz::DTLZ2<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::DifferentialEvolution<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	_TProblem problem(nObjectives);
	const std::vector<_TDecision> initial = otl::initial::PopulationUniformReal(random, problem.GetBoundary(), populationSize);
	_TCrossover _crossover(random, 1, problem.GetBoundary());
	otl::crossover::XTripleCrossoverAdapter<_TReal, _TDecision, _TRandom &> crossover(_crossover, random);
	_TMutation mutation(random, 1 / (_TReal)problem.GetBoundary().size(), problem.GetBoundary(), 20);
	_TOptimizer optimizer(random, problem, initial, crossover, mutation);
	BOOST_CHECK(problem.GetNumberOfEvaluations() == populationSize);
	for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
	{
		optimizer();
		BOOST_CHECK_EQUAL(problem.GetNumberOfEvaluations(), (generation + 1) * initial.size());
	}
}

BOOST_AUTO_TEST_CASE(NSGA_II_Integer)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef int _TInteger;
	typedef otl::problem::zdt::ZDT5<_TReal, _TInteger> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SinglePointCrossover<_TReal, _TInteger, _TRandom &> _TCrossover;
	typedef otl::mutation::BitwiseMutation<_TReal, _TInteger, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	_TProblem problem(nObjectives);
	std::vector<std::pair<_TInteger, _TInteger> > boundary(problem.GetDecisionBits().size(), std::pair<_TInteger, _TInteger>(0, (1 << problem.GetDecisionBits()[i]) - 1));
	const std::vector<_TDecision> initial = otl::initial::PopulationUniformInteger(random, boundary, populationSize);
	_TCrossover _crossover(random, 1, problem.GetDecisionBits());
	otl::crossover::CoupleCoupleCrossoverAdapter<_TReal, _TDecision, _TRandom &> crossover(_crossover, random);
	_TMutation mutation(random, 1 / (_TReal)problem.GetDecisionBits().size(), problem.GetDecisionBits());
	_TOptimizer optimizer(random, problem, initial, crossover, mutation);
	BOOST_CHECK(problem.GetNumberOfEvaluations() == populationSize);
	for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
	{
		optimizer();
		BOOST_CHECK_EQUAL(problem.GetNumberOfEvaluations(), (generation + 1) * initial.size());
	}
}

BOOST_AUTO_TEST_CASE(NSGA_II_TSP)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef std::pair<_TReal, _TReal> _TMinMax;
	typedef std::vector<_TMinMax> _TBoundary;
	typedef otl::problem::tsp::TSP<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::OrderBasedCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::InversionMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t populationSize = 100;
	_TRandom random;
	auto cities = GenerateCities(random, _TBoundary(2, _TMinMax(0, 1)), 30);
	auto adjacencyMatrix = otl::problem::tsp::CalculateAdjacencyMatrix<_TReal>(cities.begin(), cities.end());
	_TProblem problem(adjacencyMatrix);
	const std::vector<_TDecision> initial = otl::initial::PopulationShuffleTSP(random, problem.GetNumberOfCities(), populationSize);
	_TCrossover _crossover(random, 1);
	otl::crossover::CoupleCoupleCrossoverAdapter<_TReal, _TDecision, _TRandom &> crossover(_crossover, random);
	_TMutation mutation(random, 1 / (_TReal)problem.GetNumberOfCities());
	_TOptimizer optimizer(random, problem, initial, crossover, mutation);
	BOOST_CHECK(problem.GetNumberOfEvaluations() == populationSize);
	for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
	{
		optimizer();
		BOOST_CHECK_EQUAL(problem.GetNumberOfEvaluations(), (generation + 1) * initial.size());
	}
}

BOOST_AUTO_TEST_CASE(NSGA_II_MOTSP)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef std::pair<_TReal, _TReal> _TMinMax;
	typedef std::vector<_TMinMax> _TBoundary;
	typedef otl::problem::tsp::MOTSP<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::OrderBasedCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::InversionMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	const size_t nCities = 30;
	_TRandom random;
	auto adjacencyMatrics = GenerateAdjacencyMatrics(random, _TBoundary(2, _TMinMax(0, 1)), nCities, nObjectives);
	otl::problem::tsp::CorrelateAdjacencyMatrics(std::vector<_TReal>(adjacencyMatrics.size() - 1, 0), adjacencyMatrics);
	_TProblem problem(adjacencyMatrics);
	const std::vector<_TDecision> initial = otl::initial::PopulationShuffleTSP(random, nCities, populationSize);
	_TCrossover _crossover(random, 1);
	otl::crossover::CoupleCoupleCrossoverAdapter<_TReal, _TDecision, _TRandom &> crossover(_crossover, random);
	_TMutation mutation(random, 1 / (_TReal)nCities);
	_TOptimizer optimizer(random, problem, initial, crossover, mutation);
	BOOST_CHECK(problem.GetNumberOfEvaluations() == populationSize);
	for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
	{
		optimizer();
		BOOST_CHECK_EQUAL(problem.GetNumberOfEvaluations(), (generation + 1) * initial.size());
	}
}

BOOST_AUTO_TEST_CASE(NSGA_II_Bitset)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::knapsack::Knapsack<_TReal> _TKnapsack;
	typedef otl::problem::knapsack::GreedyRepairAdapter<_TKnapsack> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::BitsetSinglePointCrossover<_TReal, _TDecision, _TRandom &> _TCrossover;
	typedef otl::mutation::BitsetBitwiseMutation<_TReal, _TDecision, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	const size_t nPacks = 30;
	_TRandom random;
	auto priceMatrix = GenerateMatrix(random, std::make_pair<_TReal, _TReal>(0, 1), nObjectives, nPacks);
	auto weightMatrix = GenerateMatrix(random, std::make_pair<_TReal, _TReal>(0, 1), nObjectives, nPacks);
	auto capacity = GenerateVector(random, std::make_pair<_TReal, _TReal>(0, 1), nObjectives);
	_TKnapsack knapsack(priceMatrix, weightMatrix, capacity);
	_TProblem problem(knapsack);
	const std::vector<_TDecision> initial = otl::initial::PopulationUniformDynamicBitset(random, nPacks, populationSize);
	_TCrossover _crossover(random, 1);
	otl::crossover::CoupleCoupleCrossoverAdapter<_TReal, _TDecision, _TRandom &> crossover(_crossover, random);
	_TMutation mutation(random, 1 / (_TReal)nPacks);
	_TOptimizer optimizer(random, problem, initial, crossover, mutation);
	for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
		optimizer();
}

BOOST_AUTO_TEST_CASE(ConstrainedNSGA_II_Bitset)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::knapsack::Knapsack<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::BitsetSinglePointCrossover<_TReal, _TDecision, _TRandom &> _TCrossover;
	typedef otl::mutation::BitsetBitwiseMutation<_TReal, _TDecision, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::ConstrainedNSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	const size_t nPacks = 30;
	_TRandom random;
	auto priceMatrix = GenerateMatrix(random, std::make_pair<_TReal, _TReal>(0, 1), nObjectives, nPacks);
	auto weightMatrix = GenerateMatrix(random, std::make_pair<_TReal, _TReal>(0, 1), nObjectives, nPacks);
	auto capacity = GenerateVector(random, std::make_pair<_TReal, _TReal>(0, 1), nObjectives);
	_TProblem problem(priceMatrix, weightMatrix, capacity);
	const std::vector<_TDecision> initial = otl::initial::PopulationUniformDynamicBitset(random, nPacks, populationSize);
	_TCrossover _crossover(random, 1);
	otl::crossover::CoupleCoupleCrossoverAdapter<_TReal, _TDecision, _TRandom &> crossover(_crossover, random);
	_TMutation mutation(random, 1 / (_TReal)nPacks);
	_TOptimizer optimizer(random, problem, initial, crossover, mutation);
	BOOST_CHECK(problem.GetNumberOfEvaluations() == populationSize);
	for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
	{
		optimizer();
		BOOST_CHECK_EQUAL(problem.GetNumberOfEvaluations(), (generation + 1) * initial.size());
	}
}
