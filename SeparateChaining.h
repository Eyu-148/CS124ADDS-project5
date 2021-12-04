//
// Created by Eyu on 11/17/2021.
//

#ifndef PROJECT5_SEPARATECHAINING_H
#define PROJECT5_SEPARATECHAINING_H

#include <tuple>
#include <functional>
#include <string>
#include <vector>
using std::function, std::string, std::vector;

#include <optional>
using std::optional;
using std::nullopt;
using std::make_optional;

template<typename Hashable>
class SeparateChaining {
private:
    vector<vector<Hashable>> table;
    unsigned long tableSize;
    function<string(Hashable)> getKey;
    function<unsigned long(string, unsigned long)> hashFunction;

    unsigned long hornerHash(string key) {
        unsigned long hashVal = 0;
        for (char letter : key) {
            hashVal = hashVal * 37 + letter;
        }
        return hashVal % tableSize;
    }

    // Find the next prime number
    int nextPrime(int n) {
        if (n % 2 == 0) {
            ++n;
        }
        bool prime = false;
        while (!prime) {
            prime = true;
            for (int i = 3; i * i <= n; i += 2) {
                if (n % i == 0) {
                    prime = false;
                }
            }
            n += 2;
        }
        return (n-2);
    }

public:
    // Constructor for int value
    // Format of function: function<returnType(parameterTypes)>
    SeparateChaining(int tableSize, function<string(Hashable)> getKey, function<unsigned long(string, unsigned long)> hashFunction) {
        this->tableSize = nextPrime(tableSize);
        this->getKey = getKey;
        this->hashFunction = hashFunction;
        table.resize(this->tableSize);
    }

    // Insert
    // Return the number of reads and final size
    std::tuple<unsigned long, int> insert(Hashable item) {
        // get the string key from the item
        string key = getKey(item);
        unsigned long numRead;
        // check that the item isn't already in the table
        if (find(key, numRead) != nullopt) {
            // Put the item at that index in the table
            return std::make_tuple(numRead, tableSize);
        }
        // hash the key to get an index
        unsigned long index = hashFunction(key, tableSize);
        // put the item at that index in the table
        table[index].push_back(item);
        numRead += 1;
        return std::make_tuple(numRead, tableSize);
    }

    // Find
    optional<Hashable> find(string key, unsigned long &numRead) {
        unsigned long index = hashFunction(key, tableSize);
        // Loop through the items stored at the index

        for (int i = 0; i < table[index].size(); ++i) {
            numRead += 1;
            // Check that the object is the one we're looking for
            if (key == getKey(table[index][i])) {
                numRead++;
                // We found the object
                return table[index][i];
            }
        }
        // We did not find the object
        return nullopt;
    }

    // Remove
    optional<Hashable> remove(string key) {
        unsigned long index = hashFunction(key, tableSize);
        // Loop through the items stored at the index
        for (int i = 0; i < table[index].size(); ++i) {
            // Check that the object is the one we're looking for
            if (key == getKey(table[index][i])) {
                // We found the object. Delete it.
                Hashable returnObject = table[index][i];
                // Use the vector erase method to remove the item.
                table[index].erase(table[index].begin() + i);
                return returnObject;
            }
        }
        // We did not find the object
        return nullopt;
    }
};

#endif //PROJECT5_SEPARATECHAINING_H
