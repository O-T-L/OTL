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
template <typename _TReal>
class Fonseca : public Problem<_TReal, std::vector<_TReal> >, public otl::utility::WithBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<TReal> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithBoundary<TReal>::TRange TRange;
	typedef typename otl::utility::WithBoundary<TReal>::TBoundary TBoundary;

	Fonseca(void);
	~Fonseca(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);

private:
	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
Fonseca<_TReal>::Fonseca(void)
	: TSuper(3)
	, otl::utility::WithBoundary<TReal>(2, TRange(-4, 4))
{
}

template <typename _TReal>
Fonseca<_TReal>::~Fonseca(void)
{
}

template <typename _TReal>
size_t Fonseca<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void Fonseca<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}

template <typename _TReal>
void Fonseca<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	objective.resize(TSuper::GetNumberOfObjectives());
	assert(decision.size() == 2);
	const TReal sum1 = 0;
	for (size_t i = 0; i < this->GetBoundary().size(); ++i)
		sum1 += pow(decision[i] - (1 / sqrt((TReal)this->GetBoundary().size())), 2);
	objective[0] = 1 - exp(-sum1);

	const TReal sum2 = 0;
	for (size_t i = 0; i < this->GetBoundary().size(); ++i)
		sum2 += pow(decision[i] + (1 / sqrt((TReal)this->GetBoundary().size())), 2);
	objective[1] = 1 - exp(-sum2);
}

template <typename _TReal>
template<class _TArchive> void Fonseca<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithBoundary<TReal> >(*this);
}
}
}
