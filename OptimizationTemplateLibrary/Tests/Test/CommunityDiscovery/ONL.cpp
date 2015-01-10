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

#include <cmath>
#include <vector>
#include <list>
#include <random>
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <OTL/Problem/CommunityDiscovery/Metric/Q.h>
#include <OTL/Problem/CommunityDiscovery/Metric/QLi.h>
#include <OTL/Problem/CommunityDiscovery/Metric/MinMaxCut.h>
#include <OTL/Problem/CommunityDiscovery/Metric/Silhouette.h>
#include <OTL/Problem/CommunityDiscovery/Metric/Ductance.h>
#include <OTL/Problem/CommunityDiscovery/Metric/Expansion.h>
#include <OTL/Problem/CommunityDiscovery/Metric/NCut.h>
#include <OTL/Problem/CommunityDiscovery/ONL/Decode.h>
#include <OTL/Problem/CommunityDiscovery/ONL/ONL.h>
#include <OTL/Initial/UniformInteger.h>
#include <OTL/Crossover/SinglePointCrossover.h>
#include <OTL/Crossover/CoupleCoupleCrossoverAdapter.h>
#include <OTL/Mutation/BitwiseMutation.h>
#include <OTL/Optimizer/NSGA-II/NSGA-II.h>

namespace community_discovery
{
namespace onl
{
BOOST_AUTO_TEST_CASE(TestMetrics)
{
	typedef double _TReal;
	typedef boost::numeric::ublas::symmetric_matrix<_TReal> _TMatrix;
	std::vector<std::vector<_TReal> > _graph = {
		{1, 1, 1, 1, 0, 0, 0, 0},
		{1, 1, 1, 1, 0, 0, 0, 0},
		{1, 1, 1, 1, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 1, 1, 1, 1, 0},
		{0, 0, 0, 1, 1, 1, 1, 1},
		{0, 0, 0, 1, 1, 1, 1, 1},
		{0, 0, 0, 1, 0, 1, 1, 1},
	};
	_TMatrix graph(_graph.size());
	for (size_t i = 0; i < _graph.size(); ++i)
	{
		for (size_t j = 0; j < _graph[i].size(); ++j)
			graph(i, j) = _graph[i][j];
	}
	auto list = otl::problem::community_discovery::onl::MakeOrderedNeighborList(graph);
	std::vector<size_t> decision = {0, 0, 0, 3, 0, 3, 1, 0};
	const std::list<std::set<size_t> > _communities = otl::problem::community_discovery::onl::Decode(list, decision);
	const std::vector<std::set<size_t> > communities(_communities.begin(), _communities.end());
	BOOST_CHECK_EQUAL(communities.size(), 2);
	BOOST_CHECK_EQUAL(communities.front().size(), 3);
	for (size_t i = 0; i < 3; ++i)
		BOOST_CHECK(communities.front().find(i) != communities.front().end());
	BOOST_CHECK_EQUAL(communities.back().size(), 5);
	for (size_t i = 3; i < 8; ++i)
		BOOST_CHECK(communities.back().find(i) != communities.back().end());
	{
		otl::problem::community_discovery::metric::Q<_TReal, _TMatrix> metric;
		BOOST_CHECK_CLOSE(metric(graph, communities), 0.78, 0.1);
	}
	{
		otl::problem::community_discovery::metric::QLi<_TReal, _TMatrix> metric;
		BOOST_CHECK_CLOSE(metric(graph, communities), 4, 0.1);
	}
	{
		otl::problem::community_discovery::metric::MinMaxCut<_TReal, _TMatrix> metric;
		BOOST_CHECK_CLOSE(metric(graph, communities), 0.66666666667, 0.1);
	}
	{
		otl::problem::community_discovery::metric::Silhouette<_TReal, _TMatrix> metric;
		BOOST_CHECK_CLOSE(metric(graph, communities), 0.35, 0.1);
	}
	{
		otl::problem::community_discovery::metric::Ductance<_TReal, _TMatrix> metric;
		BOOST_CHECK_CLOSE(metric(graph, communities), 0.27692308, 0.1);
	}
	{
		otl::problem::community_discovery::metric::Expansion<_TReal, _TMatrix> metric;
		BOOST_CHECK_CLOSE(metric(graph, communities), 1.6, 0.1);
	}
	{
			otl::problem::community_discovery::metric::NCut<_TReal, _TMatrix> metric;
			BOOST_CHECK_CLOSE(metric(graph, communities), 0.44685772, 0.1);
		}
}

BOOST_AUTO_TEST_CASE(TestONL)
{
	typedef std::mt19937 _TRandom;
	typedef double _TReal;
	typedef boost::numeric::ublas::symmetric_matrix<_TReal> _TMatrix;
	typedef otl::problem::community_discovery::onl::ONL<_TReal, _TMatrix, _TRandom &> _TProblem;
	typedef _TProblem::TDecision _TDecision;
	typedef otl::crossover::SinglePointCrossover<_TReal, size_t, _TRandom &> _TCrossover;
	typedef otl::mutation::BitwiseMutation<_TReal, size_t, _TRandom &> _TMutation;
	typedef otl::optimizer::nsga_ii::NSGA_II<_TReal, _TDecision, _TRandom &> _TOptimizer;
	const size_t populationSize = 100;
	_TRandom random;
	std::vector<std::vector<_TReal> > _graph = {
		{1, 1, 1, 1, 0, 0, 0, 0},
		{1, 1, 1, 1, 0, 0, 0, 0},
		{1, 1, 1, 1, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 1, 1, 1, 1, 0},
		{0, 0, 0, 1, 1, 1, 1, 1},
		{0, 0, 0, 1, 1, 1, 1, 1},
		{0, 0, 0, 1, 0, 1, 1, 1},
	};
	_TMatrix graph(_graph.size());
	for (size_t i = 0; i < _graph.size(); ++i)
	{
		for (size_t j = 0; j < _graph[i].size(); ++j)
			graph(i, j) = _graph[i][j];
	}
	otl::problem::community_discovery::metric::Q<_TReal, _TMatrix> metric1;
	otl::problem::community_discovery::metric::QLi<_TReal, _TMatrix> metric2;
	std::vector<_TProblem::TMetric *> metrics = {&metric1, &metric2};
	_TProblem problem(graph, metrics, random);
	const std::vector<_TDecision> initial = otl::initial::PopulationUniformInteger(random, problem.GetBoundary(), populationSize);
	const std::vector<size_t> decisionBits(graph.size1(), ceil(log2((_TReal)graph.size1())));
	_TCrossover _crossover(random, 1, decisionBits);
	otl::crossover::CoupleCoupleCrossoverAdapter<_TReal, _TDecision, _TRandom &> crossover(_crossover, random);
	_TMutation mutation(random, 1 / (_TReal)problem.GetBoundary().size(), decisionBits);
	_TOptimizer optimizer(random, problem, initial, crossover, mutation);
	BOOST_CHECK(problem.GetNumberOfEvaluations() == populationSize);
	for (size_t generation = 1; problem.GetNumberOfEvaluations() < 30000; ++generation)
	{
		optimizer();
		BOOST_CHECK_EQUAL(problem.GetNumberOfEvaluations(), (generation + 1) * initial.size());
	}
}
}
}
