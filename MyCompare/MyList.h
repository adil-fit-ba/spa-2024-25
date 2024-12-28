#pragma once
#include "MyBaseCollection.h"

template<class Tip>
class Cvor {
public:
    Cvor* next;
    Cvor* prev; // Dodan pokazivač na prethodni čvor
    Tip info;

    Cvor(Tip info, Cvor* next = nullptr, Cvor* prev = nullptr) {
        this->info = info;
        this->next = next;
        this->prev = prev;
    }
};

template <class Tip>
class MyList : public MyBaseCollection<Tip> {
    string collection_name;
public:
    MyList() {
        collection_name = "MyList";
    }

    string get_collection_name() override {
        return this->collection_name;
    }

    Cvor<Tip>* prvi = nullptr;
    Cvor<Tip>* zadnji = nullptr; // Dodan pokazivač na zadnji čvor

    bool add(const Tip& v) override {
        Cvor<Tip>* novi = new Cvor<Tip>(v, nullptr, zadnji);
        if (zadnji != nullptr) {
            zadnji->next = novi;
        }
        else {
            prvi = novi; // Ako je lista prazna, ažuriramo prvi
        }
        zadnji = novi;
        return true;
    }

    Tip remove() {
        if (prvi == nullptr) {
            throw exception("Prazna lista");
        }

        Cvor<Tip>* t = prvi;
        prvi = prvi->next;
        if (prvi != nullptr) {
            prvi->prev = nullptr;
        }
        else {
            zadnji = nullptr; // Ako je lista prazna, ažuriramo i zadnji
        }

        Tip x = t->info;
        delete t;
        return x;
    }


    bool erase(const Tip& x) override {
        Cvor<Tip>* t = prvi;

        while (t != nullptr) {
            if (t->info == x) {
                return erase_po_cvoru(t); // Koristimo novu funkciju erase za čvor
            }
            t = t->next;
        }
        return false;
    }

    bool erase_po_cvoru(Cvor<Tip>*& x) {
        if (x == nullptr) {
            return false;
        }

        // Ažuriranje `prev`
        if (x->prev != nullptr) {
            x->prev->next = x->next;
        }
        else {
            prvi = x->next; // Ako je prvi čvor
        }

        // Ažuriranje `next`
        if (x->next != nullptr) {
            x->next->prev = x->prev;
        }
        else {
            zadnji = x->prev; // Ako je zadnji čvor
        }

        delete x;    // Oslobađanje memorije
        x = nullptr; // Postavljanje pokazivača na nullptr
        return true;
    }


    bool search(const Tip& x) const override {
        Cvor<Tip>* t = prvi;
        while (t != nullptr) {
            if (t->info == x) {
                return true;
            }
            t = t->next;
        }
        return false;
    }

    int count() const override {
        int count = 0;
        Cvor<Tip>* t = prvi;
        while (t != nullptr) {
            count++;
            t = t->next;
        }
        return count;
    }

    bool empty() const override {
        return prvi == nullptr;
    }

    void print(int take = 100, int skip = 0) const override {
        Cvor<Tip>* t = prvi; // Početak od prvog elementa
        int count = 0;         // Brojač za praćenje pozicije u listi

        while (t != nullptr) {
            // Preskoči prvih `skip` elemenata
            if (count >= skip) {
                // Ispisuje elemente do limita `take`
                if (count < skip + take) {
                    cout << t->info << " | ";
                }
                else {
                    break; // Prekid kada dostignemo limit `take`
                }
            }
            t = t->next; // Prelaz na naredni element
            count++;
        }

        cout << endl << endl;
    }
    void info() const override {

    }
};
