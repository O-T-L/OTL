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
#include <OTL/Problem/UF/UF1.h>
#include <OTL/Problem/UF/UF2.h>
#include <OTL/Problem/UF/UF3.h>
#include <OTL/Problem/UF/UF4.h>
#include <OTL/Problem/UF/UF5.h>
#include <OTL/Problem/UF/UF6.h>
#include <OTL/Problem/UF/UF7.h>
#include <OTL/Problem/UF/UF8.h>
#include <OTL/Problem/UF/UF9.h>
#include <OTL/Problem/UF/UF10.h>
#include <OTL/Initial/UniformReal.h>
#include <OTL/Crossover/SimulatedBinaryCrossover.h>
#include <OTL/Crossover/CoupleCoupleCrossoverAdapter.h>
#include <OTL/Mutation/PolynomialMutation.h>
#include <OTL/Optimizer/NSGA-II/NSGA-II.h>

BOOST_AUTO_TEST_CASE(UF1)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::uf::UF1<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	_TProblem problem;
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

BOOST_AUTO_TEST_CASE(UF2)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::uf::UF2<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	_TProblem problem;
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

BOOST_AUTO_TEST_CASE(UF3)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::uf::UF3<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	_TProblem problem;
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

BOOST_AUTO_TEST_CASE(UF4)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::uf::UF4<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	_TProblem problem;
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

BOOST_AUTO_TEST_CASE(UF5)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::uf::UF5<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	_TProblem problem;
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

BOOST_AUTO_TEST_CASE(UF6)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::uf::UF6<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	_TProblem problem;
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

BOOST_AUTO_TEST_CASE(UF7)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::uf::UF7<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	_TProblem problem;
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

BOOST_AUTO_TEST_CASE(UF8)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::uf::UF8<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	_TProblem problem;
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

BOOST_AUTO_TEST_CASE(UF9)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::uf::UF9<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	_TProblem problem;
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

BOOST_AUTO_TEST_CASE(UF10)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::uf::UF10<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	const size_t populationSize = 100;
	_TRandom random;
	_TProblem problem;
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
