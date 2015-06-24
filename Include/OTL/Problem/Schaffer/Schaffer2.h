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
#include <OTL/Problem/Problem.h>
#include <OTL/Utility/WithBoundary.h>

namespace otl
{
namespace problem
{
namespace schaffer
{
template <typename _TReal>
class Schaffer2 : public Problem<_TReal, std::vector<_TReal> >, public otl::utility::WithBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<TReal> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithBoundary<TReal>::TRange TRange;
	typedef typename otl::utility::WithBoundary<TReal>::TBoundary TBoundary;

	Schaffer2(void);
	~Schaffer2(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);

private:
	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
Schaffer2<_TReal>::Schaffer2(void)
	: TSuper(2)
	, otl::utility::WithBoundary<TReal>(TBoundary(1, TRange(-5, 10)))
{
}

template <typename _TReal>
Schaffer2<_TReal>::~Schaffer2(void)
{
}

template <typename _TReal>
size_t Schaffer2<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void Schaffer2<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}

template <typename _TReal>
void Schaffer2<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	assert(this->IsInside(decision));
	objective.resize(TSuper::GetNumberOfObjectives());
	if (decision[0] <= 1)
		objective[0] = -decision[0];
	else if (decision[0] <= 3)
		objective[0] = decision[0] - 2.0;
	else if (decision[0] <= 4)
		objective[0] = 4.0 - decision[0];
	else
		objective[0] = decision[0] - 4.0;
	const TReal x_5 = decision[0] - 5;
	objective[1] = x_5 * x_5;
}

template <typename _TReal>
template<class _TArchive> void Schaffer2<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithBoundary<TReal> >(*this);
}
}
}
}
