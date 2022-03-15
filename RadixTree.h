#ifndef _RADIXTREE_H_
#define _RADIXTREE_H_

#include <stack>
#include <string>
#include <iostream>

template <typename ValueType>
class RadixTree {
public:
	RadixTree();
	~RadixTree();
	void insert(std::string key, const ValueType& value);
	ValueType* search(std::string key) const;
private:
	struct TreeNode {
		TreeNode() {
			hasValue = false;
			for (int i = 0; i < 128; i++) {
				edges[i] = nullptr;
			}
		}
		std::string key;
		ValueType value;
		TreeNode* edges[128];
		bool hasValue;
	};
	TreeNode* root = nullptr;
	void insertHere(TreeNode* addMe, TreeNode* n);
	ValueType* searchFromHere(std::string key, TreeNode* n) const;
	std::stack<TreeNode*> nodeStack;
};

template<typename ValueType>
inline RadixTree<ValueType>::RadixTree() : root(nullptr)
{
}
template<typename ValueType>
inline RadixTree<ValueType>::~RadixTree()
{
	while (!nodeStack.empty()) {
		delete nodeStack.top();
		nodeStack.pop();
	}
}

//assumes new node is unique
template<typename ValueType>
inline void RadixTree<ValueType>::insert(std::string key, const ValueType& value)
{
	TreeNode* NodeToBeAdded = new TreeNode();
	

	NodeToBeAdded->key = key;
	NodeToBeAdded->value = value;
	NodeToBeAdded->hasValue = true;
	TreeNode* here = root;
	nodeStack.push(NodeToBeAdded);
	insertHere(NodeToBeAdded, here);
}
template<typename ValueType>
inline ValueType* RadixTree<ValueType>::search(std::string key) const
{
	TreeNode* here = root;
	return searchFromHere(key, here);
}
template<typename ValueType>
inline void RadixTree<ValueType>::insertHere(TreeNode* addMe, TreeNode* n)
{
	//std::cout << addMe->key << std::endl;
	//tree is empty
	if (root == nullptr) {
		root = new TreeNode();
		nodeStack.push(root);
		(*root).edges[addMe->key[0]] = addMe;

		//std::cout << (*root).key << " " << *(root->value.begin()) << std::endl;
		return;
	}
	//if edge bucket is empty
	if ((*n).edges[addMe->key[0]] == nullptr) {
		(*n).edges[addMe->key[0]] = addMe;
		//std::cout << " empty edge" << std::endl;

		//std::cout << (*root).key << " " << *(root->value.begin()) << std::endl;


		return;
	}
	std::string addedKey = addMe->key;
	int matchesUpTo = 0;
	TreeNode* traverse = n->edges[addedKey[0]];
	for (int i = 0; i < traverse->key.size(); i++) {
		if (i == addedKey.size() || addedKey[i] != traverse->key[i]) {
			break;
		}
		if (addedKey[i] == traverse->key[i]) {
			matchesUpTo++;
		}
	}
	//std::cout << traverse->key << " " << addedKey << matchesUpTo << std::endl;
	if (matchesUpTo == traverse->key.size() && matchesUpTo == addedKey.size()) {
		if (traverse->hasValue == false) {
			(*traverse).hasValue = true;
			(*traverse).value = addMe->value;
		}
		return;
	}
	// key we want to add is longer than how far the node's key goes
	if (matchesUpTo == traverse->key.size() && traverse->key.size() < addedKey.size()) {
		//std::cout << "firstie" << std::endl;
		if (traverse->edges[addedKey[matchesUpTo]] == nullptr) {
			//std::cout << "first1" << std::endl;
			TreeNode* splicedNode = new TreeNode();
			nodeStack.push(splicedNode);
			splicedNode->key = addedKey.substr(matchesUpTo);
			splicedNode->value = addMe->value;
			splicedNode->hasValue = true;
			(*traverse).edges[addedKey[matchesUpTo]] = splicedNode;
		}
		//account for non-nullptr
		else if (traverse->edges[addedKey[matchesUpTo]] != nullptr) {
			//std::cout << "first2" << std::endl;
			TreeNode* temp = new TreeNode();
			nodeStack.push(temp);
			temp->key = (*addMe).key = addMe->key.substr(matchesUpTo);
			temp->value = addMe->value;
			temp->hasValue = true;
			insertHere(temp, (traverse));
		}
	}
	//key we want to add is within the node's key
	else if (matchesUpTo < traverse->key.size() && matchesUpTo == addMe->key.size()) {
		//std::cout << "second" << std::endl;
		TreeNode* spliced = new TreeNode();
		nodeStack.push(spliced);
		spliced->key = traverse->key.substr(matchesUpTo);
		for (int i = 0; i < 128; i++) {
			spliced->edges[i] = traverse->edges[i];
		}
		spliced->value = traverse->value;
		spliced->hasValue = true;
		addMe->edges[spliced->key[0]] = spliced;
		//deallocate useless node
		//delete ((*n).edges[addMe->key[0]]);
		((*n).edges[addMe->key[0]]) = nullptr;
		//assign parent's node edge to new node
		n->edges[addMe->key[0]] = addMe;
	}
	//key we want to add diverges from node's key
	else if (matchesUpTo < traverse->key.size() && matchesUpTo < addMe->key.size()) {
		//std::cout << "third" << std::endl;
		TreeNode* central = new TreeNode();
		nodeStack.push(central);
		(*central).key = (*traverse).key.substr(0, matchesUpTo);
		central->hasValue = false;
		TreeNode* spliced = new TreeNode();
		nodeStack.push(spliced);
		spliced->key = traverse->key.substr(matchesUpTo);
		for (int i = 0; i < 128; i++) {
			spliced->edges[i] = traverse->edges[i];
		}
		spliced->value = traverse->value;
		spliced->hasValue = true;
		addMe->key = addMe->key.substr(matchesUpTo);
		//deallocate useless node
		//delete ((*n).edges[traverse->key[0]]);
		((*n).edges[traverse->key[0]]) = nullptr;
		//assign parent's node edge to new node
		central->edges[spliced->key[0]] = spliced;
		central->edges[addMe->key[0]] = addMe;
		(*n).edges[central->key[0]] = central;
	}
}
template<typename ValueType>
inline ValueType* RadixTree<ValueType>::searchFromHere(std::string key, TreeNode* n) const
{
	//std::cout << key << std::endl;
	if (root == nullptr || n == nullptr || (*n).edges[key[0]] == nullptr) {
		return nullptr;
	}
	int matchesUpTo = 0;
	TreeNode* traverse = n->edges[key[0]];
	//std::cout << key << " " << traverse->key << std::endl;
	for (int i = 0; i < traverse->key.size(); i++) {
		if (i == key.size() || key[i] != traverse->key[i]) {
			break;
		}
		if (key[i] == traverse->key[i]) {
			matchesUpTo++;
		}
	}
	if (matchesUpTo == traverse->key.size() && matchesUpTo == key.size()) {
		if (traverse->hasValue == true) {
			return &(traverse->value);
		}
		return nullptr;
	}
	else {
		return searchFromHere(key.substr(matchesUpTo), traverse);
	}
}
#endif



