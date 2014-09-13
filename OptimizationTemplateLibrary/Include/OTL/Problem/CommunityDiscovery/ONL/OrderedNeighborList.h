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
#include <boost/numeric/ublas/symmetric.hpp>
#include <OTL/Problem/Problem.h>
#include <OTL/Utility/WithSpaceBoundary.h>
#include "Decode.h"

namespace otl
{
namespace problem
{
namespace community_discovery
{
namespace onl
{
template <typename _TReal>
class OrderedNeighborList : public Problem<_TReal, std::vector<size_t> >, public otl::utility::WithSpaceBoundary<size_t>
{
public:
	typedef _TReal TReal;
	typedef std::vector<size_t> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithSpaceBoundary<size_t>::TMinMax TMinMax;
	typedef typename otl::utility::WithSpaceBoundary<size_t>::TBoundary TBoundary;
	typedef boost::numeric::ublas::symmetric_matrix<TReal> TMatrix;
	typedef std::set<size_t> TCommunity;
	typedef std::vector<TCommunity> TCommunities;
	typedef TReal (*TMetric)(const TMatrix &, const TCommunities &);

	OrderedNeighborList(const TMatrix &graph, const std::vector<TMetric> &functions, const std::vector<bool> &maximize);
	~OrderedNeighborList(void);
	const TMatrix &GetGraph(void) const;
	const std::vector<std::vector<size_t> > &GetList(void) const;

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective) const;

private:
	TMatrix graph_;
	std::vector<TMetric> functions_;
	std::vector<bool> maximize_;
	std::vector<std::vector<size_t> > list_;

	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
OrderedNeighborList<_TReal>::OrderedNeighborList(const TMatrix &graph, const std::vector<TMetric> &functions, const std::vector<bool> &maximize)
	: TSuper(functions.size())
	, otl::utility::WithSpaceBoundary<size_t>(TBoundary(graph.size1(), TMinMax(0, graph.size1() - 1)))
	, graph_(graph)
	, functions_(functions)
	, maximize_(maximize)
	, list_(MakeOrderedNeighborList(graph))
{
	assert(graph.size1() == graph.size2());
	assert(functions.size() == maximize.size());
}

template <typename _TReal>
OrderedNeighborList<_TReal>::~OrderedNeighborList(void)
{
}

template <typename _TReal>
const typename OrderedNeighborList<_TReal>::TMatrix &OrderedNeighborList<_TReal>::GetGraph(void) const
{
	return graph_;
}

template <typename _TReal>
const std::vector<std::vector<size_t> > &OrderedNeighborList<_TReal>::GetList(void) const
{
	return list_;
}

template <typename _TReal>
size_t OrderedNeighborList<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void OrderedNeighborList<_TReal>::_DoFix(std::vector<TReal> &objective)
{
	assert(objective.size() == functions_.size());
	for (size_t i = 0; i < objective.size(); ++i)
	{
		if (maximize_[i])
			objective[i] = -objective[i];
	}
}

template <typename _TReal>
void OrderedNeighborList<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective) const
{
	auto _communities = Decode(list_, decision);
	TCommunities communities(_communities.begin(), _communities.end());
	objective.resize(functions_.size());
	for (size_t i = 0; i < objective.size(); ++i)
	{
		objective[i] = functions_[i](graph_, communities);
		if (maximize_[i])
			objective[i] = -objective[i];
	}
}

template <typename _TReal>
template<class _TArchive> void OrderedNeighborList<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithSpaceBoundary<TReal> >(*this);
}
}
}
}
}
