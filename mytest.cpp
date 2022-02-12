#include <iostream>
#include "hash.h"
#include "car.h"
#include <iomanip>
#include <chrono>

/*
Jacob Enoch
Prof. Kartchner
CMSC341 - Section 90
December 1, 2020
*/

unsigned cyclic_hash16(string key)
{
    unsigned usize = 16;
    unsigned s = 5; // shift by 5
    unsigned h = 0;
    for (int i = 0; i < key.length(); i++) {
        h = ((h << s) | (h >> (usize - s)));
        h += key[i];
        h = h & 0xffff;
    }
    return h;
}

unsigned simple_hash(string key) //Used for testing hashing of objects without collisions
{
    int keyValue = std::stoi(key);
    
    return keyValue;
}

int main()
{
    /*-------------------------------------Heap Testing-------------------------------------*/
    Heap<Car> newHeap;

    cout << "---------------------------HEAP TESTING---------------------------" << endl;

    cout << endl << "---Heap after insertion of elements---" << endl;

//Inserts various objects into the max heap
    newHeap.insert(Car("x101", 12, "mega car"));
    newHeap.insert(Car("x101", 4, "shack of cars"));
    newHeap.insert(Car("challenger", 10, "mega car"));
    newHeap.insert(Car("challenger", 5, "car world"));
    newHeap.insert(Car("stratos", 7, "car world"));
    newHeap.insert(Car("stratos", 15, "super car"));
    newHeap.insert(Car("stratos", 8, "shack of cars"));
    newHeap.insert(Car("challenger", 3, "car joint"));
    newHeap.insert(Car("gt500", 3, "shack of cars"));
    newHeap.insert(Car("miura", 28, "mega car"));
    newHeap.insert(Car("gt500", 11, "super car"));

    newHeap.dump();

    cout << endl << "---Heap after removal of top element---" << endl;

//Removes an object from the top of the heap
    newHeap.removeTop();

    newHeap.dump();

    cout << endl << "---Heap after removal of top element---" << endl;

//Removes an object from the top of the heap
    newHeap.removeTop();

    newHeap.dump();

    cout << endl << "---Testing Exception Throwing for readTop() and removeTop() on an empty heap---" << endl;

//Emptying of the heap

    while (newHeap.size() != 0)
    {
        newHeap.removeTop();
    }

    newHeap.dump();

//Testing exception throwing for an empty heap

    try
    {
        newHeap.readTop();
    }

    catch (range_error& error)
    {
        cout << error.what() << endl;
    }

    try
    {
        newHeap.removeTop();
    }

    catch (range_error& error)
    {
        cout << error.what() << endl;
    }

    cout << endl << "---Inserting Duplicate Objects into Heap---" << endl;
    Car testCar("Bricklin SV1", 25, "Gullwing Cars");

    for (int i = 0; i < 20; i++)
    {
        newHeap.insert(testCar);
    }

    newHeap.dump();


/*-------------------------------------Hash Table Testing-------------------------------------*/
    cout << endl << "---------------------------HASH TABLE TESTING---------------------------" << endl;

//Used to help with checking correct insertion into hashed index
    cout << endl << "------INITIAL HASH VALUES------" << endl;
    cout << "gt500 Inital Hash: " << cyclic_hash16("gt500") % 10 << endl; // = 2
    cout << "Miata Initial Hash: " << cyclic_hash16("Miata") % 10 << endl; // = 9
    cout << "XJ220 Initial Hash: " << cyclic_hash16("XJ220") % 10 << endl; // = 3
    cout << "350z Initial Hash: " << cyclic_hash16("350z") % 10 << endl << endl; // = 8


    HashTable<Car> ht(10, cyclic_hash16); //gt340 = collision w/ gt500

//This test checks to see if insertion works without collisions -> each key has a unique bucket, no probing
    cout << "------NORMAL INSERT TEST------" << endl;
    ht.insert(Car("gt500", 25, "shack of cars"));
    ht.insert(Car("gt500", 11, "super car"));
    ht.insert(Car("Miata", 11, "super car"));
    ht.insert(Car("Miata", 15, "super car"));
    ht.insert(Car("XJ220", 28, "shack of cars"));
    ht.insert(Car("350z", 13, "super car"));
    ht.insert(Car("350z", 51, "big cars"));
    ht.dump();

//Used to help with checking correct insertion into hashed index
    cout << endl << "------INITIAL HASH VALUES------" << endl;
    cout << "gt500 Inital Hash: " << cyclic_hash16("gt500") % 10 << endl; // = 2
    cout << "Miata Initial Hash: " << cyclic_hash16("Miata") % 10 << endl; // = 9
    cout << "XJ220 Initial Hash: " << cyclic_hash16("XJ220") % 10 << endl; // = 3
    cout << "310z Initial Hash: " << cyclic_hash16("310z") % 10 << endl; // = 2
    cout << "350z Initial Hash: " << cyclic_hash16("350z") % 10 << endl << endl; // = 8

//This test checks whether insertion correctly handles collisons 
    cout << endl << "------COLLISION INSERTION TEST------" << endl; //Tests inserts for objects that collide during insertion
    ht.insert(Car("gt350", 54, "shack of cars"));
    ht.insert(Car("gt350", 3, "shack of cars"));
    ht.insert(Car("310z", 23, "shack of cars"));
    ht.insert(Car("310z", 45, "shack of cars"));
    ht.insert(Car("310z", 24, "shack of cars"));

    ht.dump();

//This test will fill the hash table up to capacity in order to fully test how collisions are handled within the table//
    cout << endl << "------FILLING HASH TABLE UP COMPLETELY------" << endl;
    ht.insert(Car("Supra", 2, "super car"));
    ht.insert(Car("Supra", 9, "super car"));
    ht.insert(Car("MR2", 7, "big cars"));
    ht.insert(Car("MR2", 15, "big cars"));
    ht.insert(Car("MR2", 25, "big cars"));
    ht.insert(Car("MR2", 1, "big cars"));
    ht.insert(Car("Viper", 17, "super car"));
    ht.insert(Car("Viper", 25, "super car"));
    ht.insert(Car("Viper", 11, "super car"));
    ht.insert(Car("AE86", 11, "Drift Masters"));
    ht.dump();

//This test will determine whether a brand new object will be inserted into the table if the table is full//
    cout << endl << "------ATTEMPTING TO ADD TO A FULL HASH TABLE------" << endl;
    cout << "Adding BMW E30..." << endl << endl;
    ht.insert(Car("BMW E30", 11, "super car"));
    cout << endl << endl;
    ht.dump();


//This test simply removes various objects from the table, testing how removal collisions are handled as well as normal removals where probing was not required//
    cout << endl << "------REMOVING OBJECTS FROM HASH TABLE------" << endl;
    cout << "Removing: 2 gt500 (Index 2), 1 350z (Index 8), 1 MR2 (Index 0), 1 Viper (Index 1), 1 AE86 (Index 6)" << endl << endl;
    Car d;
    ht.getNext("gt500", d); //Tests removal for an object without collision
    ht.getNext("gt500", d);
    ht.getNext("350z", d);
    ht.getNext("MR2", d);
    ht.getNext("Viper", d);
    ht.getNext("AE86", d);
    ht.dump();

//This test determines if objects insert correctly when their initial hash location is empty but a heap with identical keys already exists
    cout << endl << "------Inserting Objects that would normally hash directly to empty spot but have existing items in table already------" << endl;
    cout << "These objects (Except for the 310z, priority 54) should not be inserted since empty and used buckets cannot be re-used." << endl << endl;
    /* cout << endl << "310z should hash at index 2, but since there are already 310z objects it will instead hash to the heap with existing objects" << endl;
    cout << endl << "gt340 should insert at index 6 since index 2 is occupied and index 6 is the next empty heap in the table" << endl;
    cout << endl << "Hellcat should insert at index 3 since it is the only empty index following the removal of the last XJ220 object" << endl;
    cout << endl << "Cobra should insert at index 2 since there are no existing Cobra heaps, and index 2 is the only empty heap left" << endl << endl;*/

    ht.insert(Car("310z", 54, "Ahh"));
    ht.insert(Car("gt340", 32, "Beep Beep"));
    ht.insert(Car("gt340", 12, "Car Noises"));
    ht.insert(Car("Cobra", 53, "Big Bill Hell's"));
    ht.getNext("XJ220", d);
    ht.insert(Car("Hellcat", 22, "Speen"));
    cout << endl;
    ht.dump();

//Tests how the table reacts to insertions when the size is 0
    cout << endl << "------Inserting objects into a Hash Table of Size 0------" << endl;
    HashTable<Car> ht5(0, cyclic_hash16);
    ht5.insert(Car("Camaro", 15, "Ugly Cars"));
    ht5.dump();

//Tests how the table reacts to insertions when the size is 0
    cout << endl << "------Removing objects from a Hash Table of Size 0------" << endl;
    ht5.getNext("Miata", d);
    ht5.dump();

//--------------------Copy Constructor and Assignment Operator Testing--------------------//

    cout << endl << "---------Copy Constructor and Assignment Operator Testing---------" << endl << endl;
    HashTable<Car> ht2(ht);
    HashTable<Car> ht3(20, cyclic_hash16);
    ht3 = ht;

//This test checks that the copy constructor correctly copied the values from ht into ht2//
    cout << "------(Copy Constructor) Hash Table ht2 Dump------" << endl;
    ht2.dump();

//This test removes some objects from the original table ht//
    cout << "------(Copy Constructor) Hash Table ht Dump Following Removal of Objects------" << endl;
    ht.getNext("310z", d);
    ht.getNext("Cobra", d);
    ht.getNext("Hellcat", d);
    ht.getNext("350z", d);
    ht.dump();

//This test makes sure that the copy constructor makes a deep copy of the host object (ht in this case)
    cout << "------(Copy Constructor) Hash Table ht2 Dump Following Removal of Objects from ht------" << endl;
    ht2.dump();

//This test makes sure that the Assignment Operator correctly copies the values from ht into ht3
    cout << endl << "------(Assignment Operator) Hash Table ht3 Dump------" << endl;
    ht3.dump();

//Dumps Hash Table ht following the removal of more objects from the table
    cout << "------(Assignment Operator) Hash Table ht Dump Following Removal of Objects (Completely remove Supra and Viper heaps)------" << endl;
    ht.getNext("Viper", d);
    ht.getNext("Viper", d);
    ht.getNext("Supra", d);
    ht.getNext("Supra", d);
    ht.dump();

//This test makes sure that the assignment makes a deep copy of the host object (ht in this case)
    cout << "------(Assignment Operator) Hash Table ht3 Dump Following Removal of Objects from ht------" << endl;
    ht3.dump();

//--------------------------------------------------------Heap Runtime Testing--------------------------------------------------------//
    
    cout << "------------Testing Runtimes For Heap Insertion/Removal------------" << endl << endl;

    Heap<Car> test1;

    auto time1 = std::chrono::high_resolution_clock::now(); //Takes time before testing occurs
    auto time2 = std::chrono::high_resolution_clock::now(); //Takes time after testing occurs
    int priority; //Used for randomly generated priorities
    string test = "testing"; //Used for test objects

//--------------------------------------------------------Inserting 1000 cars into a heap--------------------------------------------------------//
    
    time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 1000; i++)
    {
        priority = (rand() % 1000) + 1;
        test1.insert(Car(test, priority, test));
    }

    time2 = std::chrono::high_resolution_clock::now();

    auto totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00; //Total time in seconds = time1 - time2 / 1000 

    cout << std::setw(20) << std::left << "Time to insert 1,000 Cars: " << std::setw(15) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds 

