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
@ARTICLE{,
  author = {Rainer Storn and Kenneth Price},
  title = {Differential Evolution – A Simple and Efficient Heuristic for Global
	Optimization over Continuous Spaces},
  journal = {Journal of Global Optimization},
  year = {1997},
  volume = {11},
  pages = {341-359},
  number = {4},
  month = {December},
  doi = {10.1023/A:1008202821328}
}
*/

#pragma once

#include <cassert>
#include <algorithm>
#include <random>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Crossover/XTripleCrossover.h>
#include <OTL/Utility/WithProbability.h>
#include <OTL/Utility/WithBoundary.h>
#include <OTL/Utility/Fix/Truncate.h>

#undef min
#undef max

namespace otl
{
namespace crossover
{
template <typename _TReal, typename _TRandom>
class DifferentialEvolution : public XTripleCrossover<_TReal, std::vector<_TReal> >, public otl::utility::WithRandom<_TRandom>, public otl::utility::WithProbability<_TReal>, public otl::utility::WithBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef _TRandom TRandom;
	typedef std::vector<TReal> TDecision;
	typedef XTripleCrossover<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithBoundary<TReal>::TRange TRange;
	typedef typename otl::utility::WithBoundary<TReal>::TBoundary TBoundary;

	DifferentialEvolution(TRandom random, const TReal probability, const TBoundary &boundary, const TReal scalingFactor = 0.5);
	~DifferentialEvolution(void);
	TReal GetScalingFactor(void) const;

protected:
	void _DoCrossover(const TSolution &parent, const TSolution &parent1, const TSolution &parent2, const TSolution &parent3, TSolution &child);
	void _Crossover(const TDecision &parent, const TDecision &parent1, const TDecision &parent2, const TDecision &parent3, TDecision &child);

private:
	std::uniform_real_distribution<TReal> dist_;
	TReal scalingFactor_;
};

template <typename _TReal, typename _TRandom>
DifferentialEvolution<_TReal, _TRandom>::DifferentialEvolution(TRandom random, const TReal probability, const TBoundary &boundary, const TReal scalingFactor)
	: otl::utility::WithRandom<TRandom>(random)
	, otl::utility::WithProbability<TReal>(probability)
	, otl::utility::WithBoundary<TReal>(boundary)
	, dist_(0, 1)
	, scalingFactor_(scalingFactor)
{
}

template <typename _TReal, typename _TRandom>
DifferentialEvolution<_TReal, _TRandom>::~DifferentialEvolution(void)
{
}

template <typename _TReal, typename _TRandom>
_TReal DifferentialEvolution<_TReal, _TRandom>::GetScalingFactor(void) const
{
	return scalingFactor_;
}

template <typename _TReal, typename _TRandom>
void DifferentialEvolution<_TReal, _TRandom>::_DoCrossover(const TSolution &parent, const TSolution &parent1, const TSolution &parent2, const TSolution &parent3, TSolution &child)
{
	_Crossover(parent.decision_, parent1.decision_, parent2.decision_, parent3.decision_, child.decision_);
}

template <typename _TReal, typename _TRandom>
void DifferentialEvolution<_TReal, _TRandom>::_Crossover(const TDecision &parent, const TDecision &parent1, const TDecision &parent2, const TDecision &parent3, TDecision &child)
{
	assert(!parent.empty());
	assert(parent1.size() == parent.size());
	assert(parent2.size() == parent.size());
	assert(parent3.size() == parent.size());
	std::uniform_int_distribution<size_t> dist(0, this->GetBoundary().size() - 1);
	const size_t randIndex = dist(this->GetRandom());
	child.resize(this->GetBoundary().size());
	for (size_t i = 0; i < this->GetBoundary().size(); ++i)
	{
		if (dist_(this->GetRandom()) < this->GetProbability() || i == randIndex)
			child[i] = otl::utility::fix::Truncate(parent3[i] + scalingFactor_ * (parent1[i] - parent2[i]), this->GetBoundary()[i]);
		else
			child[i] = parent[i];
	}
}
}
}
