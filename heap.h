#pragma once
// CMSC 341 - Fall 2020 - Project 4
// heap.h
// Templated, vector-based heap implementation

// To work with Heap, the template class T must provide the following:
//  (a) Default constructor
//  (b) priority function (unsigned priority())
//  (c) overloaded insertion operator
// To work with Heap *and* HashTable, it also needs:
//  (d) key function (string key())

/*
Jacob Enoch
Prof. Kartchner
CMSC341 - Section 90
December 1, 2020
*/

#ifndef _HEAP_H
#define _HEAP_H

#include <iostream>
#include <vector>
#include <exception>
#include <utility>   // swap

using std::vector;
using std::endl;
using std::cout;
using std::swap;
using std::range_error;

// To work with Heap and HashTable, class T must provide the
// following:
//  (a) Default constructor
//  (b) priority function (unsigned priority())
//  (c) key function (string key())
//  (d) overloaded insertion operator

class Grader;

template <class T>
class Heap {

	friend Grader;

public:
	// Constructor
	Heap();

	// Inline functions: size, empty, used 
	unsigned size() const { return _heap.size() - 1; }
	bool empty() const { return _heap.size() <= 1; }
	bool used() const { return _used; }

	// Main heap operations: insert, read, remove
	void insert(const T& object);
	T readTop() const;
	void removeTop();

	// Dump the entire heap
	void dump() const;

	// Root node always has index 1
	static const unsigned ROOT = 1;

private:
	vector<T> _heap;   // Vector-based heap
	bool _used;        // Has the heap ever been used?  Needed for linear probing.


	// *********************************************
	// Private helper function declarations go here!
	// *********************************************

	//void heapifyVector(vector<T>& _heap, int heapSize, int index);

void percolateDown(int index); //Used in the removal of an object -> shifts new root down to spot where heap property is maintained

void percolateUp(int index); //Used in the insertion of an object -> swaps item from bottom of heap up to heap-property-fulfilling position

int findParent(int providedIndex) const; //Finds parent of a given index

int findLeftChild(int providedIndex) const; //Finds left child of a given index

int findRightChild(int providedIndex) const; //Finds right child of a given index

};

// ***************************************
// Templated function definitions go here!
// ***************************************

template<class T> Heap<T>::Heap() //Default constructor for the Heap object
{
	T placeHolder;
	_heap.push_back(placeHolder); //Empty T object used to hold the first index of the vector
	_used = false;
}

template<class T> T Heap<T>::readTop() const //Returns the object at the top of the heap
{
	if (size() == 0) //Heap is empty, throw range error
	{
		throw range_error("Heap is empty, nothing can be returned.");
	}

	else return _heap.at(1);
}

template<class T> void Heap<T>::insert(const T& object) //Inserts item at bottom of heap and shifts item to heap property fulfilling position
{
	if (empty() == true) //If the heap is empty, percolating is not needed, simply push back object
	{
		_heap.push_back(object);
		_used = true;
		return;
	}
	
	_heap.push_back(object);

	int lastObjectIndex = _heap.size() - 1;

	percolateUp(lastObjectIndex);

	_used = true;
}

template<class T> void Heap<T>::removeTop() //Removes the top of the heap and shifts next highest priority item to top
{
	if (size() == 0) //If the size of the heap is 0, remove nothing and throw an error
	{
		throw range_error("Heap is empty, nothing can be removed.");
	}

	if (size() == 1) //Catches case where removal attempts to remove a heap with just one item -> prevents error with percolateDown
	{
		_heap.pop_back();
		return;
	}
	
	_heap[1] = _heap[size()];

	_heap.pop_back();

	percolateDown(ROOT);
}

template<class T> void Heap<T>::dump() const //Outputs the contents of the vector
{
	if (size() != 0) //Makes sure the vector is not "empty" - size 0 
	{
		for (unsigned int i = 1 ; i < _heap.size(); i++)
		{
			cout << _heap.at(i) << endl;
		}

		cout << endl;
		
		return;
	}

	else return;
}

template<class T> int Heap<T>::findLeftChild(int providedIndex) const //Finds the left child for a given index of the vector
{
	return (2 * providedIndex);
}

template<class T> int Heap<T>::findRightChild(int providedIndex) const //Finds the right child for a given index of the vector
{
	return (2 * providedIndex) + 1;
}

template<class T> int Heap<T>::findParent(int providedIndex) const //Finds the parent for a given index of the vector
{
	int parent = (providedIndex) / 2;
	
	if (parent < 1)
	{
		return 1;
	}

	else return parent;
}

template<class T> void Heap<T>::percolateDown(int index) //Used in the removal of an object -> shifts new root down to spot where heap property is maintained
{
	
	int childIndex = findLeftChild(index);
	T temp = _heap[index];

	while (childIndex < _heap.size())
	{
		T maxPriority = temp;
		int maxPriorityIndex = -1;

		for (unsigned int i = 0; (i < 2) && (i + childIndex < _heap.size()); i++)
		{
			if (_heap[i + childIndex].priority() > maxPriority.priority())
			{
				maxPriority = _heap[i + childIndex];
				maxPriorityIndex = i + childIndex;
			}
		}

		if ((maxPriority.priority() == temp.priority()) && (maxPriority.key() == temp.key()))
		{
			return;
		}

		else
		{
			std::swap(_heap[index], _heap[maxPriorityIndex]);
			index = maxPriorityIndex;
			childIndex = findLeftChild(index);
		}
	}
	
	return;
}

template<class T> void Heap<T>::percolateUp(int index) //Used in the insertion of an object -> swaps item from bottom of heap up to heap-property-fulfilling position
{
	int parentIndex = findParent(index);

	if (_heap[index].priority() > _heap[parentIndex].priority())
	{
		std::swap(_heap[index], _heap[parentIndex]);

		parentIndex = findParent(index);
		percolateUp(parentIndex);
	}
}

#endif