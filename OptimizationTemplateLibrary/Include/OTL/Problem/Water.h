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
class Water : public Problem<_TReal, std::vector<_TReal> >, public otl::utility::WithBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<TReal> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithBoundary<TReal>::TRange TRange;
	typedef typename otl::utility::WithBoundary<TReal>::TBoundary TBoundary;

	Water(void);
	~Water(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective, std::vector<TReal> &inequality);
	static TBoundary _GetDecisionSpaceBoundary(void);

private:
	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
Water<_TReal>::Water(void)
	: TSuper(5)
	, otl::utility::WithBoundary<TReal>(_GetDecisionSpaceBoundary())
{
}

template <typename _TReal>
Water<_TReal>::~Water(void)
{
}

template <typename _TReal>
size_t Water<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_, solution.inequality_);
	return 1;
}

template <typename _TReal>
void Water<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}

template <typename _TReal>
void Water<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective, std::vector<TReal> &inequality)
{
	objective.resize(TSuper::GetNumberOfObjectives());
	assert(decision.size() == 3);
	objective[0] = 106780.37 * (decision[1] + decision[2]) + 61704.67;
	objective[1] = 3000 * decision[0];
	objective[2] = (305700 / pow(0.06*2289, 0.65)) * 2289 * decision[1];
	objective[3] = 250 * 2289 * pow(boost::math::constants::e<TReal>(), -39.75*decision[1] + 9.9*decision[2] + 2.74);
	objective[4] = 25 * (1.39 / (decision[0] * decision[1]) + 4940 * decision[2] - 80);

	objective[0] = objective[0] / 80000;
	objective[1] = objective[1] / 1500;
	objective[2] = objective[2] / 3000000;
	objective[3] = objective[3] / 6000000;
	objective[4] = objective[4] / 8000;

	inequality.resize(7);
	inequality[0] = 1 - (0.00139 / (decision[0] * decision[1]) + 4.94 * decision[2] - 0.08);
	inequality[1] = 0.1 - (0.0000306 / (decision[0] * decision[1]) + 0.1082 * decision[2] - 0.00986);
	inequality[2] = 50000 - (12.307 / (decision[0] * decision[1]) + 49408.24 * decision[2] - 4051.02);
	inequality[3] = 16000 - (2.098 / (decision[0] * decision[1]) + 8046.33 * decision[2] - 696.71);
	inequality[4] = 10000 - (2.138 / (decision[0] * decision[1]) + 7883.39 * decision[2] - 705.04);
	inequality[5] = 2000 - (0.417 / (decision[0] * decision[1]) + 1721.36 * decision[2] - 136.54);
	inequality[6] = 550 - (0.164 / (decision[0] * decision[1]) + 631.13 * decision[2] - 54.48);
}

template <typename _TReal>
typename Water<_TReal>::TBoundary Water<_TReal>::_GetDecisionSpaceBoundary(void)
{
	TBoundary boundary(3);
	boundary[0].first = 0.01;
	boundary[0].second = 0.45;
	for (size_t i = 1; i < boundary.size(); ++i)
	{
		boundary[i].first = 0.01;
		boundary[i].second = 0.10;
	}
	return boundary;
}

template <typename _TReal>
template<class _TArchive> void Water<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithBoundary<TReal> >(*this);
}
}
}