//--------------------------------------------------------Removing 1000 items from a heap--------------------------------------------------------//

    time1 = std::chrono::high_resolution_clock::now();

    while (test1.size() != 0)
    {
        test1.removeTop();
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to remove 1,000 Cars: " << std::setw(15) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

//--------------------------------------------------------Inserting 2000 cars into a heap--------------------------------------------------------//
    
    time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 2000; i++)
    {
        priority = (rand() % 1000) + 1;
        test1.insert(Car(test, priority, test));
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to insert 2,000 Cars: " << std::setw(15) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

//--------------------------------------------------------Removing 2000 cars into a heap--------------------------------------------------------//

    time1 = std::chrono::high_resolution_clock::now();

    while (test1.size() != 0)
    {
        test1.removeTop();
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to remove 2,000 Cars: " << std::setw(15) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

//--------------------------------------------------------Inserting 4000 cars into a heap--------------------------------------------------------//
    
    time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 4000; i++)
    {
        priority = (rand() % 1000) + 1;
        test1.insert(Car(test, priority, test));
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to insert 4,000 Cars: " << std::setw(15) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

//--------------------------------------------------------Removing 4000 cars into a heap--------------------------------------------------------//
    
    time1 = std::chrono::high_resolution_clock::now();

    while (test1.size() != 0)
    {
        test1.removeTop();
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to remove 4,000 Cars: " << std::setw(15) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

//--------------------------------------------------------Inserting 8000 cars into a heap--------------------------------------------------------//

    time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 8000; i++)
    {
        priority = (rand() % 1000) + 1;
        test1.insert(Car(test, priority, test));
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to insert 8,000 Cars: " << std::setw(15) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

//--------------------------------------------------------Removing 8000 cars into a heap--------------------------------------------------------//

    time1 = std::chrono::high_resolution_clock::now();

    while (test1.size() != 0)
    {
        test1.removeTop();
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to remove 8,000 Cars: " << std::setw(15) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

//--------------------------------------------------------Inserting 16000 cars into a heap--------------------------------------------------------//

    time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 16000; i++)
    {
        priority = (rand() % 1000) + 1;
        test1.insert(Car(test, priority, test));
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to insert 16,000 Cars: " << std::setw(14) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

//--------------------------------------------------------Removing 16000 cars into a heap--------------------------------------------------------//

    time1 = std::chrono::high_resolution_clock::now();

    while (test1.size() != 0)
    {
        test1.removeTop();
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to remove 16,000 Cars: " << std::setw(14) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

//--------------------------------------------------------Inserting 32000 cars into a heap--------------------------------------------------------//

    time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 32000; i++)
    {
        priority = (rand() % 1000) + 1;
        test1.insert(Car(test, priority, test));
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to insert 32,000 Cars: " << std::setw(14) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

//--------------------------------------------------------Removing 32000 cars into a heap--------------------------------------------------------//

    time1 = std::chrono::high_resolution_clock::now();

    while (test1.size() != 0)
    {
        test1.removeTop();
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to remove 32,000 Cars: " << std::setw(14) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

    cout << "------------Testing Runtimes For Hash Table ------------" << endl << endl;

    //--------------------------------------------------------Hash Table Runtime Testing--------------------------------------------------------//
    HashTable<Car> test2(1000, simple_hash);

//--------------------------------------------------------Inserting 1000 Unique Cars Into a Hash Table--------------------------------------------------------//
    
    time1 = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 1000; i++) //Inserts items into the table
    {
        string key = std::to_string(i);
        test2.insert(Car(key, 42, test));
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to insert 1,000 Cars: " << std::setw(18) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

    //--------------------------------------------------------Removing 1000 Unique Cars Into a Hash Table--------------------------------------------------------//

    time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 1000; i++) //Removes items from the table
    {
        string key = std::to_string(i);
        test2.getNext(key, d);
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to Retrieve 1,000 Cars: " << std::setw(16) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

    HashTable<Car> test3(2000, simple_hash);

    //--------------------------------------------------------Inserting 2000 Unique Cars Into a Hash Table--------------------------------------------------------//

    time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 2000; i++) //Inserts items into the table
    {
        string key = std::to_string(i);
        test3.insert(Car(key, 42, test));
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to insert 2,000 Cars: " << std::setw(18) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

    //--------------------------------------------------------Removing 2000 Unique Cars Into a Hash Table--------------------------------------------------------//

    time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 2000; i++) //Removes items from the table
    {
        string key = std::to_string(i);
        test3.getNext(key, d);
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to Retrieve 2,000 Cars: " << std::setw(16) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

//--------------------------------------------------------Inserting 4000 Unique Cars Into a Hash Table--------------------------------------------------------//

    HashTable<Car> test4(4000, simple_hash);
    
    time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 4000; i++) //Inserts items into the table
    {
        string key = std::to_string(i);
        test4.insert(Car(key, 42, test));
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to insert 4,000 Cars: " << std::setw(18) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

    //--------------------------------------------------------Removing 4000 Unique Cars Into a Hash Table--------------------------------------------------------//

    time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 4000; i++) //Removes items from the table
    {
        string key = std::to_string(i);
        test4.getNext(key, d);
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to Retrieve 4,000 Cars: " << std::setw(16) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

//--------------------------------------------------------Inserting 8000 Unique Cars Into a Hash Table--------------------------------------------------------//
    
    HashTable<Car> test5(8000, simple_hash);

    time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 8000; i++) //Inserts items into the table
    {
        string key = std::to_string(i);
        test5.insert(Car(key, 42, test));
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to insert 8,000 Cars: " << std::setw(18) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

    //--------------------------------------------------------Removing 8000 Unique Cars Into a Hash Table--------------------------------------------------------//

    time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 8000; i++) //Removes items from the table
    {
        string key = std::to_string(i);
        test5.getNext(key, d);
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to Retrieve 8,000 Cars: " << std::setw(16) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

    //--------------------------------------------------------Inserting 16000 Unique Cars Into a Hash Table--------------------------------------------------------//

    HashTable<Car> test6(16000, simple_hash);

    time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 16000; i++) //Inserts items into the table
    {
        string key = std::to_string(i);
        test6.insert(Car(key, 42, test));
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to insert 16,000 Cars: " << std::setw(17) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

    //--------------------------------------------------------Removing 16000 Unique Cars Into a Hash Table--------------------------------------------------------//

    time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 16000; i++) //Removes items from the table
    {
        string key = std::to_string(i);
        test6.getNext(key, d);
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to Retrieve 16,000 Cars: " << std::setw(15) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

    //--------------------------------------------------------Inserting 32000 Unique Cars Into a Hash Table--------------------------------------------------------//

    HashTable<Car> test7(32000, simple_hash);

    time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 32000; i++) //Inserts items into the table
    {
        string key = std::to_string(i);
        test7.insert(Car(key, 42, test));
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to insert 32,000 Cars: " << std::setw(17) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

    //--------------------------------------------------------Removing 32000 Unique Cars Into a Hash Table--------------------------------------------------------//

    time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 32000; i++) //Removes items from the table
    {
        string key = std::to_string(i);
        test7.getNext(key, d);
    }

    time2 = std::chrono::high_resolution_clock::now();

    totalTime = (std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count()) / 1000.00;

    cout << std::setw(20) << std::left << "Time to Retrieve 32,000 Cars: " << std::setw(15) << std::right << std::fixed << totalTime << " seconds" << endl << endl; //Outputs total time in seconds

}
