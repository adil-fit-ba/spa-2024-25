#pragma once
#include "MyBaseCollection.h"
#include "MyHashFunctions.h"
#include "MyList.h"
#include <functional>

template <class Tip>
class MyHashSet:public MyBaseCollection<Tip> {
    string collection_name;
    MyList<Tip>* buckets = nullptr;
    int bucket_size;
    int counter = 0;

    function<int(const Tip&, const int bucket_size)> my_hash_function = my_default_hash_function<Tip>;

    void rehash() {
        MyList<Tip>* oldBuckets = buckets;
        int oldbucket_size = bucket_size;
        bucket_size *= 2;
        buckets = new MyList<Tip>[bucket_size];
        counter = 0;

        for (int i = 0; i < oldbucket_size; ++i) {
            while (!oldBuckets[i].empty()) {
                Tip x = oldBuckets[i].remove();
                add(x);
            }
        }
    }

    void check_bucket_size() {
        if (counter > bucket_size * 0.7) {
            rehash();
        }
    }

public:
    MyHashSet(int bucket_size = 100, function<int(const Tip&, const int)>* my_hash_function = nullptr) {
        this->bucket_size = bucket_size;
        buckets = new MyList<Tip>[bucket_size];

        collection_name = "MyHashSet";

        if (my_hash_function != nullptr)
        {
            this->my_hash_function = *my_hash_function;
        }
    }

    string get_collection_name() override {
        return this->collection_name;
    }

    bool add(const Tip& v) override {
        int i = my_hash_function(v, bucket_size);
        if (buckets[i].search(v)) {
            return false;
        }
        buckets[i].add(v);
        counter++;
        check_bucket_size();
        return true;
    }

    bool erase(const Tip& v) override {
        int i = my_hash_function(v, bucket_size);
        if (buckets[i].erase(v)) {
            counter--;
            return true;
        }
        return false;
    }

    bool search(const Tip& v) const override {
        int i = my_hash_function(v, bucket_size);
        return buckets[i].search(v);
    }

    int count() const override {
        return counter;
    }

    bool empty() const override {
        return counter == 0;
    }

    void show_distribution() const {
        cout << "Distribucija elemenata po bucketima:\n";
        for (int i = 0; i < bucket_size; i++) {
            cout << "Bucket " << i << ": " << buckets[i].count() << " elemenata\n";
        }
    }

    void print(int take = 100, int skip = 0) const override {
        int totalPrinted = 0; // Ukupan broj ispisanih elemenata

        for (int i = 0; i < bucket_size; ++i) {
            Cvor<Tip>* current = buckets[i].prvi; // Početni pokazivač na prvi čvor u bucket-u

            while (current != nullptr) {
                if (totalPrinted >= skip && totalPrinted < skip + take) {
                    cout << current->info << " | ";
                }

                totalPrinted++;
                if (totalPrinted >= skip + take) {
                    break; // Prekini ako smo već ispisali dovoljno elemenata
                }

                current = current->next; // Pomjeri na sljedeći čvor
            }

            if (totalPrinted >= skip + take) {
                break; // Prekini iteraciju kroz bucket-e ako smo dostigli limit
            }
        }

        cout << endl << endl;
    }
    void info() const override {

    }
};
