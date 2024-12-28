#pragma once
// Klasa za èvor stabla (MyBstNode)
template <class T>
class MyBstNode {
public:
    T value;
    MyBstNode* left;
    MyBstNode* right;
    int height; // Dodana visina za optimizaciju

    MyBstNode(const T& value) : value(value), left(nullptr), right(nullptr), height(1) {}
};