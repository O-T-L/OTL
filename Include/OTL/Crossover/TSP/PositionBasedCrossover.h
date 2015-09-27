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

#include <algorithm>
#include <random>
#include <OTL/Utility/WithRandom.h>
#include <OTL/Utility/WithProbability.h>
#include <OTL/Crossover/CoupleCoupleCrossover.h>

namespace otl
{
namespace crossover
{
namespace tsp
{
template <typename _TReal, typename _TRandom>
class PositionBasedCrossover : public CoupleCoupleCrossover<_TReal, std::vector<size_t> >, public otl::utility::WithRandom<_TRandom>, public otl::utility::WithProbability<_TReal>
{
public:
	typedef _TReal TReal;
	typedef _TRandom TRandom;
	typedef std::vector<size_t> TDecision;
	typedef CoupleCoupleCrossover<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;

	PositionBasedCrossover(TRandom random, const TReal probability);
	~PositionBasedCrossover(void);

protected:
	void _DoCrossover(const TSolution &parent1, const TSolution &parent2, TSolution &child1, TSolution &child2);
	void _Crossover(const TDecision &parent1, const TDecision &parent2, TDecision &child1, TDecision &child2);

private:
	std::uniform_real_distribution<TReal> dist_;
};

template <typename _TReal, typename _TRandom>
PositionBasedCrossover<_TReal, _TRandom>::PositionBasedCrossover(TRandom random, const TReal probability)
	: otl::utility::WithRandom<TRandom>(random)
	, otl::utility::WithProbability<TReal>(probability)
	, dist_(0, 1)
{
}

template <typename _TReal, typename _TRandom>
PositionBasedCrossover<_TReal, _TRandom>::~PositionBasedCrossover(void)
{
}

template <typename _TReal, typename _TRandom>
void PositionBasedCrossover<_TReal, _TRandom>::_DoCrossover(const TSolution &parent1, const TSolution &parent2, TSolution &child1, TSolution &child2)
{
	_Crossover(parent1.decision_, parent2.decision_, child1.decision_, child2.decision_);
}

template <typename _TReal, typename _TRandom>
void PositionBasedCrossover<_TReal, _TRandom>::_Crossover(const TDecision &parent1, const TDecision &parent2, TDecision &child1, TDecision &child2)
{
	if (dist_(this->GetRandom()) < this->GetProbability() || parent2 == parent1)
	{
		child1 = parent1;
		child2 = parent2;
		return;
	}
	child1.assign(parent1.size(), -1);
	child2.assign(parent2.size(), -1);

	std::vector<size_t> chosenCitiesIndex;
	for (size_t i = std::uniform_int_distribution<size_t>(0, parent2.size() - 2)(this->GetRandom()); i < parent2.size(); i += std::uniform_int_distribution<size_t>(1, parent2.size() - i)(this->GetRandom()))
		chosenCitiesIndex.push_back(i);

	for (size_t i = 0; i < chosenCitiesIndex.size(); ++i)
	{
		child1[chosenCitiesIndex[i]] = parent1[chosenCitiesIndex[i]];
		child2[chosenCitiesIndex[i]] = parent2[chosenCitiesIndex[i]];
	}

	for (size_t i = 0, city1 = 0, city2 = 0; i < parent2.size(); ++i)
	{
		while(city1 < child1.size() && child1[city1] != -1)
			++city1;
		if (std::find(child1.begin(), child1.end(), parent2[i]) == child1.end())
			child1[city1] = parent2[i];

		while(city2 < child2.size() && child2[city2] != -1)
			++city2;
		if (std::find(child2.begin(), child2.end(), parent1[i]) == child2.end())
			child2[city2] = parent1[i];
	}
}
}
}
}
