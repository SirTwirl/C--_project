#ifndef INPUT_HPP
#define INPUT_HPP

#include <string>
#include <vector>

using namespace std;

struct UslugaInfo {
    string id;
    string nazwa;
    string kategoria;
    string specjalizacja;
    int czas{0};
    int priorytet{1};
};

struct PracownikInfo {
    int id{0};
    string specjalizacja;
};

struct ZgloszenieInfo {
    int idZgloszenia{0};
    string idUslugi;
    string typUrzadzenia;
    string marka;
    int czasWejscia{0};
    int termin{0};
};

struct KonfiguracjaData {
    int algorytm{1};
    int maxCzas{480};
    vector<PracownikInfo> pracownicy;
    vector<UslugaInfo> uslugi;
};

class Input {
public:
    static bool wczytajKonfiguracje(const string& sciezka, KonfiguracjaData& outKonfig);
    static bool wczytajZgloszenia(const string& sciezka, vector<ZgloszenieInfo>& outZgloszenia);
};

#endif