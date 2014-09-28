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

#include <OTL/Problem/CommunityDiscovery/Utility.h>
#include "Decode.h"

namespace otl
{
namespace problem
{
namespace community_discovery
{
namespace onl
{
bool IsLegal(const std::vector<std::vector<size_t> > &list, const std::vector<size_t> &decision)
{
	for (size_t i = 0; i < decision.size(); ++i)
	{
		if (decision[i] < 0 || decision[i] >= list[i].size())
			return false;
	}
	return true;
}

template <typename _TIterator>
_TIterator Find(const size_t key, _TIterator begin, _TIterator end)
{
	for (_TIterator i = begin; i != end; ++i)
	{
		const std::set<size_t> &community = *i;
		if (community.find(key) != community.end())
			return i;
	}
	return end;
}

std::list<std::set<size_t> > Decode(const std::vector<std::vector<size_t> > &list, const std::vector<size_t> &decision)
{
	assert(IsLegal(list, decision));
	std::list<std::set<size_t> > communities;
	for (size_t i = 0; i < decision.size(); ++i)
	{
		communities.push_back(std::set<size_t>());
		communities.back().insert(i);
	}
	for (size_t i = 0; i < decision.size(); ++i)
	{
		auto iCurrent = Find(i, communities.begin(), communities.end());
		assert(iCurrent != communities.end());
		assert(0 <= decision[i] && decision[i] < list[i].size());
		auto iAnother = Find(list[i][decision[i]], communities.begin(), communities.end());
		assert(iAnother != communities.end());
		if (iCurrent != iAnother) //merge
		{
			std::set<size_t> &community = *iCurrent;
			const std::set<size_t> &_community = *iAnother;
			community.insert(_community.begin(), _community.end());
			communities.erase(iAnother);
		}
	}
	assert(CheckCommunities(decision.size(), communities.begin(), communities.end()));
	return communities;
}
}
}
}
}
