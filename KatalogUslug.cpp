#include "KatalogUslug.hpp"

using namespace std;

void KatalogUslug::dodajUsluge(const Usluga& usluga) {
    uslugi[usluga.getId()] = usluga;
}

optional<Usluga> KatalogUslug::pobierzUsluge(const string& id) const {
    auto it = uslugi.find(id);
    if (it != uslugi.end()) {
        return it->second;
    }
    return nullopt;
}

bool KatalogUslug::czyIstnieje(const string& id) const {
    return uslugi.find(id) != uslugi.end();
}

vector<Usluga> KatalogUslug::jakoWektor() const {
    vector<Usluga> lista;
    lista.reserve(uslugi.size());
    for (const auto& [id, usluga] : uslugi) {
        lista.push_back(usluga);
    }
    return lista;
}
