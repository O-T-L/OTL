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

/*
@Article{,
  Title                    = {网络社区发现的粒子群优化算法},
  Author                   = {黄发良 and 肖南峰},
  Journal                  = {控制理论与应用},
  Year                     = {2011},
  Month                    = {September},
  Number                   = {9},
  Pages                    = {1135-1140},
  Volume                   = {28},
  Doi                      = {10.7641/j.issn.1000-8152.2011.9.CCTA100397}
}
*/

#pragma once

#include <cmath>
#include <OTL/Problem/Problem.h>
#include <OTL/Utility/WithSpaceBoundary.h>
#include <OTL/Problem/CommunityDiscovery/Metric/Metric.h>
#include "Decode.h"

namespace otl
{
namespace problem
{
namespace community_discovery
{
namespace onl
{
template <typename _TReal, typename _TMatrix>
class OrderedNeighborList : public Problem<_TReal, std::vector<size_t> >, public otl::utility::WithSpaceBoundary<size_t>
{
public:
	typedef _TReal TReal;
	typedef _TMatrix TMatrix;
	typedef std::vector<size_t> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithSpaceBoundary<size_t>::TMinMax TMinMax;
	typedef typename otl::utility::WithSpaceBoundary<size_t>::TBoundary TBoundary;
	typedef std::set<size_t> TCommunity;
	typedef std::vector<TCommunity> TCommunities;
	typedef otl::problem::community_discovery::metric::Metric<TMatrix> TMetric;

	OrderedNeighborList(const TMatrix &graph, const std::vector<TMetric *> &metrics);
	~OrderedNeighborList(void);
	const TMatrix &GetGraph(void) const;
	const std::vector<std::vector<size_t> > &GetList(void) const;
	TCommunities Decode(const TDecision &decision) const;

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective) const;

private:
	TMatrix graph_;
	const std::vector<TMetric *> &metrics_;
	std::vector<std::vector<size_t> > list_;

	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal, typename _TMatrix>
OrderedNeighborList<_TReal, _TMatrix>::OrderedNeighborList(const TMatrix &graph, const std::vector<TMetric *> &metrics)
	: TSuper(metrics.size())
	, otl::utility::WithSpaceBoundary<size_t>(TBoundary(graph.size1(), TMinMax(0, graph.size1() - 1)))
	, graph_(graph)
	, metrics_(metrics)
	, list_(MakeOrderedNeighborList(graph))
{
	assert(graph.size1() == graph.size2());
}

template <typename _TReal, typename _TMatrix>
OrderedNeighborList<_TReal, _TMatrix>::~OrderedNeighborList(void)
{
}

template <typename _TReal, typename _TMatrix>
const _TMatrix &OrderedNeighborList<_TReal, _TMatrix>::GetGraph(void) const
{
	return graph_;
}

template <typename _TReal, typename _TMatrix>
const std::vector<std::vector<size_t> > &OrderedNeighborList<_TReal, _TMatrix>::GetList(void) const
{
	return list_;
}

template <typename _TReal, typename _TMatrix>
typename OrderedNeighborList<_TReal, _TMatrix>::TCommunities OrderedNeighborList<_TReal, _TMatrix>::Decode(const TDecision &decision) const
{
	auto _communities = otl::problem::community_discovery::onl::Decode(list_, decision);
	TCommunities communities(_communities.begin(), _communities.end());
	return communities;
}

template <typename _TReal, typename _TMatrix>
size_t OrderedNeighborList<_TReal, _TMatrix>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal, typename _TMatrix>
void OrderedNeighborList<_TReal, _TMatrix>::_DoFix(std::vector<TReal> &objective)
{
	assert(objective.size() == metrics_.size());
	for (size_t i = 0; i < objective.size(); ++i)
	{
		if (metrics_[i]->maximize_)
			objective[i] = -objective[i];
	}
}

template <typename _TReal, typename _TMatrix>
void OrderedNeighborList<_TReal, _TMatrix>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective) const
{
	TCommunities communities = Decode(decision);
	objective.resize(metrics_.size());
	for (size_t i = 0; i < objective.size(); ++i)
	{
		objective[i] = (*metrics_[i])(graph_, communities);
		if (metrics_[i]->maximize_)
			objective[i] = -objective[i];
	}
}

template <typename _TReal, typename _TMatrix>
template<class _TArchive> void OrderedNeighborList<_TReal, _TMatrix>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithSpaceBoundary<TReal> >(*this);
}
}
}
}
}
