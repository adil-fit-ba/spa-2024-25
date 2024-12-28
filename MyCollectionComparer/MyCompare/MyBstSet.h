#pragma once

#include <iostream>
#include <functional>
using namespace std;
#include "MyComparator.h"
#include "MyBstNode.h"

// Klasa za implementaciju skupa koriste�i AVL stablo (balansirano binarno stablo pretra�ivanja).
// https://www.programiz.com/dsa/avl-tree
// T predstavlja generi�ki tip podataka koji se koristi unutar stabla.
template <class T>
class MyBstSet : public MyBaseCollection<T> {
private:
    string collection_name;
    const int imbalance_threshold; // Maksimalno dozvoljena neuravnote�enost �vora prije balansiranja.
    MyBstNode<T>* root = nullptr; // Pokaziva� na korijenski �vor stabla (po�etna ta�ka).
    int nodeCount = 0; // Ukupan broj �vorova u stablu.

    // Funkcija koja vra�a visinu datog �vora. 
    // Ako �vor ne postoji (nullptr), visina je 0.
    int getHeight(MyBstNode<T>* node) const {
        return node ? node->height : 0;
    }

    // Funkcija koja a�urira visinu �vora na osnovu visina njegovih podstabala.
    // Visina je definisana kao maksimalna visina lijevog ili desnog podstabla plus 1.
    void updateHeight(MyBstNode<T>* node) {
        if (node) {
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        }
    }

