# Hash_Of_Heaps
A hash table that has heaps (vector implementation) for buckets.
* Each bucket in the hash table represents a type of car being sold by dealerships. The string for each car's name is used to generate an initial hash number. 
* Insertion will check to see if a bucket is empty and if the bucket has been used before. If the bucket is empty and has not been used before then insertion can safely occur. If the bucket has been used and is not empty, the keys of the top-most item and the item being inserted will be checked; if these do not match the item being inserted will need to probe for either a bucket that has the same keys as itself, and if that fails, the next empty and unused bucket. This project specified that empty, used buckets cannot be re-used again but I commented out an implementation that allows this to occur. 
* Removal in the hash table simply attempts to remove the top-most object from a heap with a corresponding key. This will first check the key of the object being requested for removel. If this object is not at the calculated key initially it will then linerally probe through the table until it either finds the value or finds nothing. \
* Heaps are vector-based and use the percolation method to ensure that heap properties are maintained. The implementation is very similar to an array-based tree structure where vector[i-1/2] is a parent node, with vector[2i + 1] and vector[2i + 2] being the left and right child respectively. percolateUp() and percolateDown() are used during insertion/removal respectively to maintain the max heap property.
* Valgrind runs without finding any memory leaks. 
