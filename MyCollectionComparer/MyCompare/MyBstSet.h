#pragma once

#include <iostream>
#include <functional>
using namespace std;
#include "MyComparator.h"
#include "MyBstNode.h"

// Klasa za implementaciju skupa koristeæi AVL stablo (balansirano binarno stablo pretraživanja).
// https://www.programiz.com/dsa/avl-tree
// T predstavlja generièki tip podataka koji se koristi unutar stabla.
template <class T>
class MyBstSet : public MyBaseCollection<T> {
private:
    string collection_name;
    const int imbalance_threshold; // Maksimalno dozvoljena neuravnoteženost èvora prije balansiranja.
    MyBstNode<T>* root = nullptr; // Pokazivaè na korijenski èvor stabla (poèetna taèka).
    int nodeCount = 0; // Ukupan broj èvorova u stablu.

    // Funkcija koja vraæa visinu datog èvora. 
    // Ako èvor ne postoji (nullptr), visina je 0.
    int getHeight(MyBstNode<T>* node) const {
        return node ? node->height : 0;
    }

    // Funkcija koja ažurira visinu èvora na osnovu visina njegovih podstabala.
    // Visina je definisana kao maksimalna visina lijevog ili desnog podstabla plus 1.
    void updateHeight(MyBstNode<T>* node) {
        if (node) {
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        }
    }

