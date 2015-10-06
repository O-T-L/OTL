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
#include <boost/numeric/ublas/vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <OTL/Solution.h>

namespace otl
{
namespace optimizer
{
namespace isnps
{
template <typename _TReal, typename _TDecision>
class Individual : public Solution<_TReal, _TDecision>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef Solution<TReal, TDecision> TSuper;

	boost::numeric::ublas::vector<TReal> direction_;
	TReal convergence_;
	std::vector<TReal> distribution_;
	size_t exclude_;

	Individual(void);
	~Individual(void);

private:
	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal, typename _TDecision>
Individual<_TReal, _TDecision>::Individual(void)
{
#ifndef NDEBUG
	convergence_ = std::numeric_limits<TReal>::infinity();
	exclude_ = -1;
#endif
}

template <typename _TReal, typename _TDecision>
Individual<_TReal, _TDecision>::~Individual(void)
{
}

template <typename _TReal, typename _TDecision>
template<class _TArchive> void Individual<_TReal, _TDecision>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & convergence_;
	archive & distribution_;
}
}
}
}
