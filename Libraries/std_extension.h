#pragma once

#include <set>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

namespace string
{
	static std::vector<std::string> splitStr(const std::string& str, const std::string& delim = " ")
	{
		std::vector<std::string> res;
		std::string::size_type pos, lastPos = 0, length = str.length();

		while (lastPos < length + 1)
		{
			pos = str.find_first_of(delim, lastPos);
			if (pos == std::string::npos)			// not found
				pos = length;

			if (pos != lastPos)
				res.push_back(std::string(str.data() + lastPos, pos - lastPos));

			lastPos = pos + 1;
		}

		return res;
	}
}


namespace dict
{
	template<typename Key, typename Val>
	static std::vector<Key> keys(const std::unordered_map<Key, Val>& m)
	{
		std::vector<Key> k(m.size());
		std::transform(m.begin(), m.end(), k.begin(), [](std::pair<Key, Val> pair) { return pair.first; });

		return k;
	}
	
	template<typename Key, typename Val>
	static std::vector<Key> keys(const std::map<Key, Val>& m)
	{
		std::vector<Key> k(m.size());
		std::transform(m.begin(), m.end(), k.begin(), [](std::pair<Key, Val> pair) { return pair.first; });

		return k;
	}

	template<typename Key, typename Val>
	static std::vector<Val> values(const std::unordered_map<Key, Val>& m)
	{
		std::vector<Val> v(m.size());
		std::transform(m.begin(), m.end(), v.begin(), [](std::pair<Key, Val> pair) { return pair.second; });

		return v;
	}

	template<typename Key, typename Val>
	static std::vector<Val> values(const std::map<Key, Val>& m)
	{
		std::vector<Val> v(m.size());
		std::transform(m.begin(), m.end(), v.begin(), [](std::pair<Key, Val> pair) { return pair.second; });

		return v;
	}
}

namespace vector
{
	template<typename T>
	static bool isIn(const std::vector<T>& v, const T& val) {
		return std::find(v.begin(), v.end(), val) != v.end();
	}
}

namespace set
{

}