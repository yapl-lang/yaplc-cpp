#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>

namespace yaplc { namespace util {
	template<class Source, class Target> void map(const std::vector<Source> &source, std::vector<Target> &target, std::function<Target(Source, unsigned long)> callback) {
		target.clear();
		target.resize(source.size());
		
		unsigned long i = 0;
		auto sit = source.begin();
		auto tit = target.begin();
		
		for (; sit != source.end(); ++i, ++sit, ++tit) {
			*tit = callback(*sit, i);
		}
	}
	
	template<class Key, class Value> void map(const std::map<Key, Value> &source, std::map<Key, Value> &target, std::function<std::pair<Key, Value>(const std::pair<Key, Value> &)> callback) {
		target.clear();
		
		auto sit = source.begin();
		
		for (; sit != source.end(); ++sit) {
			target.insert(callback({(*sit).first, (*sit).second}));
		}
	}
} }
