#ifndef KATALOGUSLUG_HPP
#define KATALOGUSLUG_HPP

#include "Usluga.hpp"
#include <unordered_map>
#include <string>
#include <optional>
#include <vector>

using namespace std;

class KatalogUslug {
private:
    unordered_map<string, Usluga> uslugi;

public:
    KatalogUslug() = default;

    void dodajUsluge(const Usluga& usluga);
    optional<Usluga> pobierzUsluge(const string& id) const;
    bool czyIstnieje(const string& id) const;
    size_t rozmiar() const noexcept { return uslugi.size(); }
    const unordered_map<string, Usluga>& pobierzWszystkie() const noexcept { return uslugi; }
    vector<Usluga> jakoWektor() const;
    void wyczysc() noexcept { uslugi.clear(); }
};

#endif
