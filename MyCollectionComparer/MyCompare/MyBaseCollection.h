#pragma once

#include <iostream>
#include <string>
using namespace std;

template <class Tip>
class MyBaseCollection {
protected:

public:
    virtual string get_collection_name() = 0;

    virtual bool add(const Tip& v) = 0;

    virtual bool erase(const Tip& v) = 0;

    virtual bool search(const Tip& v) const = 0;

    virtual int count() const = 0; 

    virtual bool empty() const = 0;

    virtual void print(int take = 100, int skip = 0) const = 0;

    virtual void info() const = 0;


};