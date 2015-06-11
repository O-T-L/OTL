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
namespace viennet
{
template <typename _TReal>
class Viennet2 : public Problem<_TReal, std::vector<_TReal> >, public otl::utility::WithBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<TReal> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithBoundary<TReal>::TRange TRange;
	typedef typename otl::utility::WithBoundary<TReal>::TBoundary TBoundary;

	Viennet2(void);
	~Viennet2(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);

private:
	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
Viennet2<_TReal>::Viennet2(void)
	: TSuper(3)
	, otl::utility::WithBoundary<TReal>(TBoundary(2, TRange(-4, 4)))
{
}

template <typename _TReal>
Viennet2<_TReal>::~Viennet2(void)
{
}

template <typename _TReal>
size_t Viennet2<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void Viennet2<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}

template <typename _TReal>
void Viennet2<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	assert(this->IsInside(decision));
	objective.resize(TSuper::GetNumberOfObjectives());
	assert(decision.size() == 2);
	objective[0] = (decision[0] - 2) * (decision[0] - 2) / 2 + (decision[1] + 1) * (decision[1] + 1) / 13 + 3;
	objective[1] = (decision[0] + decision[1] - 3) * (decision[0] + decision[1] - 3) / 36 + (-decision[0] + decision[1] + 2) * (-decision[0] + decision[1] + 2) / 8 - 17;
	objective[2] = (decision[0] + 2 * decision[1] - 1) * (decision[0] + 2 * decision[1] - 1) / 175 + (2 * decision[1] - decision[0]) * (2 * decision[1] - decision[0]) / 17 - 13;
}

template <typename _TReal>
template<class _TArchive> void Viennet2<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithBoundary<TReal> >(*this);
}
}
}
}
