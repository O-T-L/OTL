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
#include <OTL/Initial/ShuffleTSP.h>
#include <OTL/Crossover/OrderBasedCrossover.h>
#include <OTL/Crossover/PartiallyMappedCrossover.h>
#include <OTL/Crossover/PositionBasedCrossover.h>
#include <OTL/Crossover/CoupleCoupleCrossoverAdapter.h>
#include <OTL/Mutation/DisplacementMutation.h>
#include <OTL/Mutation/ExchangeMutation.h>
#include <OTL/Mutation/InsertionMutation.h>
#include <OTL/Mutation/InversionMutation.h>
#include <OTL/Mutation/SpreadMutation.h>

template <typename _TCrossover, typename _TSolution>
std::vector<_TSolution> MakeCrossover(_TCrossover &crossover, const std::vector<_TSolution> &ancestor, const size_t nOffspring)
{
	std::vector<const _TSolution *> _ancestor(ancestor.size());
	for (size_t i = 0; i < _ancestor.size(); ++i)
		_ancestor[i] = &ancestor[i];
	std::vector<_TSolution> offspring(nOffspring);
	std::vector<_TSolution *> _offspring(offspring.size());
	for (size_t i = 0; i < _offspring.size(); ++i)
		_offspring[i] = &offspring[i];
	crossover(_ancestor, _offspring);
	return offspring;
}

template <typename _TCrossover, typename _TSolution>
std::vector<_TSolution> MakeCrossover(_TCrossover &crossover, const std::vector<_TSolution> &ancestor)
{
	return MakeCrossover(crossover, ancestor, ancestor.size());
}

template <typename _TReal, typename _TDecision>
void TestSolution(const otl::Solution<_TReal, _TDecision> &solution)
{
	_TDecision test = solution.decision_;
	std::sort(test.begin(), test.end());
	for (size_t i = 0; i < test.size(); ++i)
		BOOST_CHECK(test[i] == i);
}

template <typename _TIterator>
void TestPopulation(_TIterator iBegin, _TIterator iEnd)
{
	for (_TIterator i = iBegin; i != iEnd; ++i)
		TestSolution(*i);
}

BOOST_AUTO_TEST_CASE(TSP)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef std::vector<size_t> _TDecision;
	typedef otl::Solution<_TReal, _TDecision> _TSolution;
	_TRandom random;
	std::vector<_TDecision> population = otl::initial::PopulationShuffleTSP(random, 30, 100);
	for (size_t i = 0; i < population.size(); ++i)
	{
		_TDecision test = population[i];
		std::sort(test.begin(), test.end());
		for (size_t i = 0; i < test.size(); ++i)
			BOOST_CHECK(test[i] == i);
	}
	std::vector<_TSolution> ancestor;
	std::vector<_TSolution> offspring(population.size());
	for (size_t i = 0; i < offspring.size(); ++i)
		offspring[i].decision_ = population[i];
	{
		ancestor = offspring;
		otl::crossover::OrderBasedCrossover<_TReal, _TRandom &> _crossover(random, 1);
		otl::crossover::CoupleCoupleCrossoverAdapter<_TReal, _TDecision, _TRandom &> crossover(_crossover, random);
		offspring = MakeCrossover(crossover, ancestor);
		TestPopulation(offspring.begin(), offspring.end());
	}
	{
		ancestor = offspring;
		otl::crossover::PartiallyMappedCrossover<_TReal, _TRandom &> _crossover(random, 1);
		otl::crossover::CoupleCoupleCrossoverAdapter<_TReal, _TDecision, _TRandom &> crossover(_crossover, random);
		offspring = MakeCrossover(crossover, ancestor);
		TestPopulation(offspring.begin(), offspring.end());
	}
	{
		ancestor = offspring;
		otl::crossover::PositionBasedCrossover<_TReal, _TRandom &> _crossover(random, 1);
		otl::crossover::CoupleCoupleCrossoverAdapter<_TReal, _TDecision, _TRandom &> crossover(_crossover, random);
		offspring = MakeCrossover(crossover, ancestor);
		TestPopulation(offspring.begin(), offspring.end());
	}
	{
		otl::mutation::DisplacementMutation<_TReal, _TRandom &> mutation(random, 1);
		for (size_t i = 0; i < offspring.size(); ++i)
		{
			mutation(offspring[i]);
			TestSolution(offspring[i]);
		}
	}
	{
		otl::mutation::ExchangeMutation<_TReal, _TRandom &> mutation(random, 1);
		for (size_t i = 0; i < offspring.size(); ++i)
		{
			mutation(offspring[i]);
			TestSolution(offspring[i]);
		}
	}
	{
		otl::mutation::InsertionMutation<_TReal, _TRandom &> mutation(random, 1);
		for (size_t i = 0; i < offspring.size(); ++i)
		{
			mutation(offspring[i]);
			TestSolution(offspring[i]);
		}
	}
	{
		otl::mutation::InversionMutation<_TReal, _TRandom &> mutation(random, 1);
		for (size_t i = 0; i < offspring.size(); ++i)
		{
			mutation(offspring[i]);
			TestSolution(offspring[i]);
		}
	}
	{
		otl::mutation::SpreadMutation<_TReal, _TRandom &> mutation(random, 1);
		for (size_t i = 0; i < offspring.size(); ++i)
		{
			mutation(offspring[i]);
			TestSolution(offspring[i]);
		}
	}
}
