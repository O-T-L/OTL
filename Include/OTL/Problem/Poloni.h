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

#pragma once

#include <cmath>
#include <boost/math/constants/constants.hpp>
#include <OTL/Problem/Problem.h>
#include <OTL/Utility/WithBoundary.h>

namespace otl
{
namespace problem
{
template <typename _TReal>
class Poloni : public Problem<_TReal, std::vector<_TReal> >, public otl::utility::WithBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<TReal> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithBoundary<TReal>::TRange TRange;
	typedef typename otl::utility::WithBoundary<TReal>::TBoundary TBoundary;

	Poloni(void);
	~Poloni(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);

private:
	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
Poloni<_TReal>::Poloni(void)
	: TSuper(2)
	, otl::utility::WithBoundary<TReal>(2, TRange(-boost::math::constants::pi<TReal>(), boost::math::constants::pi<TReal>()))
{
}

template <typename _TReal>
Poloni<_TReal>::~Poloni(void)
{
}

template <typename _TReal>
size_t Poloni<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void Poloni<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}

template <typename _TReal>
void Poloni<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	assert(this->IsInside(decision));
	objective.resize(TSuper::GetNumberOfObjectives());
	assert(decision.size() == 2);
	static const TReal A1 = 0.5 * sin((TReal)1) - 2 * cos((TReal)1) + sin((TReal)2) - 1.5 * cos((TReal)2);
	static const TReal A2 = 1.5 * sin((TReal)1) - cos((TReal)1) + 2 * sin((TReal)2) - 0.5 * cos((TReal)2);
	const TReal B1 = 0.5 * sin(decision[0]) - 2 * cos(decision[0]) + sin(decision[1]) - 1.5 * cos(decision[1]);
	const TReal B2 = 1.5 * sin(decision[0]) - cos(decision[0]) + 2 * sin(decision[1]) - 0.5 * cos(decision[1]);
	objective[0] = -(1 + pow(A1 - B1, 2) + pow(A2 - B2, 2));
	objective[1] = -(pow(decision[0] + 3, 2) + pow(decision[1] + 1, 2));
}

template <typename _TReal>
template<class _TArchive> void Poloni<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithBoundary<TReal> >(*this);
}
}
}
