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

/*
@ARTICLE{,
  author = {Peter J. Bentley and Jonathan P. Wakefield},
  title = {Finding Acceptable Pareto-Optimal Solutions using Multiobjective
	Genetic Algorithms},
  journal = {Soft Computing in Engineering Design and Manufacturing},
  year = {1997},
  volume = {5},
  pages = {231-240},
  address = {London},
  booktitle = {Soft Computing in Engineering Design and Manufacturing},
  publisher = {Springer}
}
*/

#pragma once

#include <algorithm>
#include <iterator>
#include <type_traits>

namespace otl
{
namespace optimizer
{
namespace ar
{
template <typename _TIterator>
void AverageRanking(_TIterator begin, _TIterator end)
{
	for (_TIterator i = begin; i != end; ++i)
		(**i).averageRank_ = 0;
	const size_t nObjectives = (**begin).objective_.size();
	for (size_t dimension = 0; dimension < nObjectives; ++dimension)
	{
		typedef typename std::iterator_traits<_TIterator>::value_type _TPointer;
		std::sort(begin, end
			, [dimension](_TPointer individual1, _TPointer individual2)->bool{return individual1->objective_[dimension] < individual2->objective_[dimension];}
		);
		size_t rank = 0;
		for (_TIterator i = begin; i != end; ++i)
		{
			(**i).averageRank_ += rank;
			++rank;
		}
	}
}
}
}
}
