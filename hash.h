#pragma once
// CMSC 341 - Fall 2020 - Project 4
// hash.h

// Templated, hash table implementation.  Each buckets is a heap.  A
// bucket contains objects with the same key values stored as a
// max-heap based on priority.  Collisions are resolved by linear
// probing.

// To work with Heap and HashTable, the template class T must provide
// the following:
//  (a) Default constructor
//  (b) priority function (unsigned priority())
//  (c) key function (string key())
//  (d) overloaded insertion operator

/*
Jacob Enoch
Prof. Kartchner
CMSC341 - Section 90
December 1, 2020
*/

#ifndef _HASH_H
#define _HASH_H

#include <string>
#include "heap.h"

using std::string;

// Hash function typedef.  The hash function must return an 'unsigned
// int' value.  Reduction mod N is the responsiblity of the caller,
// not the hash function.
typedef unsigned (*hash_fn)(string);

class Grader;

template <class T>
class HashTable {

	friend Grader;

public:
	// Constructor.  Requires table size and hash function.
	HashTable(unsigned size, hash_fn hash);

	// Destructor, copy, and assignment
	~HashTable();
	HashTable(const HashTable<T>& ht);
	const HashTable<T>& operator=(const HashTable<T>& ht);

	// In-line functions

	// Table size; set by constructor
	unsigned tableSize() const { return _N; }
	// Number of entries in the table
	unsigned numEntries() const { return _n; }
	// Load factor
	float lambda() const { return ((float)_n) / _N; }

	// Main hash table functions

	// insert returns 'true' if successful; 'false' otherwise
	bool insert(const T& object);
	// getNext retrieves **and removes** the highest priority order of
	// type indicated by key.  It returns 'true' if successful; 'false'
	// otherwise.
	bool getNext(string key, T& obj);

	// Dump the entire hash table
	void dump() const;

	//Heap<T>* _table;


private:
	unsigned _N;       // hash table size
	unsigned _n;       // current number of entries
	hash_fn _hash;     // hash function
	Heap<T>* _table;   // array of heaps

	// ***********************************************
	// Private helper function declarations go here! *
	// ***********************************************

};

// *****************************************
// Templated function definitions go here! *
// *****************************************

template <class T> HashTable<T>::HashTable(unsigned size, hash_fn hash)
{
	_N = size;
	_n = 0;
	_hash = hash;
	_table = new Heap<T>[_N]; //Array of heaps of type T
}

template <class T> HashTable<T>::HashTable(const HashTable<T>& ht) //Copy constructor
{
	_N = ht._N;
	_n = ht._n;
	_hash = ht._hash;
	_table = new Heap<T>[_N];

	for (unsigned int i = 0; i != _N; i++)
	{
		_table[i] = ht._table[i];
	}
}

template <class T> const HashTable<T>& HashTable<T>::operator=(const HashTable<T>& ht)
{
	if (this == &ht)
	{
		return *this;
	}

	delete[] _table;

	_N = ht._N;
	_n = ht._n;
	_hash = ht._hash;
	_table = new Heap<T>[_N];

	for (unsigned int i = 0; i != _N; i++)
	{
		_table[i] = ht._table[i];
	}

	return *this;
}

template <class T> HashTable<T>::~HashTable() //Destructor -> deletes each vector at the index
{
	delete[] _table;
}

