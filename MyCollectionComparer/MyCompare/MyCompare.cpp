#include <iostream>
#include <unordered_set>
#include <list>
#include <chrono> // Za mjerenje vremena
#include <random>

#include "CppHashSet.h"
#include "CppList.h"
#include "MyHashSet.h"
#include "MyList.h"
#include "MyOrderedHashSet1.h"
#include "MyOrderedHashSet2.h"
#include "CppOrderedBstSet.h"
#include "MyBstSet.h"

using namespace std;
using namespace chrono;


const int N = 5000 * 1000; // Broj elemenata

struct testing_data {
    string name;
    vector<int> data_vector;

    testing_data(const string& name, vector<int> data_vector)
        : name(name), data_vector(data_vector)
    {
    }
};

struct my_testing_result_item {
    testing_data data;
    int items_found = 0;
    long add_ms = 0;
    long search_ms = 0;
    long delete_ms = 0;

    my_testing_result_item(testing_data& data) :data(data) {

    }
};

struct my_testing_result {
    string collection_name;
    vector<my_testing_result_item> result_items;
    void print() {
        cout << "Rezultati testiranja za kolekciju: " << collection_name << endl;
        cout << "------------------------------------------------------------" << endl;

        for (const auto& item : result_items) {
            cout << "Ime testa: " << item.data.name << endl;
            cout << "Broj elemenata total: " << item.data.data_vector.size() << endl;
            cout << "Broj elemenata pronadenih: " << item.items_found << ", " << int(100* item.items_found / (float) item.data.data_vector.size()) <<"%" << endl;
            cout << "Vrijeme dodavanja (ms): " << item.add_ms << endl;
            cout << "Vrijeme pretrage (ms): " << item.search_ms << endl;
            cout << "Vrijeme brisanja (ms): " << item.delete_ms << endl;
            cout << endl;
            cout << "------------------------------------------------------------" << endl;
        }
    }
};


testing_data my_random_vector(int size, int minValue, int maxValue) {
    vector<int> data;
    data.reserve(size);

    random_device rd; // Nasumični uređaj
    mt19937 gen(rd()); // Mersenne Twister generator
    uniform_int_distribution<> distrib(minValue, maxValue);

    for (int i = 0; i < size; ++i) {
        data.push_back(distrib(gen));
    }

    return testing_data("random", data);
}

testing_data my_increasing_vector(int size, int startValue = 0) {
    vector<int> data ;
    data.reserve(size);

    for (int i = startValue; i < size; ++i) {
        data.push_back(i);
    }

    return testing_data("increasing", data);
}



template <class Tip>
my_testing_result test_my_collection(MyBaseCollection<Tip>* collection, vector<testing_data> data_vector) {

    my_testing_result testing_result;
    testing_result.collection_name = collection->get_collection_name();

    cout << "Pokrece testove za " << collection->get_collection_name() << endl << endl;

    for (int i=0; i< data_vector.size(); i++)
    {
        testing_data data = data_vector[i];
        my_testing_result_item result_item(data);

        // Dodavanje elemenata
        auto start = high_resolution_clock::now();
        for (size_t i = 0; i < data.data_vector.size(); ++i) {
            collection->add(data.data_vector[i]);
        }
        auto end = high_resolution_clock::now();
        result_item.add_ms = duration_cast<milliseconds>(end - start).count();

        // Pretraga elemenata
        result_item.items_found = 0;
        start = high_resolution_clock::now();
        for (size_t i = 0; i < data.data_vector.size(); ++i) {
            if (collection->search(data.data_vector[i]))
            {
                result_item.items_found++;
            }
        }
        end = high_resolution_clock::now();
        result_item.search_ms = duration_cast<milliseconds>(end - start).count();

        // Brisanje elemenata
        start = high_resolution_clock::now();

        int erased = 0;
        for (size_t i = 0; i < data.data_vector.size(); ++i) {
            if (collection->erase(i))
            {
                erased++;
            }
        }

        end = high_resolution_clock::now();
        result_item.delete_ms = duration_cast<milliseconds>(end - start).count();

        testing_result.result_items.push_back(result_item);
    }
    return testing_result;
}


int main() {
    vector<testing_data> testing_datas;

    testing_datas.push_back(my_random_vector(N, 1, N * 3));
//    testing_datas.push_back(my_increasing_vector(N, 0));
   
//    test_my_collection(new CppList<int>(), testing_datas).print();
//    test_my_collection(new MyList<int>(), testing_datas).print();

    test_my_collection(new CppHashSet<int>(), testing_datas).print();
    test_my_collection(new MyHashSet<int>(), testing_datas).print();
    test_my_collection(new MyBstSet<int>(), testing_datas).print();

    test_my_collection(new CppOrderedBstSet<int>(), testing_datas).print();
//    test_my_collection(new MyOrderedHashSet1<int>(), testing_datas).print();
    test_my_collection(new MyOrderedHashSet2<int>(), testing_datas).print();


    return 0;
}
