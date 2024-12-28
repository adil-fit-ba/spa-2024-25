#pragma once


// Predlo�ak funkcije za zadanu hash funkciju koja podr�ava razli�ite tipove podataka.
// T - generi�ki tip podatka (mo�e biti cijeli broj, broj s pomi�nim zarezom, string, itd.)
// bucket_size - broj kanti (npr. za hash tablicu)

template<class T>
int my_default_hash_function(const T& x, int bucket_size) {

    // Provjerava se tip podataka pomo�u 'constexpr if'. Ovo omogu�ava razli�ito pona�anje
    // ovisno o tome koji je tip podatka proslije�en u funkciju.

    if constexpr (is_integral<T>::value) {
        // Ako je tip cijeli broj (npr. int, long, short itd.)
        // vra�a ostatak dijeljenja broja 'x' s veli�inom 'bucket_size'.
        // Ovo osigurava da vrijednost hash funkcije bude unutar raspona 0 do bucket_size - 1.
        return x % bucket_size;
    }
    else if constexpr (is_floating_point<T>::value) {
        // Ako je tip podatka broj s pomi�nim zarezom (npr. float, double),
        // pretvara broj u cijeli broj pomo�u 'static_cast<int>' i ra�una hash.
        // Napomena: Ova metoda mo�e izgubiti preciznost za velike brojeve s pomi�nim zarezom.
        return static_cast<int>(x) % bucket_size;
    }
    else if constexpr (is_same<T, string>::value) {
        // Ako je tip podatka string:
        // 1. Pokre�e petlju kroz svaki znak stringa.
        // 2. Svaki znak doprinosi vrijednosti hash-a koriste�i formulu:
        //    hash = (hash * 31 + znak) % bucket_size.
        //    (31 je �esto kori�teni faktor u hashiranju stringova zbog dobre distribucije.)
        int hash = 0;
        for (char c : x) {
            hash = (hash * 31 + c) % bucket_size;
        }
        return hash;
    }
    else {
        // Ako tip podatka nije podr�an, baca izuzetak s opisom gre�ke.
        // Ovo osigurava da program ne nastavi raditi s nepodr�anim tipovima.
        throw exception("Hash funkcija nije podr�ana za ovaj tip.");
    }
};

/*
Detaljna obja�njenja:
#pragma once:
    Spre�ava vi�estruko uklju�ivanje datoteke (efikasnija alternativa #ifndef/#define).

    
if constexpr:
    Koristi se za odabir grane tokom kompajliranja, na osnovu tipa T.
    Ovo pove�ava efikasnost jer se nepotrebni kod ne kompajlira.


is_integral<T>::value:
    Provjerava je li T cijeli brojni tip (int, long, short, itd.).
    
    
is_floating_point<T>::value:
    Provjerava je li T broj s pomi�nim zarezom (float, double).

    
is_same<T, string>::value:
    Provjerava je li T string.

    
String hash funkcija:
    Koristi kombinaciju znakova i broja 31 kako bi kreirala hash vrijednost.
    Ova metoda osigurava dobru raspodjelu hash vrijednosti za stringove.
*/


/**
Pitanja za diskusiju sa studentima:
1 Za�to koristimo 31 kao faktor u hashiranju stringova?
2 Koji su potencijalni problemi s hashiranjem brojeva s pomi�nim zarezom?
3 Kako bi pro�irili funkciju da podr�ava korisni�ki definirane tipove (npr. strukture)?


*/