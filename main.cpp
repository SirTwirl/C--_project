#include "Typy.hpp"
#include "Usluga.hpp"
#include "KatalogUslug.hpp"
#include "Pracownik.hpp"
#include "Zgloszenie.hpp"
#include "IAlgorytm.hpp"
#include "AlgorytmA.hpp"
#include "AlgorytmB.hpp"
#include "AlgorytmC.hpp"
#include "AlgorytmAging.hpp"
#include "input.hpp"
#include "Output.hpp"
#include "Statystyki.hpp"
#include "SilnikSymulacji.hpp"
#include "GeneratorZgloszen.hpp"

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <filesystem>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

static unique_ptr<IAlgorytm> stworzAlgorytm(TypAlgorytmu typ) {
    switch (typ) {
        case TypAlgorytmu::FIFO:
            return make_unique<AlgorytmA>();
        case TypAlgorytmu::Priorytetowy:
            return make_unique<AlgorytmB>();
        case TypAlgorytmu::EDF:
            return make_unique<AlgorytmC>();
        case TypAlgorytmu::PriorytetAging:
            return make_unique<AlgorytmAging>(15);
        default:
            return make_unique<AlgorytmA>();
    }
}

static KatalogUslug zbudujKatalog(const vector<UslugaInfo>& infoUslugi) {
    KatalogUslug kat;
    for (const auto& u : infoUslugi) {
        kat.dodajUsluge(Usluga(u.id, u.nazwa, u.kategoria, u.specjalizacja, u.czas, u.priorytet));
    }
    return kat;
}

static void uruchomPojedynczaSymulacje(const string& sciezkaKonfig,
                                      const string& sciezkaZgloszenia,
                                      const string& sciezkaWyjscia,
                                      TypAlgorytmu wybranyTyp) {
    KonfiguracjaData konfig;
    if (!Input::wczytajKonfiguracje(sciezkaKonfig, konfig)) {
        cerr << "Blad odczytu pliku konfiguracji!" << endl;
        return;
    }

    vector<ZgloszenieInfo> zgloszeniaInfo;
    if (!Input::wczytajZgloszenia(sciezkaZgloszenia, zgloszeniaInfo)) {
        cerr << "Blad odczytu pliku zgloszen!" << endl;
        return;
    }

    KatalogUslug katalog = zbudujKatalog(konfig.uslugi);
    SilnikSymulacji silnik(konfig.maxCzas);
    silnik.zainicjalizujZKatalogu(katalog, konfig.pracownicy);
    silnik.wczytajZgloszeniaZInfo(zgloszeniaInfo);
    silnik.ustawAlgorytm(stworzAlgorytm(wybranyTyp));

    cout << "\n>>> Start symulacji: " << typAlgorytmuNaString(wybranyTyp) << " <<<\n";
    auto tStart = chrono::high_resolution_clock::now();
    silnik.uruchom(false);
    auto tEnd = chrono::high_resolution_clock::now();
    double czasWykonaniaMs = chrono::duration<double, milli>(tEnd - tStart).count();

    const auto& stats = silnik.getStatystyki();
    Output::wyswietlWKonsoli(stats);
    Output::zapiszRaportDoPliku(sciezkaWyjscia, stats);

    cout << "[INFO] Wyniki zapisano do pliku: " << sciezkaWyjscia << "\n";
    cout << "[PERFORMANCE] Czas wykonania symulacji w C++: " << czasWykonaniaMs << " ms\n\n";
}

