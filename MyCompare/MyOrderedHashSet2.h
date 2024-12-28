#pragma once
#include "MyBaseCollection.h"
#include "MyList.h"
#include "MyDictionary.h" // Umjesto unordered_map
#include "MyHashSet.h"

template <class Tip>
class MyOrderedHashSet2 : public MyBaseCollection<Tip> {
    MyDictionary<Tip, Cvor<Tip>*> my_map; // mapa
    MyList<Tip> order;                 // Lista koja �uva redoslijed umetanja
    int counter = 0;
    string collection_name;
public:
    MyOrderedHashSet2() {
        this->collection_name = "MyOrderedHashSet2 [MyDictionary<Tip, Cvor*> mapa + MyList<Tip> order]";
    }

    string get_collection_name() override {
        return this->collection_name;
    }

    bool add(const Tip& v) override {
        if (my_map.search(v)) { // Provjera da li klju� ve� postoji
            return false;
        }
        order.add(v); // add u listu reda umetanja
        my_map.add(v, order.zadnji); // add u MyDictionary s pokaziva�em na zadnji �vor
        counter++;
        return true;
    }

    bool erase(const Tip& kljuc) override {
        // Prona�i �vor u mapi
        if (!my_map.search(kljuc)) {
            return false; // Klju� ne postoji
        }

        Cvor<Tip>* cvor = my_map[kljuc]; // Dohvati pokaziva� na �vor iz mape

        // Ukloni �vor iz liste
        order.erase_po_cvoru(cvor);

        // Ukloni �vor iz MyDictionary
        my_map.erase(kljuc);

        counter--;
        return true;
    }

    bool search(const Tip& v) const override {
        return my_map.search(v);
    }

    int count() const override {
        return counter;
    }

    bool empty() const override {
        return counter == 0;
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
