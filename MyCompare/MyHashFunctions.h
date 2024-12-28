#pragma once


// Predložak funkcije za zadanu hash funkciju koja podržava razlièite tipove podataka.
// T - generièki tip podatka (može biti cijeli broj, broj s pomiènim zarezom, string, itd.)
// bucket_size - broj kanti (npr. za hash tablicu)

template<class T>
int my_default_hash_function(const T& x, int bucket_size) {

    // Provjerava se tip podataka pomoæu 'constexpr if'. Ovo omoguæava razlièito ponašanje
    // ovisno o tome koji je tip podatka proslijeðen u funkciju.

    if constexpr (is_integral<T>::value) {
        // Ako je tip cijeli broj (npr. int, long, short itd.)
        // vraæa ostatak dijeljenja broja 'x' s velièinom 'bucket_size'.
        // Ovo osigurava da vrijednost hash funkcije bude unutar raspona 0 do bucket_size - 1.
        return x % bucket_size;
    }
    else if constexpr (is_floating_point<T>::value) {
        // Ako je tip podatka broj s pomiènim zarezom (npr. float, double),
        // pretvara broj u cijeli broj pomoæu 'static_cast<int>' i raèuna hash.
        // Napomena: Ova metoda može izgubiti preciznost za velike brojeve s pomiènim zarezom.
        return static_cast<int>(x) % bucket_size;
    }
    else if constexpr (is_same<T, string>::value) {
        // Ako je tip podatka string:
        // 1. Pokreæe petlju kroz svaki znak stringa.
        // 2. Svaki znak doprinosi vrijednosti hash-a koristeæi formulu:
        //    hash = (hash * 31 + znak) % bucket_size.
        //    (31 je èesto korišteni faktor u hashiranju stringova zbog dobre distribucije.)
        int hash = 0;
        for (char c : x) {
            hash = (hash * 31 + c) % bucket_size;
        }
        return hash;
    }
    else {
        // Ako tip podatka nije podržan, baca izuzetak s opisom greške.
        // Ovo osigurava da program ne nastavi raditi s nepodržanim tipovima.
        throw exception("Hash funkcija nije podržana za ovaj tip.");
    }
};

/*
Detaljna objašnjenja:
#pragma once:
    Spreèava višestruko ukljuèivanje datoteke (efikasnija alternativa #ifndef/#define).

    
if constexpr:
    Koristi se za odabir grane tokom kompajliranja, na osnovu tipa T.
    Ovo poveæava efikasnost jer se nepotrebni kod ne kompajlira.


is_integral<T>::value:
    Provjerava je li T cijeli brojni tip (int, long, short, itd.).
    
    
is_floating_point<T>::value:
    Provjerava je li T broj s pomiènim zarezom (float, double).

    
is_same<T, string>::value:
    Provjerava je li T string.

    
String hash funkcija:
    Koristi kombinaciju znakova i broja 31 kako bi kreirala hash vrijednost.
    Ova metoda osigurava dobru raspodjelu hash vrijednosti za stringove.
*/


/**
Pitanja za diskusiju sa studentima:
1 Zašto koristimo 31 kao faktor u hashiranju stringova?
2 Koji su potencijalni problemi s hashiranjem brojeva s pomiènim zarezom?
3 Kako bi proširili funkciju da podržava korisnièki definirane tipove (npr. strukture)?


*/