static void uruchomPelnePorownanie(const string& sciezkaKonfig,
                                  const string& sciezkaZgloszenia,
                                  const string& sciezkaWyjsciaRaport,
                                  const string& sciezkaWyjsciaCsv) {
    KonfiguracjaData konfig;
    if (!Input::wczytajKonfiguracje(sciezkaKonfig, konfig)) return;

    vector<ZgloszenieInfo> zgloszeniaInfo;
    if (!Input::wczytajZgloszenia(sciezkaZgloszenia, zgloszeniaInfo)) return;

    KatalogUslug katalog = zbudujKatalog(konfig.uslugi);
    SilnikSymulacji silnik(konfig.maxCzas);
    silnik.zainicjalizujZKatalogu(katalog, konfig.pracownicy);
    silnik.wczytajZgloszeniaZInfo(zgloszeniaInfo);

    vector<TypAlgorytmu> strategie = {
        TypAlgorytmu::FIFO,
        TypAlgorytmu::Priorytetowy,
        TypAlgorytmu::EDF,
        TypAlgorytmu::PriorytetAging
    };

    vector<Statystyki> wynikiPorownania;

    cout << "\n================================================================================\n";
    cout << "  ROZPOCZECIE POROWNANIA EKSPERYMENTALNEGO WSZYSTKICH STRATEGII HARMONOGRAMOWANIA \n";
    cout << "================================================================================\n";

    for (auto typ : strategie) {
        silnik.ustawAlgorytm(stworzAlgorytm(typ));
        auto tStart = chrono::high_resolution_clock::now();
        silnik.uruchom(false);
        auto tEnd = chrono::high_resolution_clock::now();
        double ms = chrono::duration<double, milli>(tEnd - tStart).count();

        wynikiPorownania.push_back(silnik.getStatystyki());
        cout << " -> " << typAlgorytmuNaString(typ) << " wykonano w " << ms << " ms\n";
    }

    Output::wyswietlTabelePorownawcza(wynikiPorownania);
    Output::zapiszPorownanieDoPliku(sciezkaWyjsciaRaport, wynikiPorownania);
    Output::eksportujDoCsv(sciezkaWyjsciaCsv, wynikiPorownania);

    cout << "[INFO] Zbiorczy raport zapisano do: " << sciezkaWyjsciaRaport << "\n";
    cout << "[INFO] Dane CSV wyeksportowano do:  " << sciezkaWyjsciaCsv << "\n\n";
}

static void uruchomKompleksoweBadania(const string& sciezkaKonfig) {
    KonfiguracjaData konfig;
    if (!Input::wczytajKonfiguracje(sciezkaKonfig, konfig)) return;

    KatalogUslug katalog = zbudujKatalog(konfig.uslugi);
    GeneratorZgloszen generator(1337);

    struct ScenariuszBadawczy {
        string nazwa;
        ProfilObciazenia profil;
        size_t liczbaZgloszen;
        int maxCzas;
    };

    vector<ScenariuszBadawczy> scenariusze = {
        {"Niskie Obciazenie (Underloaded)", ProfilObciazenia::Niskie, 25, 480},
        {"Zrownowazone Obciazenie (Optimal)", ProfilObciazenia::Zrownowazone, 65, 480},
        {"Przeciazenie Systemu (Overloaded)", ProfilObciazenia::Przeciazenie, 160, 480},
        {"Skokowy Naplyw (Burst / Fala)", ProfilObciazenia::SkokoweBurst, 120, 480},
        {"Wielka Skala Big-Data (N=10000)", ProfilObciazenia::DuzaSkala10k, 10000, 20000}
    };

    vector<TypAlgorytmu> algorytmy = {
        TypAlgorytmu::FIFO,
        TypAlgorytmu::Priorytetowy,
        TypAlgorytmu::EDF,
        TypAlgorytmu::PriorytetAging
    };

    cout << "\n========================================================================================\n";
    cout << "                 URUCHAMIANIE KOMPLEKSOWYCH BADAN EKSPERYMENTALNYCH                     \n";
    cout << "      (Weryfikacja wlasnosci statystycznych, glodzenia i zlozonosci obliczeniowej)       \n";
    cout << "========================================================================================\n";

    vector<Statystyki> wszystkieZbiorczeWyniki;

    for (const auto& sc : scenariusze) {
        cout << "\n>>> SCENARIUSZ: " << sc.nazwa << " [N = " << sc.liczbaZgloszen
             << ", Horyzont = " << sc.maxCzas << " min] <<<\n";

        auto zgloszenia = generator.generuj(sc.liczbaZgloszen, katalog, sc.profil, sc.maxCzas);
        SilnikSymulacji silnik(sc.maxCzas);
        silnik.zainicjalizujZKatalogu(katalog, konfig.pracownicy);
        silnik.wczytajZgloszeniaZInfo(zgloszenia);

        vector<Statystyki> wynikiScenariusza;
        for (auto algTyp : algorytmy) {
            silnik.ustawAlgorytm(stworzAlgorytm(algTyp));
            silnik.uruchom(false);
            wynikiScenariusza.push_back(silnik.getStatystyki());
            wszystkieZbiorczeWyniki.push_back(silnik.getStatystyki());
        }

        Output::wyswietlTabelePorownawcza(wynikiScenariusza);
    }

    Output::zapiszPorownanieDoPliku("wyniki_eksperymentow.txt", wszystkieZbiorczeWyniki);
    Output::eksportujDoCsv("wyniki_eksperymentow.csv", wszystkieZbiorczeWyniki);

    cout << "\n[SUKCES] Zakonczono wszystkie badania eksperymentalne!\n";
    cout << "Raport szczegolowy: wyniki_eksperymentow.txt\n";
    cout << "Dane pomiarowe CSV: wyniki_eksperymentow.csv\n\n";
}