    // Funkcija koja ra�una balans faktor za �vor.
    // Balans faktor je razlika izme�u visine lijevog i desnog podstabla.
    // Ako je balans faktor izvan opsega [-1, 1], �vor je neuravnote�en.
    int getBalanceFactor(MyBstNode<T>* node) const {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    // Funkcija za rotaciju �vora udesno, �to je jedan od osnovnih koraka balansiranja stabla.
    // Koristi se u slu�ajevima kada je lijevo podstablo ve�e (visina ve�a) od desnog.
    MyBstNode<T>* rotateRight(MyBstNode<T>* y) {
        MyBstNode<T>* x = y->left; // Novi korijenski �vor nakon rotacije.
        MyBstNode<T>* t = x->right; // Privremeno �uva desno podstablo �vora x.

        // Izvr�ava rotaciju.
        x->right = y; // Premje�ta y kao desno dijete �vora x.
        y->left = t; // Postavlja t kao lijevo dijete �vora y.

        // A�urira visine �vorova nakon rotacije.
        updateHeight(y);
        updateHeight(x);
        right_rotate_counter++; // Pove�ava broja� rotacija udesno za statisti�ke svrhe.
        return x; // Vra�a novi korijenski �vor.
    }

    // Funkcija za rotaciju �vora ulijevo, suprotna operacija od rotacije udesno.
    // Koristi se kada je desno podstablo ve�e od lijevog.
    MyBstNode<T>* rotateLeft(MyBstNode<T>* x) {
        MyBstNode<T>* y = x->right; // Novi korijenski �vor nakon rotacije.
        MyBstNode<T>* t = y->left; // Privremeno �uva lijevo podstablo �vora y.

        // Izvr�ava rotaciju.
        y->left = x; // Premje�ta x kao lijevo dijete �vora y.
        x->right = t; // Postavlja t kao desno dijete �vora x.

        // A�urira visine �vorova nakon rotacije.
        updateHeight(x);
        updateHeight(y);
        left_rotate_counter++; // Pove�ava broja� rotacija ulijevo za statisti�ke svrhe.
        return y; // Vra�a novi korijenski �vor.
    }

    // Funkcija za balansiranje datog �vora.
    // Ako �vor ima balans faktor izvan opsega [-1, 1], izvr�ava se odgovaraju�a rotacija.
    MyBstNode<T>* balance(MyBstNode<T>* node) {
        updateHeight(node); // A�urira visinu trenutnog �vora.

        int balanceFactor = getBalanceFactor(node); // Dobija balans faktor �vora.

        // Lijevo-lijevo slu�aj: lijevo podstablo je preveliko, a njegov balans faktor je tako�er pozitivan.
        if (balanceFactor > imbalance_threshold && getBalanceFactor(node->left) >= 0) {
            return rotateRight(node);
        }

        // Lijevo-desno slu�aj: lijevo podstablo je preveliko, ali njegov balans faktor je negativan.
        // Potrebna je rotacija ulijevo pa udesno.
        if (balanceFactor > imbalance_threshold && getBalanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Desno-desno slu�aj: desno podstablo je preveliko, a njegov balans faktor je tako�er negativan.
        if (balanceFactor < -imbalance_threshold && getBalanceFactor(node->right) <= 0) {
            return rotateLeft(node);
        }

        // Desno-lijevo slu�aj: desno podstablo je preveliko, ali njegov balans faktor je pozitivan.
        // Potrebna je rotacija udesno pa ulijevo.
        if (balanceFactor < -imbalance_threshold && getBalanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node; // Ako je balans faktor u opsegu [-1, 1], nije potrebno balansiranje.
    }

    // Funkcija za dodavanje novog elementa u stablo uz balansiranje.
    bool addRecursive(MyBstNode<T>*& node, const T& value) {
        if (node == nullptr) {
            node = new MyBstNode<T>(value); // Kreira novi �or ako je mjesto dodavanja prona�eno.
            return true; // Dodavanje uspje�no.
        }

        if (value < node->value) {
            if (addRecursive(node->left, value)) {
                node = balance(node); // Balansira stablo nakon dodavanja.
                return true;
            }
        }
        else if (value > node->value) {
            if (addRecursive(node->right, value)) {
                node = balance(node); // Balansira stablo nakon dodavanja.
                return true;
            }
        }

        return false; // Vrijednost ve� postoji u stablu, ne dodaje se.
    }



    // Funkcija za brisanje �vora iz stabla uz balansiranje.
    bool eraseRecursive(MyBstNode<T>*& node, const T& value) {
        if (node == nullptr) {
            return false; // �or nije prona�en.
        }

        if (value < node->value) {
            return eraseRecursive(node->left, value); // Tra�i �or u lijevom podstablu.
        }
        else if (value > node->value) {
            return eraseRecursive(node->right, value); // Tra�i �or u desnom podstablu.
        }
        else {
            // �or za brisanje je prona�en.
            if (node->left == nullptr || node->right == nullptr) {
                MyBstNode<T>* temp = node->left ? node->left : node->right;
                delete node; // Bri�e trenutni �or.
                node = temp; // Zamjenjuje trenutni �or s podstabljem.
                return true;
            }

            // �or ima dva djeteta: zamjenjuje ga s minimalnim �orom iz desnog podstabla.
            MyBstNode<T>* temp = findMin(node->right);
            node->value = temp->value;
            return eraseRecursive(node->right, temp->value);
        }

        node = balance(node); // Balansira stablo nakon brisanja.
        return true;
    }

    // Pronalazi minimalni �vor u stablu (najmanju vrijednost).
    MyBstNode<T>* findMin(MyBstNode<T>* node) const {
        while (node->left != nullptr) {
            node = node->left; // Ide u lijevo dok ne do�e do najni�eg �vora.
        }
        return node;
    }

    // Rekurzivno bri�e cijelo stablo.
    void deleteTree(MyBstNode<T>*& node) {
        if (node == nullptr) return;
        deleteTree(node->left); // Bri�e lijevo podstablo.
        deleteTree(node->right); // Bri�e desno podstablo.
        delete node; // Bri�e trenutni �vor.
        node = nullptr;
    }

    // Inorder prolazak kroz stablo i ispis elemenata.
    void inorderTraversal(MyBstNode<T>* node) const {
        if (node == nullptr) return;
        inorderTraversal(node->left); // Lijevo podstablo.
        cout << node->value << " "; // Ispis vrijednosti trenutnog �vora.
        inorderTraversal(node->right); // Desno podstablo.
    }

    // Rekurzivno pretra�ivanje vrijednosti u stablu.
    bool searchRecursive(MyBstNode<T>* node, const T& value) const {
        if (node == nullptr) {
            return false; // Vrijednost nije prona�ena.
        }
        if (isEqual(value, node->value)) {
            return true; // Vrijednost prona�ena.
        }
        if (isLess(value, node->value)) {
            return searchRecursive(node->left, value); // Tra�i u lijevom podstablu.
        }
        else {
            return searchRecursive(node->right, value); // Tra�i u desnom podstablu.
        }
    }

    int left_rotate_counter = 0; // Broja� rotacija ulijevo.
    int right_rotate_counter = 0; // Broja� rotacija udesno.

public:
    MyBstSet(int threshold = 1) : imbalance_threshold(threshold) {
        collection_name = "MyBstSet (sa rekurzijom), imbalance_threshold: " + to_string(imbalance_threshold);
    }

    ~MyBstSet() {
        deleteTree(root); // Bri�e cijelo stablo prilikom uni�tenja objekta.
    }

    string get_collection_name() override {
        return this->collection_name;
    }

    bool add(const T& value) {
        return addRecursive(root, value); // Dodaje vrijednost u stablo.
    }

    bool erase(const T& value) {
        return eraseRecursive(root, value); // Bri�e vrijednost iz stabla.
    }

    bool search(const T& value) const override {
        return searchRecursive(root, value); // Pretra�uje vrijednost u stablu.
    }

    int count() const override {
        return nodeCount; // Vra�a broj �vorova u stablu.
    }

    bool empty() const override {
        return root == nullptr; // Provjerava je li stablo prazno.
    }

    void print(int take = 100, int skip = 0) const override {
        cout << "Inorder ispis: ";
        inorderTraversal(root); // Ispisuje elemente u inorder poretku.
        cout << endl;
    }

    void info() const override {
        cout << "left counter of balancing: " << left_rotate_counter << endl;
        cout << "right counter of balancing: " << right_rotate_counter << endl;
    }
};
