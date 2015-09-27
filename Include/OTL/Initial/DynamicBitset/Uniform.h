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
#include <limits>
#include <boost/dynamic_bitset.hpp>
#include <random>

namespace otl
{
namespace initial
{
namespace dynamic_bitset
{
template <typename _TRandom>
boost::dynamic_bitset<> Uniform(_TRandom &random, const size_t nBits)
{
	typedef boost::dynamic_bitset<> _TBitset;
	typedef typename _TBitset::block_type _TBlock;
	std::uniform_int_distribution<_TBlock> dist(std::numeric_limits<_TBlock>::min(), std::numeric_limits<_TBlock>::max());
	_TBitset bitset;
	const size_t nBlocks = nBits % bitset.bits_per_block ? nBits / bitset.bits_per_block + 1 : nBits / bitset.bits_per_block;
	for (size_t i = 0; i < nBlocks; ++i)
		bitset.append(dist(random));
	assert(bitset.size() >= nBits);
	bitset.resize(nBits);
	return bitset;
}

template <typename _TRandom>
std::vector<boost::dynamic_bitset<> > BatchUniform(_TRandom &random, const size_t nBits, const size_t size)
{
	typedef boost::dynamic_bitset<> _TBitset;
	std::vector<_TBitset> initial(size);
	for (size_t i = 0; i < initial.size(); ++i)
		initial[i] = Uniform(random, nBits);
	return initial;
}
}
}
}
