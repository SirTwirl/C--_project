#ifndef USLUGA_HPP
#define USLUGA_HPP

#include <string>
#include <iostream>

using namespace std;

class Usluga {
private:
    string id;
    string nazwa;
    string kategoria;
    string wymaganaSpecjalizacja;
    int domyslnyCzas;
    int priorytet;

public:
    Usluga() = default;
    Usluga(string id, string nazwa, string kategoria,
           string wymaganaSpecjalizacja, int domyslnyCzas, int priorytet);

    const string& getId() const noexcept { return id; }
    const string& getNazwa() const noexcept { return nazwa; }
    const string& getKategoria() const noexcept { return kategoria; }
    const string& getWymaganaSpecjalizacja() const noexcept { return wymaganaSpecjalizacja; }
    int getDomyslnyCzas() const noexcept { return domyslnyCzas; }
    int getPriorytet() const noexcept { return priorytet; }

    friend ostream& operator<<(ostream& os, const Usluga& u);
};

#endif
