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
@Conference{,
  Title                    = {A Test Problem for Visual Investigation of High-dimensional Multi-objective Search},
  Author                   = {Miqing Li and Shengxiang Yang and Xiaohui Liu},
  Booktitle                = {IEEE Congress on Evolutionary Computation},
  Year                     = {2014},
  Address                  = {Beijing, China},
  Month                    = {6-11 July},
  Pages                    = {2140-2147},
}
*/

#pragma once

#include <cmath>
#include <OTL/Problem/Problem.h>
#include <OTL/Utility/WithBoundary.h>

namespace otl
{
namespace problem
{
template <typename _TReal>
class Rectangle : public Problem<_TReal, std::vector<_TReal> >, public otl::utility::WithBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<TReal> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithBoundary<TReal>::TRange TRange;
	typedef typename otl::utility::WithBoundary<TReal>::TBoundary TBoundary;

	Rectangle(const TBoundary &boundary, const TBoundary &boundaryOptimal);
	~Rectangle(void);
	const TBoundary &GetBoundaryOptimal(void) const;

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);
	bool _Check(void) const;

private:
	const TBoundary boundaryOptimal_;

	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
Rectangle<_TReal>::Rectangle(const TBoundary &boundary, const TBoundary &boundaryOptimal)
	: TSuper(boundary.size() * 2)
	, otl::utility::WithBoundary<TReal>(boundary)
	, boundaryOptimal_(boundaryOptimal)
{
	assert(boundary.size() == boundaryOptimal.size());
	assert(otl::utility::WithBoundary<TReal>::Validate(boundaryOptimal));
	assert(_Check());
}

template <typename _TReal>
Rectangle<_TReal>::~Rectangle(void)
{
}

template <typename _TReal>
const typename Rectangle<_TReal>::TBoundary &Rectangle<_TReal>::GetBoundaryOptimal(void) const
{
	return boundaryOptimal_;
}

template <typename _TReal>
size_t Rectangle<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void Rectangle<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}

template <typename _TReal>
void Rectangle<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	assert(this->IsInside(decision));
	objective.resize(TSuper::GetNumberOfObjectives());
	assert(decision.size() == this->GetBoundary().size());
	for (size_t i = 0; i < decision.size(); ++i)
	{
		const size_t j = i * 2;
		objective[j] = fabs(decision[i] - boundaryOptimal_[i].first);
		objective[j + 1] = fabs(decision[i] - boundaryOptimal_[i].second);
	}
}

template <typename _TReal>
bool Rectangle<_TReal>::_Check(void) const
{
	assert(this->GetBoundary().size() == boundaryOptimal_.size());
	for (size_t i = 0; i < this->GetBoundary().size(); ++i)
	{
		const auto &range = this->GetBoundary()[i];
		const auto &rangeOptimal = boundaryOptimal_[i];
		if (rangeOptimal.first < range.first)
			return false;
		if (range.second < rangeOptimal.second)
			return false;
	}
	return true;
}

template <typename _TReal>
template<class _TArchive> void Rectangle<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithBoundary<TReal> >(*this);
}
}
}
