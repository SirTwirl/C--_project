# PRZEWODNIK PO KODZIE I SZYBKICH MODYFIKACJACH NA OBRONĘ (OCENA 5.0)

Przewodnik przygotowany pod kątem kryterium oceny:
> **"Student sprawnie odpowiada na pytania podczas obrony, potrafi uzasadnić dobór struktur danych i bez problemu modyfikuje drobne elementy kodu na życzenie prowadzącego."**

---

## SPIS TREŚCI
1. [Jak szybko modyfikować i testować kod w 15 sekund](#1-jak-szybko-modyfikować-i-testować-kod-w-15-sekund)
2. [Omówienie każdego pliku + gotowe scenariusze modyfikacji na żywo](#2-omówienie-każdego-pliku--gotowe-scenariusze-modyfikacji-na-żywo)
   - [Typy.hpp](#typyhpp)
   - [Usluga.hpp i Usluga.cpp](#uslugahpp-i-uslugacpp)
   - [KatalogUslug.hpp i KatalogUslug.cpp](#kataloguslughpp-i-kataloguslugcpp)
   - [Zgloszenie.hpp i Zgloszenie.cpp](#zgloszeniehpp-i-zgloszeniecpp)
   - [Pracownik.hpp i Pracownik.cpp](#pracownikhpp-i-pracownikcpp)
   - [IAlgorytm.hpp](#ialgorytmhpp)
   - [AlgorytmA.hpp i AlgorytmA.cpp (FIFO)](#algorytma-fifo)
   - [AlgorytmB.hpp i AlgorytmB.cpp (HPF - Priorytet)](#algorytmb-hpf)
   - [AlgorytmC.hpp i AlgorytmC.cpp (EDF - Deadline)](#algorytmc-edf)
   - [AlgorytmAging.hpp i AlgorytmAging.cpp (Aging)](#algorytmagin-aging)
   - [input.hpp i input.cpp](#input)
   - [Statystyki.hpp i Statystyki.cpp](#statystyki)
   - [SilnikSymulacji.hpp i SilnikSymulacji.cpp](#silniksymulacji)
   - [GeneratorZgloszen.hpp i GeneratorZgloszen.cpp](#generatorzgloszen)
   - [Output.hpp i Output.cpp](#output)
   - [main.cpp](#maincpp)
3. [Typowe zadania "na żywo", które może zadać prowadzący](#3-typowe-zadania-na-żywo-które-może-zadać-prowadzący)

---

## 1. Jak szybko modyfikować i testować kod w 15 sekund

Podczas obrony, gdy prowadzący powie: *"A co jeśli zmienimy X na Y?"*:
1. Wprowadzasz zmianę w odpowiednim pliku (zgodnie z poniższą ściągawką).
2. Zapisujesz plik (`Ctrl + S`).
3. W terminalu wpisujesz:
   ```powershell
   .\build.bat
   ```
4. Natychmiast uruchamiasz test:
   ```powershell
   .\symulator.exe --porownaj
   ```
   *Wynik pojawia się w konsoli w ułamku sekundy.*

---

## 2. Omówienie każdego pliku + gotowe modyfikacje na żywo

---

### `Typy.hpp`
* **Co robi:** Definiuje typy wyliczeniowe (`enum class`) używane w całym projekcie: `StanPracownika` (`Wolny`, `Zajety`), `StanZgloszenia` (`Oczekuje`, `WRealizacji`, `Zakonczone`, `Przeterminowane`), `TypAlgorytmu` oraz funkcje pomocnicze zamieniające enum na czytelny `string`.
* **Dlaczego `enum class`:** Silne typowanie w C++ (scoped enum) zapobiega przypadkowym niejawnym rzutowaniom na int.
* **Co prowadzący może chcieć zmienić:**
  - *"Dodaj nowy typ algorytmu, np. SJF (Shortest Job First)"*:
    ```cpp
    enum class TypAlgorytmu {
        FIFO = 1,
        Priorytetowy = 2,
        EDF = 3,
        PriorytetAging = 4,
        SJF = 5 // <-- DODAJEMY
    };
    ```
    I dopisujemy w `typAlgorytmuNaString`:
    ```cpp
    case TypAlgorytmu::SJF: return "SJF (Shortest Job First)";
    ```

---

### `Usluga.hpp` i `Usluga.cpp`
* **Co robi:** Reprezentuje pozycję w cenniku serwisu informatycznego (`id`, `nazwa`, `kategoria`, `wymaganaSpecjalizacja`, `domyslnyCzas`, `priorytet`).
* **Co prowadzący może chcieć zmienić:**
  - *"Dodaj pole cena/koszt do usługi"*:
    1. W `Usluga.hpp`:
       ```cpp
       int koszt; // w prywatnych
       // w konstruktorze: dodaj parametr int koszt
       int getKoszt() const noexcept { return koszt; }
       ```
    2. W `Usluga.cpp`:
       ```cpp
       Usluga::Usluga(..., int koszt) : ..., koszt(koszt) {}
       ```

---

### `KatalogUslug.hpp` i `KatalogUslug.cpp`
* **Co robi:** Magazyn definicji usług bazujący na `unordered_map<string, Usluga>`. Wyszukiwanie odbywa się w czasie średnim $\mathcal{O}(1)$.
* **Dlaczego `optional<Usluga>`:** Wymóg na ocenę 5.0. Jeśli ID usługi nie istnieje w katalogu, metoda zwraca `nullopt`, eliminując potrzebę rzucania wyjątków czy zwracania niebezpiecznych wskaźników `nullptr`.
* **Co prowadzący może chcieć zmienić:**
  - *"Jak sprawdzić, czy katalog zawiera daną usługę?"*:
    Pokazujesz metodę `bool czyIstnieje(const string& id) const` lub użycie `if (optUsluga.has_value())`.

---

### `Zgloszenie.hpp` i `Zgloszenie.cpp`
* **Co robi:** Kluczowa klasa reprezentująca konkretne zlecenie naprawy. Zawiera dane o urządzeniu, czasie nadejścia, terminie SLA, czasie realizacji, priorytecie bazowym i dynamicznym oraz metody liczące:
  - `getCzasOczekiwania()`: $czasRozpoczecia - czasWejscia$
  - `czyDotrzymanoTerminu()`: $czasZakonczenia \le termin$
  - `getSpoznienie()`: $\max(0, czasZakonczenia - termin)$
* **Co prowadzący może chcieć zmienić:**
  - *"Zmień sposób naliczania spóźnienia (np. kara podwójna jeśli spóźnienie > 60 min)"*:
    W `Zgloszenie.hpp`:
    ```cpp
    int getSpoznienie() const noexcept {
        if (czasZakonczenia < 0) return 0;
        int diff = max(0, czasZakonczenia - termin);
        return (diff > 60) ? diff * 2 : diff;
    }
    ```
  - *"Dodaj pole 'czyVIP' (flaga priorytetowego klienta)"*:
    Dodajesz pole `bool czyVip{false};` i getter `bool isVip() const { return czyVip; }`.

---

### `Pracownik.hpp` i `Pracownik.cpp`
* **Co robi:** Reprezentuje serwisanta (`id`, `specjalizacja`, `stan`, wskaźnik do aktualnego zgłoszenia, licznik obsłużonych, łączny czas pracy).
* **Metoda kluczowa:** `getStopienWykorzystania(int calkowityCzas)` wylicza utylizację:
  $$\text{Utylizacja} = \frac{\text{lacznyCzasPracy}}{\text{calkowityCzas}} \times 100\%$$
* **Co prowadzący może chcieć zmienić:**
  - *"Pozwól pracownikowi o specjalizacji 'InzynierSieciIServerow' obsługiwać również zgłoszenia 'JuniorSoftware'"*:
    W `Pracownik.cpp` w metodzie `czyMozeObsluzyc`:
    ```cpp
    bool Pracownik::czyMozeObsluzyc(const string& wymagana) const noexcept {
        if (specjalizacja == wymagana) return true;
        if (specjalizacja == "InzynierSieciIServerow" && wymagana == "JuniorSoftware") return true;
        return false;
    }
    ```

---

### `IAlgorytm.hpp`
* **Co robi:** Czysto wirtualny interfejs wzorca **Strategia (Strategy Pattern)**.
* **Dlaczego wirtualny destruktor:** `virtual ~IAlgorytm() = default;` jest kluczowy, by usunięcie obiektu przez wskaźnik bazowy `unique_ptr<IAlgorytm>` poprawnie wywołało destruktor klasy pochodnej i zwolniło pamięć.

---

### `AlgorytmA` (FIFO)
* **Pliki:** `AlgorytmA.hpp` i `AlgorytmA.cpp`
* **Struktura:** `unordered_map<string, queue<shared_ptr<Zgloszenie>>> kolejki;`
* **Zasada:** Kolejka `queue` dla każdej specjalizacji. Zadania pobierane w kolejności FIFO w czasie $\mathcal{O}(1)$.
* **Co prowadzący może chcieć zmienić:**
  - *"Jak zmienić FIFO na LIFO (stos)?"*:
    Zamieniasz `queue` na `stack` i w `pobierzDlaPracownika` wywołujesz `it->second.top()` zamiast `.front()`.

---

### `AlgorytmB` (HPF)
* **Pliki:** `AlgorytmB.hpp` i `AlgorytmB.cpp`
* **Struktura:** `priority_queue<..., KomparatorPriorytetowy>`
* **Funktor komparatora:**
  ```cpp
  struct KomparatorPriorytetowy {
      bool operator()(const shared_ptr<Zgloszenie>& a, const shared_ptr<Zgloszenie>& b) const noexcept {
          if (a->getPriorytetBazowy() != b->getPriorytetBazowy()) {
              return a->getPriorytetBazowy() < b->getPriorytetBazowy(); // Max-heap: wiekszy priorytet na szczycie
          }
          return a->getCzasWejscia() > b->getCzasWejscia(); // Remis -> FIFO
      }
  };
  ```
* **Co prowadzący może chcieć zmienić:**
  - *"Odwróć priorytety (najmniejszy priorytet najpierw)"*:
    Zmień znak `<` na `>` w linii: `return a->getPriorytetBazowy() > b->getPriorytetBazowy();`.
  - *"W przypadku remisu wybierz zadanie o krótszym czasie trwania (SJF)"*:
    ```cpp
    if (a->getCzasObslugi() != b->getCzasObslugi()) {
        return a->getCzasObslugi() > b->getCzasObslugi(); // Krotszy czas na szczyt
    }
    ```

---

### `AlgorytmC` (EDF)
* **Pliki:** `AlgorytmC.hpp` i `AlgorytmC.cpp`
* **Struktura:** `priority_queue<..., KomparatorEDF>`
* **Funktor komparatora:** Min-heap dla terminu deadline:
  ```cpp
  return a->getTermin() > b->getTermin(); // Mniejszy termin ma pierwszenstwo!
  ```
* **Co prowadzący może chcieć zmienić:**
  - *"Uwzględnij margines bezpieczeństwa (Slack Time = termin - czasWejscia - czasObslugi)"*:
    ```cpp
    int slackA = a->getTermin() - a->getCzasObslugi();
    int slackB = b->getTermin() - b->getCzasObslugi();
    return slackA > slackB; // Najmniejszy luz czasowy na szczyt (Least Slack Time)
    ```

---

### `AlgorytmAging` (Aging)
* **Pliki:** `AlgorytmAging.hpp` i `AlgorytmAging.cpp`
* **Działanie:** Co `interwalAging` minut oczekiwania priorytet rośnie:
  $$P_{eff} = P_{bazowy} + \frac{\text{oczekiwanie}}{\text{interwalAging}}$$
* **Co prowadzący może chcieć zmienić:**
  - *"Zmień tempo starzenia z 15 minut na 5 minut"*:
    W konstruktorze lub w `main.cpp` zmień argument `interwalAging`:
    `return make_unique<AlgorytmAging>(5);`
  - *"Spraw, by zadania o priorytecie 1 starzały się dwa razy szybciej"*:
    W `AlgorytmAging.cpp`:
    ```cpp
    int mnoznik = (z->getPriorytetBazowy() == 1) ? 2 : 1;
    z->zwiekszPriorytetAging(bonus * mnoznik);
    ```

---

### `input.hpp` i `input.cpp`
* **Co robi:** Odczytuje pliki `konfiguracja.txt` oraz `zgloszenia.txt`. Posiada pełną odporność na błędy: ignoruje komentarze (`#`), puste linie oraz przechwytuje wyjątki `try ... catch (const exception& e)` przy błędnych danych liczbowych.
* **Co prowadzący może chcieć zmienić:**
  - *"Co jeśli plik używa przecinka `,` zamiast średnika `;` jako separatora?"*:
    W `input.cpp` w linii:
    `auto kolumny = podziel(linia, ';');`
    Zmień `';'` na `','`.

---

### `Statystyki.hpp` i `Statystyki.cpp`
* **Co robi:** Oblicza statystyki opisowe:
  - Średnia $\mu$, odchylenie standardowe $\sigma = \sqrt{\frac{1}{N}\sum (x_i - \mu)^2}$,
  - Percentyle $p_{50}$ (mediana), $p_{90}, p_{95}, p_{99}$ z interpolacją liniową,
  - Wskaźnik SLA %, średnie spóźnienie, stopień wykorzystania wykonawców.
* **Co prowadzący może chcieć zmienić:**
  - *"Dodaj obliczanie percentyla 75 (p75)"*:
    1. W `Statystyki.hpp`: dodaj `mutable double p75Oczekiwania{0.0};` i getter `double getP75Oczekiwania() const;`.
    2. W `Statystyki.cpp`:
       `p75Oczekiwania = obliczPercentyl(czasyOczekiwania, 75.0);`

---

### `SilnikSymulacji.hpp` i `SilnikSymulacji.cpp`
* **Co robi:** Główna pętla czasu dyskretnego ($t = 0, 1, 2, \dots$).
  1. Zwalnia pracowników kończących pracę w chwili $t$.
  2. Przyjmuje nowe zgłoszenia nadesłane w chwili $t$.
  3. Wywołuje `algorytm->aktualizuj(t)`.
  4. Przydziela wolnych techników do najlepszych zadań z kolejki.
* **Gwarancja powtarzalności:** Przed każdym startem silnik tworzy czyste kopie obiektów `Zgloszenie` i resetuje pracowników, dzięki czemu wszystkie strategie są testowane na identycznym stanie wejściowym.
* **Co prowadzący może chcieć zmienić:**
  - *"Spraw, by symulacja trwała 8 godzin (480 min) plus dogrywka aż do opróżnienia kolejki"*:
    W `main.cpp` wywołaj:
    `silnik.uruchom(true);` // flaga `dokonczWszystkieCzyDoLimitu = true`

---

### `GeneratorZgloszen.hpp` i `GeneratorZgloszen.cpp`
* **Co robi:** Generator losowy oparty na **procesie Poissona** (`std::exponential_distribution` dla odstępów czasu nadejść) oraz powtarzalnym ziarnie `seed` (`std::mt19937_64`).
* **Co prowadzący może chcieć zmienić:**
  - *"Zwiększ intensywność nadejść w scenariuszu zrównoważonym"*:
    W `GeneratorZgloszen.cpp` zmień: `double lambda = 0.15;` na np. `0.25;`.

---

### `Output.hpp` i `Output.cpp`
* **Co robi:** Formatuje i zapisuje raporty:
  - do pliku tekstowego `wyniki.txt`,
  - do pliku CSV `wyniki_porownanie.csv`,
  - wyświetla tabelę podsumowującą na ekranie.

---

### `main.cpp`
* **Co robi:** Punkt wejścia aplikacji.
  - Obsługuje menu interaktywne (opcje 1-7),
  - Obsługuje flagi CLI:
    - `--porownaj` / `-c` (porównanie 4 strategii),
    - `--eksperymenty` / `-e` (badania naukowe 5 scenariuszy),
    - `--generuj [N]` (generowanie zadanego pliku zgłoszeń).

---

## 3. Typowe zadania "na żywo", które może zadać prowadzący

### Zadanie A: "Dodajmy strategię SJF (Shortest Job First)"
1. W `Typy.hpp` dodaj `SJF` do `enum class TypAlgorytmu`.
2. W `AlgorytmB.hpp` zmień komparator na:
   ```cpp
   struct KomparatorSJF {
       bool operator()(const shared_ptr<Zgloszenie>& a, const shared_ptr<Zgloszenie>& b) const noexcept {
           if (a->getCzasObslugi() != b->getCzasObslugi()) {
               return a->getCzasObslugi() > b->getCzasObslugi(); // Krotszy czas ma pierwszenstwo!
           }
           return a->getCzasWejscia() > b->getCzasWejscia();
       }
   };
   ```
3. Zbuduj i uruchom: `.\build.bat`, `.\symulator.exe --porownaj`.

### Zadanie B: "Pokaż, że zmiana seeda daje inne, ale wciąż powtarzalne wyniki"
W `main.cpp` zmień ziarno w linii:
`GeneratorZgloszen gen(2026);` na `GeneratorZgloszen gen(9999);`.

### Zadanie C: "Spraw, by pracownik mógł obsłużyć zadanie tylko do 17:00 (czas 480)"
W `SilnikSymulacji.cpp` w KROKU 4 dodaj warunek:
```cpp
if (t + zgloszenie->getCzasObslugi() <= maxCzas) {
    p.przydzielZgloszenie(zgloszenie, t);
}
```

---
*Dokument ten pozwala na natychmiastową, spokojną reakcję i wykonanie każdej modyfikacji w czasie poniżej 1 minuty.*
