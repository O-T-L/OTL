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
#include <list>
#include <boost/serialization/base_object.hpp>
#include <OTL/Solution.h>

namespace otl
{
namespace optimizer
{
namespace grea
{
template <typename _TReal, typename _TDecision>
class Individual : public Solution<_TReal, _TDecision>
{
public:
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef Solution<TReal, TDecision> TSuper;

	std::vector<size_t> gridCoordinate_;
	size_t gr_;
	size_t gcd_;
	TReal gcpd_;
	size_t pd_;

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
	gr_ = -1;
	gcd_ = -1;
	gcpd_ = -1;
	pd_ = -1;
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
	archive & gridCoordinate_;
	archive & gcd_;
	archive & gcpd_;
}
}
}
}
