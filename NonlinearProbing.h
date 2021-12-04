//
// Created by Eyu on 11/17/2021.
// (to be) Modified from LinearProbing.h
//

#ifndef PROJECT5_NONLINEARPROBING_H
#define PROJECT5_NONLINEARPROBING_H

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
class NonlinearProbing {
private:
    enum state {EMPTY, FILLED, REMOVED};
    struct hashItem {
        state status;
        Hashable item;
    };
    vector<hashItem> table;
    unsigned long tableSize;
    function<string(Hashable)> getKey;
    function<unsigned long(string, unsigned long)> hashFunction;
    unsigned long numItems;

    unsigned long hornerHash(string key, int tableSize) {
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

    void rehash(unsigned long &read) {
        // Save a copy of the hash table
        vector<hashItem> oldTable = table;
        // Double the size of the table and go to the next prime
        tableSize = nextPrime(tableSize * 2);
        table.clear();
        table.resize(tableSize);
        // Reinsert all of the items
        numItems = 0;
        for (unsigned long i = 0; i < oldTable.size(); ++i) {
            if (oldTable[i].status == FILLED) {
                insert(oldTable[i].item);
                read++;
            }
        }
    }

public:
    // Constructor
    // Format of function: function<returnType(parameterTypes)>
    NonlinearProbing(unsigned long tableSize, function<string(Hashable)> getKey, function<unsigned long(string, unsigned long)> hashFunction) {
        this->tableSize = nextPrime(tableSize);
        this->getKey = getKey;
        this->hashFunction = hashFunction;
        table.resize(this->tableSize);
        numItems = 0;
    }

    // Insert
    std::tuple<unsigned long, unsigned long> insert(Hashable item) {
        unsigned long read = 0;
        // get the string key from the item
        string key = getKey(item);
        // get the number of read from find method
        if (find(key, read) != nullopt) {
            // the item is already in the table. No duplicates allowed.
            return std::make_tuple(read, tableSize);
        }
        // hash the key to get an index
        unsigned long index = hashFunction(key, tableSize);
        // search for a spot to put the item
        int i = 1;
        while (table[index].status == FILLED) {
            // quadratic probing
            index = (index + i * i) % tableSize;
            ++i;
        }
        table[index].item = item;
        table[index].status = FILLED;
        ++read;
        ++numItems;
        if (numItems > tableSize / 2) {
            rehash(read);
        }
        return std::make_tuple(read, tableSize);
    }

    // Find
    optional<Hashable> find(string key, unsigned long &read) {
        // hash the key to get an index
        unsigned long index = hashFunction(key, tableSize);
        int i = 1;
        while (table[index].status != EMPTY) {
            read+=3;
            // check if the index holds a value, and if the value has a key that matches the parameter
            if (table[index].status == FILLED && getKey(table[index].item) == key) {
                // we found the item
                read += 1;
                return table[index].item;
            }
            // quadratic probing
            index = (index + i * i) % tableSize;
            ++i;
        }
        // we did not find the item
        read++;
        return nullopt;
    }

    // Remove
    bool remove(string key, int tableSize) {
        // hash the key to get an index
        unsigned long index = hashFunction(key, tableSize);
        int i = 1;
        while (table[index].status != EMPTY) {
            // check if the index holds a value, and if the value has a key that matches the parameter
            if (table[index].status == FILLED && getKey(table[index].item) == key) {
                // we found the item
                // remove it
                table[index].status = REMOVED;
                table[index].item = Hashable();
                return true;
            }
            // gotta probe
            index = (index + i * i) % tableSize;
            ++i;
        }
        // we did not find the item
        return false;
    }
};

#endif //PROJECT5_NONLINEARPROBING_H
