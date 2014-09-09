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
#include <OTL/Mutation/PolynomialMutation.h>
#include <OTL/Optimizer/MOEA-D/Weight/AdjustWeight.h>
#include <OTL/Optimizer/MOEA-D/Weight/NormalizeWeight.h>
#include <OTL/Optimizer/MOEA-D/CoupleCouple/MOEA-D_WeightedSum.h>
#include <OTL/Optimizer/MOEA-D/CoupleCouple/MOEA-D_Tchebycheff.h>
#include <OTL/Optimizer/MOEA-D/CoupleCouple/NormMOEA-D_Tchebycheff.h>
#include <OTL/Optimizer/MOEA-D/CoupleCouple/MOEA-D_PBI.h>
#include <OTL/Utility/Weight/NormalBoundaryIntersection.h>

BOOST_AUTO_TEST_CASE(MOEA_D_WeightedSum)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef otl::problem::dtlz::DTLZ2<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::moea_d::couple_couple::MOEA_D_WeightedSum<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	_TRandom random;
	_TProblem problem(nObjectives);
	auto _weightVectors = otl::utility::weight::NormalBoundaryIntersection<_TReal>(nObjectives, 23);
	std::vector<std::vector<_TReal> > weightVectors(_weightVectors.begin(), _weightVectors.end());
	const std::vector<_TDecision> initial = otl::initial::PopulationUniformReal(random, problem.GetBoundary(), weightVectors.size());
	_TCrossover crossover(random, 1, problem.GetBoundary(), 20);
	_TMutation mutation(random, 1 / (_TReal)problem.GetBoundary().size(), problem.GetBoundary(), 20);
	_TOptimizer optimizer(random, problem, initial, crossover, mutation, weightVectors, initial.size() / 10);
	BOOST_CHECK(problem.GetNumberOfEvaluations() == initial.size());
	for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
	{
		optimizer();
		BOOST_CHECK_EQUAL(problem.GetNumberOfEvaluations(), (1 + generation * 2) * initial.size());
	}
}

BOOST_AUTO_TEST_CASE(MOEA_D_Tchebycheff)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef double _TReal;
	typedef otl::problem::dtlz::DTLZ2<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::moea_d::couple_couple::MOEA_D_Tchebycheff<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	_TRandom random;
	_TProblem problem(nObjectives);
	auto _weightVectors = otl::utility::weight::NormalBoundaryIntersection<_TReal>(nObjectives, 23);
	std::vector<std::vector<_TReal> > weightVectors(_weightVectors.begin(), _weightVectors.end());
	for (size_t i = 0; i < weightVectors.size(); ++i)
		otl::optimizer::moea_d::weight::AdjustWeight(weightVectors[i], 0.00001);
	const std::vector<_TDecision> initial = otl::initial::PopulationUniformReal(random, problem.GetBoundary(), weightVectors.size());
	_TCrossover crossover(random, 1, problem.GetBoundary(), 20);
	_TMutation mutation(random, 1 / (_TReal)problem.GetBoundary().size(), problem.GetBoundary(), 20);
	_TOptimizer optimizer(random, problem, initial, crossover, mutation, weightVectors, initial.size() / 10);
	BOOST_CHECK(problem.GetNumberOfEvaluations() == initial.size());
	for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
	{
		optimizer();
		BOOST_CHECK_EQUAL(problem.GetNumberOfEvaluations(), (1 + generation * 2) * initial.size());
	}
}

BOOST_AUTO_TEST_CASE(MOEA_D_PBI)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef double _TReal;
	typedef otl::problem::dtlz::DTLZ2<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::moea_d::couple_couple::MOEA_D_PBI<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	_TRandom random;
	_TProblem problem(nObjectives);
	auto _weightVectors = otl::utility::weight::NormalBoundaryIntersection<_TReal>(nObjectives, 23);
	std::vector<std::vector<_TReal> > weightVectors(_weightVectors.begin(), _weightVectors.end());
	for (size_t i = 0; i < weightVectors.size(); ++i)
		otl::optimizer::moea_d::weight::NormalizeWeight(weightVectors[i]);
	const std::vector<_TDecision> initial = otl::initial::PopulationUniformReal(random, problem.GetBoundary(), weightVectors.size());
	_TCrossover crossover(random, 1, problem.GetBoundary(), 20);
	_TMutation mutation(random, 1 / (_TReal)problem.GetBoundary().size(), problem.GetBoundary(), 20);
	_TOptimizer optimizer(random, problem, initial, crossover, mutation, weightVectors, initial.size() / 10, 5);
	BOOST_CHECK(problem.GetNumberOfEvaluations() == initial.size());
	for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
	{
		optimizer();
		BOOST_CHECK_EQUAL(problem.GetNumberOfEvaluations(), (1 + generation * 2) * initial.size());
	}
}

BOOST_AUTO_TEST_CASE(NormMOEA_D_Tchebycheff)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef double _TReal;
	typedef otl::problem::dtlz::DTLZ2<_TReal> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SimulatedBinaryCrossover<_TReal, _TRandom &> _TCrossover;
	typedef otl::mutation::PolynomialMutation<_TReal, _TRandom &> _TMutation;
	typedef otl::optimizer::moea_d::couple_couple::NormMOEA_D_Tchebycheff<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t nObjectives = 3;
	_TRandom random;
	_TProblem problem(nObjectives);
	auto _weightVectors = otl::utility::weight::NormalBoundaryIntersection<_TReal>(nObjectives, 23);
	std::vector<std::vector<_TReal> > weightVectors(_weightVectors.begin(), _weightVectors.end());
	for (size_t i = 0; i < weightVectors.size(); ++i)
		otl::optimizer::moea_d::weight::AdjustWeight(weightVectors[i], 0.0001);
	const std::vector<_TDecision> initial = otl::initial::PopulationUniformReal(random, problem.GetBoundary(), weightVectors.size());
	_TCrossover crossover(random, 1, problem.GetBoundary(), 20);
	_TMutation mutation(random, 1 / (_TReal)problem.GetBoundary().size(), problem.GetBoundary(), 20);
	_TOptimizer optimizer(random, problem, initial, crossover, mutation, weightVectors, initial.size() / 10);
	BOOST_CHECK(problem.GetNumberOfEvaluations() == initial.size());
	for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
	{
		optimizer();
		BOOST_CHECK_EQUAL(problem.GetNumberOfEvaluations(), (1 + generation * 2) * initial.size());
	}
}
