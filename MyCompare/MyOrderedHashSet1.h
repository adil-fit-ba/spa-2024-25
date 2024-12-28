#pragma once

#include "MyBaseCollection.h"
#include "MyList.h"
#include "MyHashFunctions.h"
#include <functional>

template <class Tip>
class MyOrderedHashSet1 : public MyBaseCollection<Tip> {
    string collection_name;

    MyList<Tip>* buckets = nullptr;
    MyList<Tip> order; // Lista koja čuva redoslijed umetanja
    int bucket_size;
    int counter = 0;

    function<int(const Tip&, const int bucket_size)> my_hash_function= my_default_hash_function<Tip>;;

    void rehash() {
        MyList<Tip>* oldBuckets = buckets;
        MyList<Tip> oldOrder = order;
        int oldbucket_size = bucket_size;
        bucket_size *= 2;
        buckets = new MyList<Tip>[bucket_size];
        counter = 0;
        order = MyList<Tip>();

        for (int i = 0; i < oldbucket_size; ++i) {
            while (!oldBuckets[i].empty()) {
                Tip x = oldBuckets[i].remove();
                add(x); // Ponovno umetanje u prošireni hash set
            }
        }

        auto current = oldOrder.prvi;
        while (current != nullptr) {
            add(current->info);
            current = current->next;
        }
    }

    void check_bucket_size() {
        if (counter > bucket_size * 0.7) {
            rehash();
        }
    }

public:
    MyOrderedHashSet1(int bucket_size = 100, function<int(const Tip&, const int)>*my_hash_function = nullptr) {
        this->bucket_size = bucket_size;
        buckets = new MyList<Tip>[bucket_size];
        collection_name = "MyOrderedHashSet1 [MyList<Tip>* buckets + MyList<Tip> order]";

        if (my_hash_function != nullptr)
        {
            this->my_hash_function = *my_hash_function;
        }
    }

    string get_collection_name() override {
        return this->collection_name;
    }

    bool add(const Tip& v) override {
        int i = my_hash_function(v, this->bucket_size);
        if (buckets[i].search(v)) {
            return false;
        }
        buckets[i].add(v);
        order.add(v); // add u listu reda umetanja
        counter++;
        check_bucket_size();
        return true;
    }

    bool erase(const Tip& kljuc) override {
        int i = my_hash_function(kljuc, this->bucket_size);
        if (buckets[i].erase(kljuc)) {
            order.erase(kljuc); // Ukloni iz liste reda umetanja
            counter--;
            return true;
        }
        return false;
    }

    bool search(const Tip& v) const override {
        int i = my_hash_function(v, this->bucket_size);
        return buckets[i].search(v);
    }

    int count() const override {
        return counter;
    }

    bool empty() const override {
        return counter == 0;
    }

    void showDistribution() const {
        cout << "Distribucija elemenata po bucketima:\n";
        for (int i = 0; i < bucket_size; i++) {
            cout << "Bucket " << i << ": " << buckets[i].count() << " elemenata\n";
        }
    }

    void print(int take = 100, int skip = 0) const override {
        int totalPrinted = 0;
        auto current = order.prvi;

        while (current != nullptr) {
            if (totalPrinted >= skip && totalPrinted < skip + take) {
                cout << current->info << " | ";
            }

            totalPrinted++;
            if (totalPrinted >= skip + take) {
                break;
            }

            current = current->next;
        }

        cout << endl << endl;
    }
    void info() const override {

    }
};
