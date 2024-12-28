#pragma once
#include <list>

#include "MyBaseCollection.h"
using namespace std;

template <class Tip>
class CppList:public MyBaseCollection<Tip> {
	std::list<Tip> lst;
    string collection_name;
public:
    CppList() {
        collection_name = "Cpp std::list";
    }

    string get_collection_name() override {
        return this->collection_name;
    }

    bool add(const Tip& v) override {
        lst.push_back(v);
        return true;
    }

    bool erase(const Tip& v) override {
        auto it = std::find(lst.begin(), lst.end(), v);
        if (it != lst.end()) {
            lst.erase(it);
            return true;
        }
        return false;
    }

    bool search(const Tip& v) const override {
        auto it = std::find(lst.begin(), lst.end(), v);
        return it != lst.end();
    }

    int count() const override {
        return lst.size();
    }

    bool empty() const override {
        return lst.empty();
    }

    void print(int take = 100, int skip = 0) const override {
        int count = 0; // Brojač za praćenje pozicije unutar skupa

        for (const auto& element : lst) {
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
