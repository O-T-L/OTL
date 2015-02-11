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

namespace otl
{
/*!
 * \brief The solution base class
 * \param[in] _TReal The type of objective, must be a real number type
 */
template <typename _TReal>
class Constraint
{
public:
	typedef _TReal TReal;

	std::vector<TReal> inequality_;
	std::vector<TReal> equality_;

	Constraint(void);
	~Constraint(void);
	bool operator ()(void) const;

private:
	template<class _TArchive> void serialize(_TArchive &archive, const unsigned version);

	friend class boost::serialization::access;
};

template <typename _TReal>
Constraint<_TReal>::Constraint(void)
{
}

template <typename _TReal>
Constraint<_TReal>::~Constraint(void)
{
}

/*!
 * \brief Checking violation
 * \return No constraint violation detected
 */
template <typename _TReal>
bool Constraint<_TReal>::operator ()(void) const
{
	for (size_t i = 0; i < inequality_.size(); ++i)
	{
		if (inequality_[i] < 0)
			return false;
	}
	for (size_t i = 0; i < equality_.size(); ++i)
	{
		if (equality_[i] != 0)
			return false;
	}
	return true;
}

template <typename _TReal>
template<class _TArchive> void Constraint<_TReal>::serialize(_TArchive &archive, const unsigned version)
{
	archive & inequality_;
	archive & equality_;
}
}
