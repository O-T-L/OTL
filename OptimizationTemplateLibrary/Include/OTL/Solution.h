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

#include <vector>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/base_object.hpp>
#include "Constraint.h"

namespace otl
{
/*!
 * \brief The solution base class
 * \param[in] _TReal The type of objective value, must be a real number type
 * \param[in] _TDecision The type of the decision, can be any data structure
 */
template <typename _TReal, typename _TDecision>
class Solution : public Constraint<_TReal>
{
public:
	//! \cond Doxygen_Suppress
	typedef _TReal TReal;
	typedef _TDecision TDecision;
	typedef Constraint<TReal> TSuper;
	//! \endcond

	std::vector<TReal> objective_; //!< The objective vector
	TDecision decision_; //!< The decision variable

	Solution(void);
	~Solution(void);
	bool operator ==(const Solution<TReal, TDecision> &solution) const;

private:
	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal, typename _TDecision>
Solution<_TReal, _TDecision>::Solution(void)
{
}

template <typename _TReal, typename _TDecision>
Solution<_TReal, _TDecision>::~Solution(void)
{
}

template <typename _TReal, typename _TDecision>
bool Solution<_TReal, _TDecision>::operator ==(const Solution<TReal, TDecision> &solution) const
{
	return decision_ == solution.decision_;
}

template <typename _TReal, typename _TDecision>
template<class _TArchive> void Solution<_TReal, _TDecision>::serialize(_TArchive &archive, const unsigned version)
{
	archive & boost::serialization::base_object<TSuper>(*this);
	archive & objective_;
	archive & decision_;
}
}
