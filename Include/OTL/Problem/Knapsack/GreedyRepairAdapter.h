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
#include "Knapsack.h"

namespace otl
{
namespace problem
{
namespace knapsack
{
template <typename _TReal>
std::vector<_TReal> CalculateProfits(const boost::numeric::ublas::matrix<_TReal> &priceMatrix, const boost::numeric::ublas::matrix<_TReal> &weightMatrix)
{
	assert(priceMatrix.size1() == weightMatrix.size1());
	assert(priceMatrix.size2() == weightMatrix.size2());
	std::vector<_TReal> profits(priceMatrix.size2());
	for (size_t i = 0; i < priceMatrix.size2(); ++i)
	{
		profits[i] = priceMatrix(0, i) / weightMatrix(0, i);
		for (size_t j = 1; j < priceMatrix.size1(); ++j)
		{
			const _TReal temp = priceMatrix(j, i) / weightMatrix(j, i);
			if (temp > profits[i])
				profits[i] = temp;
		}
	}
	return profits;
}

template <typename _TProblem>
class GreedyRepairAdapter : public _TProblem
{
public:
	typedef _TProblem TProblem;
	typedef TProblem TSuper;
	typedef typename TSuper::TReal TReal;
	typedef typename TSuper::TDecision TDecision;
	typedef typename TSuper::TSolution TSolution;

	GreedyRepairAdapter(const TProblem &problem);
	~GreedyRepairAdapter(void);
	size_t LocateWorstPack(const TDecision &decision) const;

protected:
	size_t _DoEvaluate(TSolution &solution);

private:
	std::vector<TReal> profits_;
};

template <typename _TProblem>
GreedyRepairAdapter<_TProblem>::GreedyRepairAdapter(const TProblem &problem)
	: TSuper(problem)
{
	profits_ = CalculateProfits(TSuper::GetPrice(), TSuper::GetWeight());
}

template <typename _TProblem>
GreedyRepairAdapter<_TProblem>::~GreedyRepairAdapter(void)
{
}

template <typename _TProblem>
size_t GreedyRepairAdapter<_TProblem>::LocateWorstPack(const TDecision &decision) const
{
	TReal WorstProfit = std::numeric_limits<TReal>::max();
	size_t nPacks = -1;
	for (size_t i = 0; i < decision.size(); ++i)
	{
		if (decision[i] && profits_[i] < WorstProfit)
		{
			WorstProfit = profits_[i];
			nPacks = i;
		}
	}
	assert(0 <= nPacks && nPacks < decision.size());
	return nPacks;
}

template <typename _TProblem>
size_t GreedyRepairAdapter<_TProblem>::_DoEvaluate(TSolution &solution)
{
	size_t evaluation = 0;
	for (evaluation += TSuper::_DoEvaluate(solution); !solution(); evaluation += TSuper::_DoEvaluate(solution))
	{
		const size_t nPacks = LocateWorstPack(solution.decision_);
		assert(solution.decision_[nPacks]);
		solution.decision_[nPacks] = 0;
	}
	return evaluation;
}
}
}
}
