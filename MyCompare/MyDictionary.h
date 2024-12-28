#pragma once
#include "MyBaseCollection.h"
#include "MyList.h"
#include "MyHashFunctions.h"
#include <iostream>
#include <stdexcept>
using namespace std;
#include <functional>
#include <utility> // Za std::pair

template <typename Key, typename Value>
std::ostream& operator<<(std::ostream& os, const std::pair<Key, Value>& p) {
    os << "{" << p.first << ": " << p.second << "}";
    return os;
}


template <class Key, class Value>
class MyDictionary {
    MyList<pair<Key, Value>>* buckets = nullptr;
    int bucket_size;
    int counter = 0;

    function<int(const Key&, const int bucket_size)> my_hash_function = my_default_hash_function<Key>;;

    void rehash() {
        MyList<pair<Key, Value>>* oldBuckets = buckets;
        int oldbucket_size = bucket_size;
        bucket_size *= 2;
        buckets = new MyList<pair<Key, Value>>[bucket_size];
        counter = 0;

        for (int i = 0; i < oldbucket_size; ++i) {
            while (!oldBuckets[i].empty()) {
                auto kv = oldBuckets[i].remove();
                add(kv.first, kv.second);
            }
        }

        delete[] oldBuckets;
    }

    void checkLoadFactor() {
        if (counter > bucket_size * 0.7) {
            rehash();
        }
    }

public:
    MyDictionary(int bucket_size = 100, function<int(const Key&, const int)>* my_hash_function = nullptr) {
        this->bucket_size = bucket_size;
        buckets = new MyList<pair<Key, Value>>[bucket_size];
        if (my_hash_function != nullptr)
        {
            this->my_hash_function = *my_hash_function;
        }
    }

    ~MyDictionary() {
        delete[] buckets;
    }

    bool add(const Key& key, const Value& value) {
        int i = my_hash_function(key, bucket_size);
        if (search(key)) {
            return false; // Key already exists
        }
        buckets[i].add(make_pair(key, value));
        counter++;
        checkLoadFactor();
        return true;
    }

    bool erase(const Key& key) {
        int i = my_hash_function(key, bucket_size);
        auto current = buckets[i].prvi;

        while (current != nullptr) {
            if (current->info.first == key) {
                buckets[i].erase_po_cvoru(current);
                counter--;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    bool search(const Key& key) const {
        int i = my_hash_function(key, bucket_size);
        auto current = buckets[i].prvi;

        while (current != nullptr) {
            if (current->info.first == key) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    Value& operator[](const Key& key) {
        int i = my_hash_function(key, bucket_size);
        auto current = buckets[i].prvi;

        while (current != nullptr) {
            if (current->info.first == key) {
                return current->info.second;
            }
            current = current->next;
        }

        // If key does not exist, insert a default value and return it
        add(key, Value());
        return buckets[i].prvi->info.second;
    }

    void print() const {
        for (int i = 0; i < bucket_size; ++i) {
            auto current = buckets[i].prvi;
            cout << "Bucket " << i << ": ";
            while (current != nullptr) {
                cout << "{" << current->info.first << ": " << current->info.second << "} ";
                current = current->next;
            }
            cout << endl;
        }
    }

    int count() const {
        return counter;
    }

    bool empty() const {
        return counter == 0;
    }
};
