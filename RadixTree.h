#ifndef _RADIXTREE_H_
#define _RADIXTREE_H_
#include <map>
#include <string>
template <typename ValueType>
class RadixTree {
public:
	RadixTree();
	~RadixTree();
	void insert(std::string key, const ValueType& value);
	ValueType* search(std::string key) const;
private:
	std::map<std::string, ValueType*> m_map;
};
template<typename ValueType>
inline RadixTree<ValueType>::RadixTree()
{
}
template<typename ValueType>
inline RadixTree<ValueType>::~RadixTree()
{
	/*auto iter = m_map.begin();
	while (iter != m_map.end()) {
		delete iter->second;
		++iter;
	}
	*/
}
template<typename ValueType>
inline void RadixTree<ValueType>::insert(std::string key, const ValueType& value)
{
	ValueType* v = new ValueType(value);
	m_map.insert(std::pair<std::string, ValueType*>(key, v));
}

template<typename ValueType>
inline ValueType* RadixTree<ValueType>::search(std::string key) const
{
	if (m_map.count(key)) {
		return m_map.find(key)->second;
	}
	return nullptr;
}
#endif