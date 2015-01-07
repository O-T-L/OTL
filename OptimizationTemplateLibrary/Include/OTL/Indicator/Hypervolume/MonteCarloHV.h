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
  Title                    = {Multiobjective Evolutionary Algorithms: A Comparative Case Study and the Strength {Pareto} Approach},
  Author                   = {Eckart Zitzler and Lothar Thiele},
  Journal                  = {IEEE Transactions on Evolutionary Computation},
  Year                     = {1999},
  Number                   = {4},
  Pages                    = {257-271},
  Volume                   = {3},
  Doi                      = {10.1109/4235.797969}
}

@ARTICLE{,
  author = {Johannes Bader and Eckart Zitzler},
  title = {{HypE}: An Algorithm for Fast Hypervolume-Based Many-Objective Optimization},
  journal = {Evolutionary Computation},
  year = {2011},
  volume = {19},
  pages = {45-76},
  number = {1},
  doi = {10.1162/EVCO_a_00009}
}
*/

#pragma once

#include <cassert>
#include <vector>
#include <numeric>
#include <functional>
#include <random>
#include <OTL/Indicator/Hypervolume/Hypervolume.h>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Utility/Relation/Pareto.h>
#include <OTL/Utility/Nondominate.h>

namespace otl
{
namespace indicator
{
namespace hypervolume
{
template <typename _TReal, typename _TRandom>
class MonteCarloHV : public otl::indicator::hypervolume::Hypervolume<_TReal>, public otl::utility::WithRandom<_TRandom>
{
public:
	typedef _TReal TReal;
	typedef _TRandom TRandom;
	typedef otl::indicator::hypervolume::Hypervolume<TReal> TSuper;
	typedef typename TSuper::TMetric TMetric;
	typedef typename TSuper::TPoint TPoint;
	typedef typename TSuper::TPointer TPointer;
	typedef typename TSuper::TFront TFront;

	MonteCarloHV(const TPoint &referencePoint, TRandom random, const size_t nSample);
	~MonteCarloHV(void);
	size_t GetSamples(void) const;

protected:
	TMetric _DoEvaluate(const TFront &front);
	size_t _Sampling(const TFront &front, const TPoint &lower, const TPoint &upper, const size_t nSample);
	TPoint _MakePoint(const TPoint &lower, const TPoint &upper);
	bool _IsInside(const TPoint &point, const TFront &front) const;
	static TPoint _CalculateLower(const TFront &front);
	static bool _IsNondominated(const TFront &front);

private:
	const size_t nSample_;
};

template <typename _TReal, typename _TRandom>
MonteCarloHV<_TReal, _TRandom>::MonteCarloHV(const TPoint &referencePoint, TRandom random, const size_t nSample)
	: TSuper(referencePoint)
	, otl::utility::WithRandom<TRandom>(random)
	, nSample_(nSample)
{
}

template <typename _TReal, typename _TRandom>
MonteCarloHV<_TReal, _TRandom>::~MonteCarloHV(void)
{
}

template <typename _TReal, typename _TRandom>
size_t MonteCarloHV<_TReal, _TRandom>::GetSamples(void) const
{
	return nSample_;
}

template <typename _TReal, typename _TRandom>
typename MonteCarloHV<_TReal, _TRandom>::TMetric MonteCarloHV<_TReal, _TRandom>::_DoEvaluate(const TFront &front)
{
	assert(!front.empty());
	assert(TSuper::IsValid(front));
	assert(_IsNondominated(front));
	const TPoint lower = _CalculateLower(front);
	const size_t nInside = _Sampling(front, lower, TSuper::GetReferencePoint(), nSample_);
	return nInside * std::inner_product(TSuper::GetReferencePoint().begin(), TSuper::GetReferencePoint().end(), lower.begin(), (TReal)1, std::multiplies<TReal>(), std::minus<TReal>()) / nSample_;
}

template <typename _TReal, typename _TRandom>
size_t MonteCarloHV<_TReal, _TRandom>::_Sampling(const TFront &front, const TPoint &lower, const TPoint &upper, const size_t nSample)
{
	size_t nInside = 0;
	for (size_t i = 0; i < nSample; ++i)
	{
		const TPoint point = _MakePoint(lower, upper);
		if (_IsInside(point, front))
			++nInside;
	}
	return nInside;
}

template <typename _TReal, typename _TRandom>
typename MonteCarloHV<_TReal, _TRandom>::TPoint MonteCarloHV<_TReal, _TRandom>::_MakePoint(const TPoint &lower, const TPoint &upper)
{
	assert(lower.size() == upper.size());
	TPoint point(lower.size());
	for (size_t i = 0; i < point.size(); ++i)
	{
		std::uniform_real_distribution<TReal> dist(lower[i], upper[i]);
		point[i] = dist(this->GetRandom());
	}
	return point;
}

template <typename _TReal, typename _TRandom>
typename MonteCarloHV<_TReal, _TRandom>::TPoint MonteCarloHV<_TReal, _TRandom>::_CalculateLower(const TFront &front)
{
	assert(front.size() > 0);
	TPoint lower(front[0]->size());
	for (size_t dim = 0; dim < lower.size(); ++dim)
	{
		lower[dim] = (*front[0])[dim];
		for (size_t i = 1; i < front.size(); ++i)
		{
			if ((*front[i])[dim] < lower[dim])
				lower[dim] = (*front[i])[dim];
		}
	}
	return lower;
}

template <typename _TReal, typename _TRandom>
bool MonteCarloHV<_TReal, _TRandom>::_IsInside(const TPoint &point, const TFront &front) const
{
	for (size_t i = 0; i < front.size(); ++i)
	{
		assert(!otl::utility::relation::Dominate(TSuper::GetReferencePoint(), *front[i]));
		if (otl::utility::relation::Dominate(*front[i], point))
			return true;
	}
	return false;
}

template <typename _TReal, typename _TRandom>
bool MonteCarloHV<_TReal, _TRandom>::_IsNondominated(const TFront &front)
{
	std::list<TPointer> _front(front.begin(), front.end());
	otl::utility::ExtractNondominate(_front, [](TPointer pointer1, TPointer pointer2)->bool{return otl::utility::relation::Dominate(*pointer1, *pointer2);});
	return _front.empty();
}
}
}
}
