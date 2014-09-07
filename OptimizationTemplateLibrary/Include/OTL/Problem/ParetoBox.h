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
#include <OTL/Utility/WithSpaceBoundary.h>

namespace otl
{
namespace problem
{
template <typename _TReal>
class ParetoBox : public Problem<_TReal, std::vector<_TReal> >, public otl::utility::WithSpaceBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<TReal> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TMinMax TMinMax;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TBoundary TBoundary;

	ParetoBox(void);
	~ParetoBox(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);

private:
	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
ParetoBox<_TReal>::ParetoBox(void)
	: TSuper(10)
	, otl::utility::WithSpaceBoundary<TReal>(TBoundary(2, std::make_pair<TReal, TReal>(-10000, 10000)))
{
}

template <typename _TReal>
ParetoBox<_TReal>::~ParetoBox(void)
{
}

template <typename _TReal>
size_t ParetoBox<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void ParetoBox<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}

template <typename _TReal>
void ParetoBox<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	objective.resize(TSuper::GetNumberOfObjectives());
	assert(decision.size() == 2);
	objective[0] = sqrt((decision[0] - 50) * (decision[0] - 50) + (decision[1] - 88) * (decision[1] - 88));
	objective[1] = sqrt((decision[0] - 70) * (decision[0] - 70) + (decision[1] - 80) * (decision[1] - 80));
	objective[2] = sqrt((decision[0] - 83) * (decision[0] - 83) + (decision[1] - 62) * (decision[1] - 62));
	objective[3] = sqrt((decision[0] - 83) * (decision[0] - 83) + (decision[1] - 38) * (decision[1] - 38));
	objective[4] = sqrt((decision[0] - 70) * (decision[0] - 70) + (decision[1] - 20) * (decision[1] - 20));
	objective[5] = sqrt((decision[0] - 50) * (decision[0] - 50) + (decision[1] - 12) * (decision[1] - 12));
	objective[6] = sqrt((decision[0] - 30) * (decision[0] - 30) + (decision[1] - 20) * (decision[1] - 20));
	objective[7] = sqrt((decision[0] - 17) * (decision[0] - 17) + (decision[1] - 38) * (decision[1] - 38));
	objective[8] = sqrt((decision[0] - 17) * (decision[0] - 17) + (decision[1] - 62) * (decision[1] - 62));
	objective[9] = sqrt((decision[0] - 30) * (decision[0] - 30) + (decision[1] - 80) * (decision[1] - 80));
}

template <typename _TReal>
template<class _TArchive> void ParetoBox<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithSpaceBoundary<TReal> >(*this);
}
}
}