    // Funkcija koja raèuna balans faktor za èvor.
    // Balans faktor je razlika izmeðu visine lijevog i desnog podstabla.
    // Ako je balans faktor izvan opsega [-1, 1], èvor je neuravnotežen.
    int getBalanceFactor(MyBstNode<T>* node) const {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    // Funkcija za rotaciju èvora udesno, što je jedan od osnovnih koraka balansiranja stabla.
    // Koristi se u sluèajevima kada je lijevo podstablo veæe (visina veæa) od desnog.
    MyBstNode<T>* rotateRight(MyBstNode<T>* y) {
        MyBstNode<T>* x = y->left; // Novi korijenski èvor nakon rotacije.
        MyBstNode<T>* t = x->right; // Privremeno èuva desno podstablo èvora x.

        // Izvršava rotaciju.
        x->right = y; // Premješta y kao desno dijete èvora x.
        y->left = t; // Postavlja t kao lijevo dijete èvora y.

        // Ažurira visine èvorova nakon rotacije.
        updateHeight(y);
        updateHeight(x);
        right_rotate_counter++; // Poveæava brojaè rotacija udesno za statistièke svrhe.
        return x; // Vraæa novi korijenski èvor.
    }

    // Funkcija za rotaciju èvora ulijevo, suprotna operacija od rotacije udesno.
    // Koristi se kada je desno podstablo veæe od lijevog.
    MyBstNode<T>* rotateLeft(MyBstNode<T>* x) {
        MyBstNode<T>* y = x->right; // Novi korijenski èvor nakon rotacije.
        MyBstNode<T>* t = y->left; // Privremeno èuva lijevo podstablo èvora y.

        // Izvršava rotaciju.
        y->left = x; // Premješta x kao lijevo dijete èvora y.
        x->right = t; // Postavlja t kao desno dijete èvora x.

        // Ažurira visine èvorova nakon rotacije.
        updateHeight(x);
        updateHeight(y);
        left_rotate_counter++; // Poveæava brojaè rotacija ulijevo za statistièke svrhe.
        return y; // Vraæa novi korijenski èvor.
    }

    // Funkcija za balansiranje datog èvora.
    // Ako èvor ima balans faktor izvan opsega [-1, 1], izvršava se odgovarajuæa rotacija.
    MyBstNode<T>* balance(MyBstNode<T>* node) {
        updateHeight(node); // Ažurira visinu trenutnog èvora.

        int balanceFactor = getBalanceFactor(node); // Dobija balans faktor èvora.

        // Lijevo-lijevo sluèaj: lijevo podstablo je preveliko, a njegov balans faktor je takoðer pozitivan.
        if (balanceFactor > imbalance_threshold && getBalanceFactor(node->left) >= 0) {
            return rotateRight(node);
        }

        // Lijevo-desno sluèaj: lijevo podstablo je preveliko, ali njegov balans faktor je negativan.
        // Potrebna je rotacija ulijevo pa udesno.
        if (balanceFactor > imbalance_threshold && getBalanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Desno-desno sluèaj: desno podstablo je preveliko, a njegov balans faktor je takoðer negativan.
        if (balanceFactor < -imbalance_threshold && getBalanceFactor(node->right) <= 0) {
            return rotateLeft(node);
        }

        // Desno-lijevo sluèaj: desno podstablo je preveliko, ali njegov balans faktor je pozitivan.
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
            node = new MyBstNode<T>(value); // Kreira novi èor ako je mjesto dodavanja pronaðeno.
            return true; // Dodavanje uspješno.
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

        return false; // Vrijednost veæ postoji u stablu, ne dodaje se.
    }



    // Funkcija za brisanje èvora iz stabla uz balansiranje.
    bool eraseRecursive(MyBstNode<T>*& node, const T& value) {
        if (node == nullptr) {
            return false; // Èor nije pronaðen.
        }

        if (value < node->value) {
            return eraseRecursive(node->left, value); // Traži èor u lijevom podstablu.
        }
        else if (value > node->value) {
            return eraseRecursive(node->right, value); // Traži èor u desnom podstablu.
        }
        else {
            // Èor za brisanje je pronaðen.
            if (node->left == nullptr || node->right == nullptr) {
                MyBstNode<T>* temp = node->left ? node->left : node->right;
                delete node; // Briše trenutni èor.
                node = temp; // Zamjenjuje trenutni èor s podstabljem.
                return true;
            }

            // Èor ima dva djeteta: zamjenjuje ga s minimalnim èorom iz desnog podstabla.
            MyBstNode<T>* temp = findMin(node->right);
            node->value = temp->value;
            return eraseRecursive(node->right, temp->value);
        }

        node = balance(node); // Balansira stablo nakon brisanja.
        return true;
    }

    // Pronalazi minimalni èvor u stablu (najmanju vrijednost).
    MyBstNode<T>* findMin(MyBstNode<T>* node) const {
        while (node->left != nullptr) {
            node = node->left; // Ide u lijevo dok ne doðe do najnižeg èvora.
        }
        return node;
    }

    // Rekurzivno briše cijelo stablo.
    void deleteTree(MyBstNode<T>*& node) {
        if (node == nullptr) return;
        deleteTree(node->left); // Briše lijevo podstablo.
        deleteTree(node->right); // Briše desno podstablo.
        delete node; // Briše trenutni èvor.
        node = nullptr;
    }

    // Inorder prolazak kroz stablo i ispis elemenata.
    void inorderTraversal(MyBstNode<T>* node) const {
        if (node == nullptr) return;
        inorderTraversal(node->left); // Lijevo podstablo.
        cout << node->value << " "; // Ispis vrijednosti trenutnog èvora.
        inorderTraversal(node->right); // Desno podstablo.
    }

    // Rekurzivno pretraživanje vrijednosti u stablu.
    bool searchRecursive(MyBstNode<T>* node, const T& value) const {
        if (node == nullptr) {
            return false; // Vrijednost nije pronaðena.
        }
        if (isEqual(value, node->value)) {
            return true; // Vrijednost pronaðena.
        }
        if (isLess(value, node->value)) {
            return searchRecursive(node->left, value); // Traži u lijevom podstablu.
        }
        else {
            return searchRecursive(node->right, value); // Traži u desnom podstablu.
        }
    }

    int left_rotate_counter = 0; // Brojaè rotacija ulijevo.
    int right_rotate_counter = 0; // Brojaè rotacija udesno.

public:
    MyBstSet(int threshold = 1) : imbalance_threshold(threshold) {
        collection_name = "MyBstSet (sa rekurzijom), imbalance_threshold: " + to_string(imbalance_threshold);
    }

    ~MyBstSet() {
        deleteTree(root); // Briše cijelo stablo prilikom uništenja objekta.
    }

    string get_collection_name() override {
        return this->collection_name;
    }

    bool add(const T& value) {
        return addRecursive(root, value); // Dodaje vrijednost u stablo.
    }

    bool erase(const T& value) {
        return eraseRecursive(root, value); // Briše vrijednost iz stabla.
    }

    bool search(const T& value) const override {
        return searchRecursive(root, value); // Pretražuje vrijednost u stablu.
    }

    int count() const override {
        return nodeCount; // Vraæa broj èvorova u stablu.
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
