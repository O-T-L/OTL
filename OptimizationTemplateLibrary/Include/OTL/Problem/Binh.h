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
#include <OTL/Utility/WithSpaceBoundary.h>

namespace otl
{
namespace problem
{
template <typename _TReal>
class Binh : public Problem<_TReal, std::vector<_TReal> >, public otl::utility::WithSpaceBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<TReal> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TMinMax TMinMax;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TBoundary TBoundary;

	Binh(void);
	~Binh(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);

private:
	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
Binh<_TReal>::Binh(void)
	: TSuper(2)
	, otl::utility::WithSpaceBoundary<TReal>(TBoundary(2, TMinMax(-5, 10)))
{
}

template <typename _TReal>
Binh<_TReal>::~Binh(void)
{
}

template <typename _TReal>
size_t Binh<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void Binh<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}

template <typename _TReal>
void Binh<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	objective.resize(TSuper::GetNumberOfObjectives());
	assert(decision.size() == 2);
	objective[0] = decision[0] * decision[0] + decision[1] * decision[1];
	const TReal x = decision[0] - 5;
	const TReal y = decision[1] - 5;
	objective[1] = x * x + y * y;
}

template <typename _TReal>
template<class _TArchive> void Binh<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithSpaceBoundary<TReal> >(*this);
}
}
}
