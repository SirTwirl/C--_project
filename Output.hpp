#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include "Statystyki.hpp"
#include <string>
#include <vector>

using namespace std;

class Output {
public:
    static bool zapiszRaportDoPliku(const string& sciezka, const Statystyki& statystyki);
    static bool zapiszPorownanieDoPliku(const string& sciezka, const vector<Statystyki>& listaStatystyk);
    static bool eksportujDoCsv(const string& sciezka, const vector<Statystyki>& listaStatystyk);
    static void wyswietlWKonsoli(const Statystyki& statystyki);
    static void wyswietlTabelePorownawcza(const vector<Statystyki>& listaStatystyk);
};

#endif
