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

#include <limits>
#include <cassert>
#include <vector>
#include <boost/dynamic_bitset.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <OTL/Problem/Problem.h>

namespace otl
{
namespace problem
{
namespace knapsack
{
template <typename _TReal, typename _TBitset, typename _TVector>
_TReal TotalValue(const _TBitset &decision, const _TVector &valueList)
{
	assert(decision.size() == valueList.size());
	_TReal totalWeight = 0;
	for (size_t i = 0; i < decision.size(); ++i)
	{
		if (decision[i])
			totalWeight += valueList(i);
	}
	return totalWeight;
}

template <typename _TReal>
class Knapsack : public Problem<_TReal, boost::dynamic_bitset<> >
{
public:
	typedef _TReal TReal;
	typedef boost::dynamic_bitset<> TDecision;
	typedef Problem<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef boost::numeric::ublas::matrix<TReal> TMatrix;

	Knapsack(const TMatrix &price, const TMatrix &weight, const std::vector<TReal> &capacity);
	~Knapsack(void);
	const TMatrix &GetPrice(void) const;
	const TMatrix &GetWeight(void) const;
	const std::vector<TReal> &GetCapacity(void) const;
	size_t GetPacks(void) const;

protected:
	size_t _DoEvaluate(TSolution &solution);
	void _DoFix(std::vector<TReal> &objective);
	void _Evaluate(const TDecision &decision, std::vector<TReal> &objective, std::vector<TReal> &inequality);

private:
	const TMatrix &price_;
	const TMatrix &weight_;
	const std::vector<TReal> &capacity_;
};

template <typename _TReal>
Knapsack<_TReal>::Knapsack(const TMatrix &price, const TMatrix &weight, const std::vector<TReal> &capacity)
	: TSuper(capacity.size())
	, price_(price)
	, weight_(weight)
	, capacity_(capacity)
{
	assert(capacity.size() == price.size1());
	assert(capacity.size() == weight.size1());
	assert(price.size2() == weight.size2());
}

template <typename _TReal>
Knapsack<_TReal>::~Knapsack(void)
{
}

template <typename _TReal>
const boost::numeric::ublas::matrix<_TReal> &Knapsack<_TReal>::GetPrice(void) const
{
	return price_;
}

template <typename _TReal>
const boost::numeric::ublas::matrix<_TReal> &Knapsack<_TReal>::GetWeight(void) const
{
	return weight_;
}

template <typename _TReal>
const std::vector<_TReal> &Knapsack<_TReal>::GetCapacity(void) const
{
	return capacity_;
}

template <typename _TReal>
size_t Knapsack<_TReal>::GetPacks(void) const
{
	assert(price_.size2() == weight_.size2());
	return price_.size2();
}

template <typename _TReal>
size_t Knapsack<_TReal>::_DoEvaluate(TSolution &solution)
{
	_Evaluate(solution.decision_, solution.objective_, solution.inequality_);
	return 1;
}

template <typename _TReal>
void Knapsack<_TReal>::_DoFix(std::vector<TReal> &objective)
{
	for (size_t i = 0; i < objective.size(); ++i)
		objective[i] = -objective[i];
}

template <typename _TReal>
void Knapsack<_TReal>::_Evaluate(const TDecision &decision, std::vector<TReal> &objective, std::vector<TReal> &inequality)
{
	assert(TSuper::GetNumberOfObjectives() == capacity_.size());
	assert(TSuper::GetNumberOfObjectives() == price_.size1());
	assert(decision.size() == price_.size2());
	objective.resize(TSuper::GetNumberOfObjectives());
	inequality.resize(TSuper::GetNumberOfObjectives());
	for (size_t i = 0; i < objective.size(); ++i)
	{
		boost::numeric::ublas::matrix_row<const TMatrix> _price(price_, i);
		objective[i] = -TotalValue<TReal>(decision, _price);
		boost::numeric::ublas::matrix_row<const TMatrix> _weight(weight_, i);
		inequality[i] = capacity_[i] - TotalValue<TReal>(decision, _weight);
	}
}
}
}
}