int main(int argc, char* argv[]) {
    cout << "====================================================================\n";
    cout << "    SYMULATOR SYSTEMU OBSLUGI ZGLOSZEN W CZASIE DYSKRETNYM          \n";
    cout << "            Kontekst: Serwis Informatyczny (C++20)                  \n";
    cout << "====================================================================\n\n";

    const string plikKonfig = "konfiguracja.txt";
    const string plikZgloszen = "zgloszenia.txt";
    const string plikWynikow = "wyniki.txt";

    if (!fs::exists(plikZgloszen)) {
        cout << "[INFO] Plik " << plikZgloszen << " nie istnieje. Generowanie danych domyslnych...\n";
        KonfiguracjaData kData;
        if (Input::wczytajKonfiguracje(plikKonfig, kData)) {
            KatalogUslug kat = zbudujKatalog(kData.uslugi);
            GeneratorZgloszen gen(2026);
            auto domyslne = gen.generuj(60, kat, ProfilObciazenia::Zrownowazone, kData.maxCzas);
            GeneratorZgloszen::zapiszDoPliku(plikZgloszen, domyslne);
            cout << "[INFO] Wygenerowano domyslny strumien 60 zgloszen w: " << plikZgloszen << "\n\n";
        }
    }

    if (argc > 1) {
        string flaga = argv[1];
        if (flaga == "--porownaj" || flaga == "-c") {
            uruchomPelnePorownanie(plikKonfig, plikZgloszen, plikWynikow, "wyniki_porownanie.csv");
            return 0;
        } else if (flaga == "--eksperymenty" || flaga == "-e") {
            uruchomKompleksoweBadania(plikKonfig);
            return 0;
        } else if (flaga == "--generuj" || flaga == "-g") {
            size_t n = (argc > 2) ? stoul(argv[2]) : 100;
            string plikWyj = (argc > 3) ? argv[3] : "zgloszenia.txt";
            KonfiguracjaData kData;
            if (Input::wczytajKonfiguracje(plikKonfig, kData)) {
                KatalogUslug kat = zbudujKatalog(kData.uslugi);
                GeneratorZgloszen gen(42);
                auto z = gen.generuj(n, kat, ProfilObciazenia::Zrownowazone, kData.maxCzas);
                GeneratorZgloszen::zapiszDoPliku(plikWyj, z);
                cout << "[SUKCES] Wygenerowano " << n << " zgloszen do " << plikWyj << "\n";
            }
            return 0;
        }
    }

    while (true) {
        cout << "MENU GLOWNE:\n";
        cout << " 1. Uruchom symulacje dla Strategii A (FIFO / FCFS)\n";
        cout << " 2. Uruchom symulacje dla Strategii B (HPF - Highest Priority First)\n";
        cout << " 3. Uruchom symulacje dla Strategii C (EDF - Earliest Deadline First)\n";
        cout << " 4. Uruchom symulacje dla Strategii D (HPF z Aging - Przeciw glodzeniu)\n";
        cout << " 5. Porownaj wszystkie 4 strategie na bieżących zgloszeniach\n";
        cout << " 6. Wygeneruj nowe zgloszenia (Niskie / Zrownowazone / Przeciazenie / 10k)\n";
        cout << " 7. Uruchom kompleksowe badania eksperymentalne (wymog na 5.0)\n";
        cout << " 0. Zakoncz program\n";
        cout << "\nWybierz opcje [0-7]: ";

        int opcja;
        if (!(cin >> opcja)) {
            break;
        }

        if (opcja == 0) {
            cout << "Do widzenia!\n";
            break;
        }

        switch (opcja) {
            case 1:
                uruchomPojedynczaSymulacje(plikKonfig, plikZgloszen, plikWynikow, TypAlgorytmu::FIFO);
                break;
            case 2:
                uruchomPojedynczaSymulacje(plikKonfig, plikZgloszen, plikWynikow, TypAlgorytmu::Priorytetowy);
                break;
            case 3:
                uruchomPojedynczaSymulacje(plikKonfig, plikZgloszen, plikWynikow, TypAlgorytmu::EDF);
                break;
            case 4:
                uruchomPojedynczaSymulacje(plikKonfig, plikZgloszen, plikWynikow, TypAlgorytmu::PriorytetAging);
                break;
            case 5:
                uruchomPelnePorownanie(plikKonfig, plikZgloszen, plikWynikow, "wyniki_porownanie.csv");
                break;
            case 6: {
                cout << "\nWybierz profil obciazenia:\n";
                cout << " 1. Niskie (Underloaded)\n";
                cout << " 2. Zrownowazone (Optimal)\n";
                cout << " 3. Przeciazenie (Overloaded)\n";
                cout << " 4. Skokowy naplyw (Burst)\n";
                cout << " 5. Wielka skala (N = 10000 zgloszen)\n";
                cout << "Wybierz [1-5]: ";
                int p;
                cin >> p;
                ProfilObciazenia prof = ProfilObciazenia::Zrownowazone;
                size_t n = 60;
                int horyzont = 480;
                if (p == 1) { prof = ProfilObciazenia::Niskie; n = 25; }
                else if (p == 2) { prof = ProfilObciazenia::Zrownowazone; n = 65; }
                else if (p == 3) { prof = ProfilObciazenia::Przeciazenie; n = 150; }
                else if (p == 4) { prof = ProfilObciazenia::SkokoweBurst; n = 120; }
                else if (p == 5) { prof = ProfilObciazenia::DuzaSkala10k; n = 10000; horyzont = 20000; }

                KonfiguracjaData kData;
                if (Input::wczytajKonfiguracje(plikKonfig, kData)) {
                    KatalogUslug kat = zbudujKatalog(kData.uslugi);
                    GeneratorZgloszen gen(100);
                    auto z = gen.generuj(n, kat, prof, horyzont);
                    GeneratorZgloszen::zapiszDoPliku(plikZgloszen, z);
                    cout << "[SUKCES] Wygenerowano " << n << " zgloszen do " << plikZgloszen << "\n\n";
                }
                break;
            }
            case 7:
                uruchomKompleksoweBadania(plikKonfig);
                break;
            default:
                cout << "Niepoprawna opcja.\n";
                break;
        }
    }

    return 0;
}
