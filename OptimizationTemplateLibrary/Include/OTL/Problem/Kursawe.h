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
class Kursawe : public Problem<_TReal, std::vector<_TReal> >, public otl::utility::WithSpaceBoundary<_TReal>
{
public:
	typedef _TReal TReal;
	typedef std::vector<TReal> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TMinMax TMinMax;
	typedef typename otl::utility::WithSpaceBoundary<TReal>::TBoundary TBoundary;

	Kursawe(const size_t nDecisions);
	~Kursawe(void);

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective);

private:
	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
Kursawe<_TReal>::Kursawe(const size_t nDecisions)
	: TSuper(2)
	, otl::utility::WithSpaceBoundary<TReal>(nDecisions, TMinMax(-5, 5))
{
}

template <typename _TReal>
Kursawe<_TReal>::~Kursawe(void)
{
}

template <typename _TReal>
size_t Kursawe<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_);
	return 1;
}

template <typename _TReal>
void Kursawe<_TReal>::_DoFix(std::vector<TReal> &objective)
{
}

template <typename _TReal>
void Kursawe<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective)
{
	objective.resize(TSuper::GetNumberOfObjectives());
	objective[0] = 0;
	for (size_t i = 0; i < this->GetBoundary().size() - 1; ++i)
	{
		const TReal x1 = decision[i] * decision[i];
		const TReal x2 = decision[i + 1] * decision[i + 1];
		objective[0] += -10 * exp(-0.2 * sqrt(x1 + x2));
	}

	objective[1] = 0;
	for (size_t i = 0; i < this->GetBoundary().size(); ++i)
		objective[1] += pow(std::abs(decision[i]), 0.8) + 5 * sin(pow(decision[i], 3));
}

template <typename _TReal>
template<class _TArchive> void Kursawe<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & boost::serialization::base_object<otl::utility::WithSpaceBoundary<TReal> >(*this);
}
}
}
