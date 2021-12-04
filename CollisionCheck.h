//
// Created by Eyu on 11/17/2021.
//

#ifndef PROJECT5_COLLISIONCHECK_H
#define PROJECT5_COLLISIONCHECK_H

#include <functional>
#include <string>
#include <vector>
using std::function, std::string, std::vector;

#include <optional>
using std::optional;
using std::nullopt;
using std::make_optional;

template<typename Hashable>
class CollisionCheck {
private:
    vector<optional<Hashable>> table;
    unsigned long tableSize;
    function<string(Hashable)> getKey;

    unsigned long hornerHash(string key) {
        unsigned long hashVal = 0;
        for (char letter : key) {
            hashVal = hashVal * 37 + letter;
        }
        return hashVal % tableSize;
    }

public:
    // Constructor
    // Format of function: function<returnType(parameterTypes)>
    CollisionCheck(unsigned long tableSize, function<string(Hashable)> getKey) {
        this->tableSize = tableSize;
        this->getKey = getKey;
        table.resize(tableSize);
    }

    // Insert
    void insert(Hashable item) {
        // get the string key from the item
        string key = getKey(item);
        // hash the key to get an index
        unsigned long index = hornerHash(key);
        // put the item at that index in the table
        table[index] = item;
    }

    // Find
    optional<Hashable> find(string key) {
        // hash the key to get an index
        unsigned long index = hornerHash(key);
        // check if the index holds a value, and if the value has a key that matches the parameter
        if (table[index] && getKey(table[index].value()) == key) {
            // we found the item
            return table[index].value();
        }
        return nullopt;
    }

    // Remove
    bool remove(string key) {
        // hash the key to get an index
        unsigned long index = hornerHash(key);
        // check if the index holds a value, and if the value has a key that matches the parameter
        if (table[index] && getKey(table[index].value()) == key) {
            // We found the item. Remove it.
            table[index] = nullopt;
            return true;
        }
        return false;
    }
};


#endif //PROJECT5_COLLISIONCHECK_H