template <class T> bool HashTable<T>::insert(const T& objectToInsert) //First search for a bucket with same keys
{                                                                     //if no "like"-bucket is found, insert into next empty bucket; if no empty bucket is found insertion is impossible

	if (_N <= 0) //Size of the table is less than or equal to 0, nothing can be inserted, return false
	{
		cout << "Size of table is 0, insert is not possible." << endl;
		return false;
	}
	
	unsigned insertionIndex = (*_hash)(objectToInsert.key()) % _N;

	if (_table[insertionIndex].empty() == true && _table[insertionIndex].used() == false) //Table location is empty and not used, insertion can proceed without probing
	{
		_table[insertionIndex].insert(objectToInsert);
		_n++;
		return true;
	}


	if(_table[insertionIndex].empty() == false && _table[insertionIndex].readTop().key() == objectToInsert.key()) //Table location is not empty and has same key as the object being inserted, insertion can proceed
	{
		_table[insertionIndex].insert(objectToInsert);
		return true;
	}

	else //Bucket is already occupied and keys don't match or bucket was used before, linear probing is required
	{
		unsigned nextBucket = insertionIndex + 1; //Check the next bucket of the array

		while (nextBucket != insertionIndex) //Search for a bucket with the same key as objectToInsert
		{
			if (nextBucket == _N) //Search cannot exceed size of array, start at beginning
			{
				nextBucket = 0;
			}

			if ((_table[nextBucket].empty() == true) && (_table[nextBucket].used() == false)) //If the next bucket is empty AND NOT USED (uncomment to use _used = true buckets again), insert the object here 
			{
				_table[nextBucket].insert(objectToInsert);
				_n++;
				return true;
			}

			else if ((_table[nextBucket].empty() == false) && (_table[nextBucket].readTop().key() == objectToInsert.key())) //If the next bucket is not empty has the same key, insert object here
			{
				_table[nextBucket].insert(objectToInsert);
				return true;
			}

			nextBucket++; //Move to the next bucket in the array
		}


		//if (_table[insertionIndex].empty() == true) //If the inital hash location is still empty after probing, insert at this location safely
		//{                                           //Was used before the abrubt change regarding whether empty, used buckets were allowed to be reused or not >:(
		//	_table[nextBucket].insert(objectToInsert);
		//	_n++;
		//	return true;
		//}

		cout << "Table is full, no new objects can be inserted." << endl; //Used, empty buckets will not be filled again so the table is technically full at this point

		return false; //Object could not be inserted, return false
	}
}

template <class T> void HashTable<T>::dump() const
{
	for (unsigned int i = 0; i < _N; i++)
	{
		cout << "---Index " << i << " Dump---" << endl;
		_table[i].dump();

		if (_table[i].empty())
		{
			cout << endl;
		}
	}

	return;
}

template <class T> bool HashTable<T>::getNext(string key, T& obj) //Returns and removes the top-most object from a heap in the hash table
{
	if (_N <= 0) //Size of the table is less than or equal to 0, nothing can be removed, return false
	{
		cout << "Size of table is 0, removal is not possible." << endl;
		return false;
	}
	
	unsigned removalIndex = (*_hash)(key) % _N; //Index for where item should be located is calculated

	if (_table[removalIndex].empty() == true) //Where the object SHOULD be is empty, linear probing will be needed in order to be sure that the entire table does not hold the value
	{

	}

	if ((_table[removalIndex].empty() == false) && (_table[removalIndex].readTop().key() == key)) //Item is located at first hash, first item is removed
	{
		obj = _table[removalIndex].readTop();
		_table[removalIndex].removeTop();

		//if (_table[removalIndex].empty() == true) //if the heap is now empty after removal, decrement the total number of occupied buckets
		//{

		//}

		return true;
	}

	else //Object was inserted using linear probing, must linear probe to search for object
	{
		unsigned nextBucket = removalIndex + 1;

		while (nextBucket != removalIndex)
		{
			if (nextBucket == _N) //Search cannot exceed size of array, start at beginning and continue searching
			{
				nextBucket = 0;
				continue;
			}

			if (_table[nextBucket].empty() == true) //Bucket is empty, nothing can be removed; increment nextBucket and start next iteration
			{

			}

			else if (_table[nextBucket].readTop().key().compare(key) == 0) //Key of item to remove matches key of bucket, remove item from 
			{
				obj = _table[nextBucket].readTop();
				_table[nextBucket].removeTop();

				//if (_table[removalIndex].empty() == true) //if the heap is now empty after removal, decrement the total number of occupied buckets
				//{
				//	_n--;
				//}

				return true;
			}

			nextBucket++;
		}
	}

	return false; //Key is not in the hash table, nothing can be removed
}

#endif