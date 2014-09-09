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

#include <cassert>
#include <vector>
#include <random>
#include <OTL/Utility/WithRandom.h>
#include "Crossover.h"
#include "WithCoupleCrossover.h"

namespace otl
{
namespace crossover
{
template <typename _TReal, typename _TDecision, typename _TRandom>
class CoupleCrossoverAdapter : public Crossover<_TReal, _TDecision>, public WithCoupleCrossover<_TReal, _TDecision>, public otl::utility::WithRandom<_TRandom>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef _TRandom TRandom;
	typedef Crossover<TReal, TDecision> TSuper;
	typedef typename TSuper::TSolution TSolution;
	typedef typename WithCoupleCrossover<TReal, TDecision>::TCrossover TCrossover;

	CoupleCrossoverAdapter(TCrossover &crossover, TRandom random);
	~CoupleCrossoverAdapter(void);
	void operator ()(std::vector<const TSolution *> &ancestor, std::vector<TSolution *> &offspring);
	void operator ()(const TSolution &parent1, const TSolution &parent2, TSolution &child);

protected:
	void _DoCrossover(std::vector<const TSolution *> &ancestor, std::vector<TSolution *> &offspring);
};

template <typename _TReal, typename _TDecision, typename _TRandom>
CoupleCrossoverAdapter<_TReal, _TDecision, _TRandom>::CoupleCrossoverAdapter(TCrossover &crossover, TRandom random)
	: WithCoupleCrossover<TReal, TDecision>(crossover)
	, otl::utility::WithRandom<TRandom>(random)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
CoupleCrossoverAdapter<_TReal, _TDecision, _TRandom>::~CoupleCrossoverAdapter(void)
{
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void CoupleCrossoverAdapter<_TReal, _TDecision, _TRandom>::operator ()(std::vector<const TSolution *> &ancestor, std::vector<TSolution *> &offspring)
{
	TSuper::operator ()(ancestor, offspring);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void CoupleCrossoverAdapter<_TReal, _TDecision, _TRandom>::operator ()(const TSolution &parent1, const TSolution &parent2, TSolution &child)
{
	this->GetCrossover()(parent1, parent2, child);
}

template <typename _TReal, typename _TDecision, typename _TRandom>
void CoupleCrossoverAdapter<_TReal, _TDecision, _TRandom>::_DoCrossover(std::vector<const TSolution *> &ancestor, std::vector<TSolution *> &offspring)
{
	for (size_t child = 0; child < offspring.size();)
	{
		std::random_shuffle(ancestor.begin(), ancestor.end(), [this](const size_t n)-> size_t{std::uniform_int_distribution<size_t> dist(0, n - 1);return dist(this->GetRandom());});
		for (size_t parent1 = 0; child < offspring.size() && parent1 < ancestor.size(); ++child, parent1 += 2)
		{
			const size_t parent2 = (parent1 + 1) % ancestor.size();
			(*this)(*ancestor[parent1], *ancestor[parent2], *offspring[child]);
		}
	}
}
}
}
