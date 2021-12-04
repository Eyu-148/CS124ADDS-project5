//created on nov 17

#include "Course.h"
#include "SeparateChaining.h"
#include "NonlinearProbing.h"
#include <iostream>
#include <string>
#include <ctime>
#include <algorithm>
#include <cstdlib>
#include <chrono>
using namespace std;

//default hash function
unsigned long hornerHash(string key, unsigned long tableSize) {
    unsigned long hashVal = 0;
    for (char letter : key) {
        hashVal = hashVal * 37 + letter;
    }
    return hashVal % tableSize;
}

//another hash function
unsigned long hashHash(string key, unsigned long tableSize) {
    unsigned long hashVal = key.length();
    return hashVal % tableSize;
}


// another getKey
string intToString(Course object) {
    int value = object.getCRN();
    return to_string(value);
}

// default getKey function for the Course class
string getVector(Course object) {
   return object.getEmail();
}

// Default hash method
// Get the best size from this step
// Create separate chaining and quadratic probing (getKey accept Course object)
tuple<unsigned long, unsigned long, unsigned long, unsigned long, int, int> hashCourse(unsigned long size,
                vector<Course> unhashedVector,
                function<string(Course)> getKey,
                function<unsigned long(string, unsigned long)> hashFunction) {
    unsigned long readInsertSep = 0, readInsertQua = 0;
    int finalSizeSep, finalSizeQua;
    int vectorSize = unhashedVector.size();
    SeparateChaining<Course> hashSep(size, getKey, hashFunction);
    // timer for separate chaining insertion
    auto start1 = chrono::high_resolution_clock::now();
    for (int i=0; i<vectorSize; ++i) {
        auto recordSep = hashSep.insert(unhashedVector[i]);
        readInsertSep += get<0>(recordSep);
        finalSizeSep = get<1>(recordSep);
    }
    auto stop1 = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::microseconds>(stop1 - start1);

    NonlinearProbing<Course> hashQua(size, getKey, hashFunction);
    //timer for quadratic probing
    auto start2 = chrono::high_resolution_clock::now();
    for (int i=0; i<vectorSize; ++i) {
        auto recordQua = hashQua.insert(unhashedVector[i]);
        readInsertQua += get<0>(recordQua);
        finalSizeQua = get<1>(recordQua);
    }
    auto stop2 = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::microseconds>(stop2 - start2);

    return make_tuple(readInsertSep, readInsertQua, duration1.count(), duration2.count(), finalSizeSep, finalSizeQua);
}

// Hash method with different getKey and hash functions
// Create separate chaining and quadratic probing (getKey accept Course object)
tuple<unsigned long, unsigned long, unsigned long, unsigned long> anotherKey(unsigned long size,
                vector<Course> unhashedVector,
                function<string(Course)> getKey) {
    unsigned long readInsertSep = 0, readInsertQua = 0;
    int vectorSize = unhashedVector.size();
    SeparateChaining<Course> hashSep(size, getKey, hornerHash);
    auto start1 = chrono::high_resolution_clock::now();
    for (int i=0; i<vectorSize; ++i) {
        readInsertSep += get<0>(hashSep.insert(unhashedVector[i]));
    }
    auto stop1 = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::microseconds>(stop1 - start1);

    NonlinearProbing<Course> hashQua(size, getKey, hornerHash);
    auto start2 = chrono::high_resolution_clock::now();
    for (int i=0; i<vectorSize; ++i) {
        readInsertQua += get<0>(hashQua.insert(unhashedVector[i]));
    }
    auto stop2 = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::microseconds>(stop2 - start2);

    return make_tuple(readInsertSep, readInsertQua, duration1.count(), duration2.count());
}

