#pragma once
#include <unordered_set>

#include "MyBaseCollection.h"
using namespace  std;

template <class Tip>
class CppHashSet :public MyBaseCollection<Tip> {
	std::unordered_set<Tip> hashSet;
    string collection_name;
public:
    CppHashSet() {
        collection_name = "Cpp Set (Unorederd HashSet)";
    }

    string get_collection_name() override {
        return this->collection_name;
    }

    bool add(const Tip& v) override {
        hashSet.insert(v);
        return true;
    }

    bool erase(const Tip& v)  override {
        return hashSet.erase(v) > 0;
    }

    bool search(const Tip& v) const  override {
        return hashSet.find(v) != hashSet.end();
    }

    int count() const  override {
        return hashSet.size();
    }

    bool empty() const  override {
        return hashSet.empty();
    }

    void show_distribution() const {
        cout << "Distribucija elemenata po bucketima:\n";
        for (size_t i = 0; i < hashSet.bucket_count(); i++) {
            size_t bucket_size = distance(hashSet.begin(i), hashSet.end(i)); // Brojanje elemenata u bucketu
            cout << "Bucket " << i << ": " << bucket_size << " elemenata\n";
        }
    }

    void print(int take = 100, int skip = 0) const override {
        int count = 0; // Brojač za praćenje pozicije unutar skupa

        for (const auto& element : hashSet) {
            if (count >= skip && count < skip + take) {
                cout << element << " | ";
            }
            count++;

            // Prekidamo ako smo već uzeli sve potrebne elemente
            if (count >= skip + take) {
                break;
            }
        }

        cout << endl << endl;
    }
    void info() const override {

    }

};
