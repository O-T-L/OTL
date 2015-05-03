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
class Shubert : public Problem<_TReal, std::vector<_TReal> >, public otl::utility::WithBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<TReal> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithBoundary<TReal>::TRange TRange;
	typedef typename otl::utility::WithBoundary<TReal>::TBoundary TBoundary;

	Shubert(void);
	~Shubert(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);

private:
	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
Shubert<_TReal>::Shubert(void)
	: TSuper(1)
	, otl::utility::WithBoundary<TReal>(TBoundary(2, std::make_pair<TReal, TReal>(-10, 10)))
{
}

template <typename _TReal>
Shubert<_TReal>::~Shubert(void)
{
}

template <typename _TReal>
size_t Shubert<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void Shubert<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}

template <typename _TReal>
void Shubert<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	assert(this->IsInside(decision));
	objective.resize(TSuper::GetNumberOfObjectives());
	assert(decision.size() == 2);
	TReal part1 = 0;
	for (size_t i = 1; i <= 5; ++i)
		part1 += i * cos((i + 1) * decision[0] + 1);
	TReal part2 = 0;
	for (size_t i = 1; i <= 5; ++i)
		part2 += i * cos((i + 1) * decision[1] + 1);
	objective[0] = part1 * part2 + 0.5 * (pow(decision[0] + 1.42513, 2) + pow(decision[1] + 0.80032, 2));
}

template <typename _TReal>
template<class _TArchive> void Shubert<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithBoundary<TReal> >(*this);
}
}
}