tuple<unsigned long, unsigned long, unsigned long, unsigned long> anotherHash(unsigned long size,
                vector<Course> unhashedVector,
                function<unsigned long(string, unsigned long)> hashFunction) {
    unsigned long readInsertSep = 0, readInsertQua = 0;
    int vectorSize = unhashedVector.size();
    SeparateChaining<Course> hashSep(size, getVector, hashFunction);
    auto start1 = chrono::high_resolution_clock::now();
    for (int i=0; i<vectorSize; ++i) {
        readInsertSep += get<0>(hashSep.insert(unhashedVector[i]));
    }
    auto stop1 = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::microseconds>(stop1 - start1);

    NonlinearProbing<Course> hashQua(size, getVector, hashFunction);
    auto start2 = chrono::high_resolution_clock::now();
    for (int i=0; i<vectorSize; ++i) {
        readInsertQua += get<0>(hashQua.insert(unhashedVector[i]));
    }
    auto stop2 = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::microseconds>(stop2 - start2);

    return make_tuple(readInsertSep, readInsertQua, duration1.count(), duration2.count());
}

/**
 * function to write the data into file
 **/
void writeToFile(string tableType, unsigned long numSep, unsigned long numQua){
    ofstream outFile;
    outFile.open("hash_table.csv", ios::app);
    outFile << tableType << ", "
            << numSep <<  ", "
            << numQua <<  ", "
            << endl;
    outFile.close();
}


int main() {
    // Reda the input file and create vector of objects
    vector<Course> course;
    string fn = "../uvm_fall2021_courses.csv";

    if (loadFromFile(fn, course)) {
        cout << course.size()
             << " records read from file" << endl;
    } else {
        cout << "Something went wrong." << endl;
    }

    // The header of file
    ofstream outFile;
    outFile.open("hash_table.csv", ios::app);
    outFile << "Type" << ", "
            << "#Separate chaining" <<  ", "
            << "#Quadratic probing" <<  ", "
            << endl;
    outFile.close();

    // will create 5 tables of varying sizes for separate chaining and quadratic probing
    // find out the best size
    for (int t=0; t<5; ++t) {
        auto record = hashCourse(course.size()+(t*500), course, getVector, hornerHash);
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "Default table in size " << course.size()+(t*500) << endl;
        //cout << "Total # of Separate chaining: " << get<0>(record) << endl;
        //cout << "Final size of Separate chaining: " << get<4>(record) << endl;
        cout << "Timer: " << get<2>(record)  << " microseconds" << endl;
        //cout << "Total # of Quadratic probing: " << get<1>(record) << endl;
        //cout << "Final size of Quadratic probing: " << get<5>(record) << endl;
        cout << "Timer: " << get<3>(record) << " microseconds" << endl;
        //writeToFile("default " + course.size() * t, get<0>(record), get<1>(record));
    }

    // Create 3 tables with different getKey and hash functions
    auto record2 = anotherKey(5922, course, intToString);
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Hash table with a different getKey function" << endl;
    //cout << "Total # of Separate chaining: " << get<0>(record2) << endl;
    cout << "Timer: " << get<2>(record2) << " microseconds" << endl;
    //cout << "Total # of Quadratic probing: " << get<1>(record2) << endl;
    cout << "Timer: " << get<3>(record2) << " microseconds" << endl;
    //writeToFile("New getKey " + course.size() * 5, get<0>(record2), get<1>(record2));

    auto record3 = anotherHash(5922, course, hashHash);
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Hash table with a different hash function" << endl;
    //cout << "Total # of Separate chaining: " << get<0>(record3) << endl;
    cout << "Timer: " << get<2>(record3) << " microseconds" << endl;
    //cout << "Total # of Quadratic probing: " << get<1>(record3) << endl;
    cout << "Timer: " << get<3>(record3) << " microseconds" << endl;
    //writeToFile("New Hash " + course.size() * 5, get<0>(record3), get<1>(record3));

    auto record4 = hashCourse(5922, course, intToString, hashHash);
    cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    cout << "Both differ in getKey ans hash function" << endl;
    //cout << "Total # of Separate chaining: " << get<0>(record4) << endl;
    cout << "Timer: " << get<2>(record4) << " microseconds" << endl;
    //cout << "Total # of Quadratic probing: " << get<1>(record4) << endl;
    cout << "Timer: " << get<3>(record4) << " microseconds" << endl;
    //writeToFile("Change Both " + course.size() * 6, get<0>(record4), get<1>(record4));

    return 0;
}