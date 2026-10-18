# Gonjiam: Ghost Hunt

> **Walking Simulator Horror** în C++ axat pe atmosferă și presiune psihologică.

---

##  Concept & Game Design (GDD)

### Concept
Jucătorul este un **YouTuber** (stil *Dima Maslenikov*) care explorează **Spitalul Psihiatric Gonjiam** din Coreea de Sud pentru a demonstra într-un videoclip că zvonurile despre activitățile paranormale sunt false.

Acțiunea începe noaptea, în pădurea spre spital. În timp ce înaintează prin întuneric, personajul povestește istoria spitalului, de ce a fost construit departe de civilizație și de ce este considerat bântuit.

### Regulile Jocului
* **Walking Simulator:** Fără luptă, doar explorare și investigație. Zonele întunecate se luminează cu lanterna sau cu o sursă de lumină aflată în spatele jucătorului.
* **Mecanică de Frică:** Frica crește la evenimente paranormale și scade în timp. Nivelul mare de frică afectează lanterna și vizibilitatea.
* **Subvertirea Așteptărilor:** Jocul creează tensiune prin alarme false după colțuri. După ce jucătorul se relaxează, apar evenimente scriptate reale, prin surprindere. Personajul se poate speria și singur.
* **Echipament Ghost Hunt:**
  *  **Lanternă:** Luminează calea.
  *  **Aparat Foto Polaroid:** Fotografiază anomalii.
  *  **EMF Meter:** Măsoară câmpurile electromagnetice (1-5).
  *  **SpiritPod:** Înregistrează și ascultă răspunsuri de la spirite.
  *  **Dowsing Rods:** Indică direcția anomaliilor.

### Controale Propuși
* `W / A / S / D`: Deplasare | `Shift`: Sprint (consumă stamina)
* `F`: Aprinde / Stinge lanterna | `Click Stânga`: Folosește unealta
* `E`: Interacțiune | `Scroll / 1-5`: Schimbă unealta | `ESC`: Pauză

---

## Structuri de date

Fiecare fișier `.hpp` din `Fisiere_HPP` are fișierul `.cpp` corespunzător în `Fisiere_CPP`, cu implementările metodelor.

| Fișier | Conține |
|---|---|
| `Types.hpp` | `Vec3`, `Color` și enum-urile `GameState`, `Action`, `ToolType`, `EventType` |
| `Tool.hpp` | clasa de bază abstractă pentru unelte |
| `Tools.hpp` | `Flashlight`, `PolaroidCamera`, `EMFMeter`, `SpiritPod`, `DowsingRods`, `Photo` |
| `Player.hpp` | jucătorul (poziție, stamina, frică, inventar) |
| `Event.hpp` | evenimentele paranormale (scaruri scriptate, false alarme) |
| `Location.hpp` | locațiile jocului și obiectele interactive |
| `NarrationLine.hpp` | o replică din povestea narată |
| `StoryManager.hpp` | gestionarea poveștii și a replicilor |
| `Listener.hpp` | starea input-ului (tastatură, mouse) |
| `IRenderer.hpp` | clasa de bază abstractă pentru redarea obiectelor de joc |
| `Renderer.hpp` | desenatorul concret (moștenește `IRenderer`): fereastră, lanternă, HUD |
| `GameEngine.hpp` | motorul jocului, ține toate componentele |

---

## Construirea proiectului

Proiectul este scris în C++17 și se construiește din linia de comandă, cu compilatorul **MinGW G++** și utilitarul **make**.

### Cerințe
- Git
- MinGW G++ (compilator C++)
- make

### Pași

1. Clonează repozitoriul și intră în ramura `lab03`:
   ```
   git checkout lab03
   git pull
   ```

2. Din directorul proiectului, curăță eventualele fișiere vechi și construiește aplicația:
   ```
   make clean
   make
   ```

   Comanda `make` compilează fiecare fișier `.cpp` din `Fisiere_CPP` într-un fișier obiect `.o` (de exemplu `g++ -std=c++17 -IFisiere_HPP -c Fisiere_CPP/Player.cpp -o Player.o`), apoi le leagă pe toate într-un singur executabil, `GonjiamGame.exe`. Fișierele `.o` se recompilează automat când se modifică un `.cpp` sau un `.hpp`.

3. Rulează jocul:
   ```
   GonjiamGame.exe
   ```
   (pe Linux/macOS: `./GonjiamGame.exe`)

4. Pentru a șterge fișierele obiect și executabilul generat:
   ```
   make clean
   ```

### Compilare manuală a unui singur fișier

Pentru a testa compilatorul pe un singur fișier, fără `make`:
```
g++ -std=c++17 -IFisiere_HPP -c Fisiere_CPP/Player.cpp -o Player.o
```
Cheia `-o` indică fișierul de ieșire, `-c` spune compilatorului să oprească după crearea fișierului obiect, fără linking, iar `-IFisiere_HPP` îi arată unde să caute fișierele `.hpp`.

### Fișiere de proiect relevante

- **`Makefile`** — construiește automat toate fișierele `.o` și executabilul final.
- **`.gitignore`** — exclude din git fișierele generate (`*.o`, `*.exe`) și fișierele specifice editorului.
- **`Prompt_lab01.md`** — prompturile folosite la generarea structurii inițiale a proiectului (Laboratorul 01).

---

## Laborator 03: constructori și operatori

Pentru fiecare structură care reprezintă un element de joc sunt definite: constructor implicit, constructor cu parametri, constructor de copiere, operator de atribuire, operatori de comparare și operatori de intrare / ieșire (`<<` și `>>`).

Structurile de tip sistem (`Listener`, `Renderer`, `GameEngine`, `StoryManager`) nu sunt elemente de joc și nu au aceste operatori.

### Ce este definit pentru fiecare structură

| Structură | Constructor cu parametri | Comparare | `<<` / `>>` |
|---|---|---|---|
| `Vec3` | `Vec3(x, y, z)` | `==` `!=` `<` `>` `<=` `>=` (ordine lexicografică) | `x y z` |
| `Color` | `Color(r, g, b, a = 255)` | `==` `!=` | `r g b a` |
| `Photo` | `Photo(position, anomalyCaptured, description)` | `==` `!=` | da |
| `Flashlight` | `Flashlight(radius, intensity)` | `==` `!=` | da |
| `PolaroidCamera` | `PolaroidCamera(photosLeft, flashCooldown)` | `==` `!=` | da |
| `EMFMeter` | `EMFMeter(detectionRange, sensitivity)` | `==` `!=` | da |
| `SpiritPod` | `SpiritPod(responseChance, range)` | `==` `!=` | da |
| `DowsingRods` | `DowsingRods(detectionRange)` | `==` `!=` | da |
| `Player` | `Player(name, position)` | `==` `!=` | da |
| `Event` | `Event(id, type, locationId, triggerPosition, fearAmount)` | `==` `!=` `<` (după `id`) | da |
| `Interactable` | `Interactable(name, position, loreText)` | `==` `!=` | da |
| `Location` | `Location(id, name, description, isDark)` | `==` `!=` `<` (după `id`) | da |
| `NarrationLine` | `NarrationLine(text, duration, triggerLocationId, triggerPosition, chapter)` | `==` `!=` `<` (după `chapter`) | da |

Operatorii `<` permit folosirea structurilor cu `std::sort` și `std::set` în laboratorul următor (Biblioteca Standard C++).

### Ierarhia de unelte

`Tool` este clasă abstractă: are metoda pur virtuală `Update` și destructor virtual, deoarece uneltele sunt ținute în `std::vector<std::unique_ptr<Tool>>`. Fiecare unealtă concretă suprascrie metodele virtuale:

| Metodă | Rol |
|---|---|
| `Clone()` | returnează o copie a uneltei, de același tip concret |
| `Equals(other)` | compară și câmpurile proprii; unelte de tipuri diferite nu sunt niciodată egale |
| `Print(os)` / `Read(is)` | scriu / citesc câmpurile uneltei |

Operatorii `==`, `!=`, `<<` și `>>` sunt definiți o singură dată, pentru `Tool`, și apelează metodele virtuale de mai sus, deci funcționează pentru orice unealtă (`std::cout << flashlight`, `flashlight1 == flashlight2`).

Funcția `MakeTool(type)` creează o unealtă după tip, iar `ReadTool(is)` citește din flux o unealtă de orice tip.

### Copierea jucătorului

`Player` conține `std::vector<std::unique_ptr<Tool>>`, care nu poate fi copiat direct. Constructorul de copiere și operatorul de atribuire fac copie profundă: copiază fiecare unealtă prin `Clone()`, deci două obiecte `Player` nu împart niciodată aceleași unelte.

### Formatul de intrare / ieșire

Valorile sunt separate prin spațiu, enum-urile se scriu ca număr, iar textele între ghilimele (`std::quoted`), ca să poată conține spații. Ce se scrie cu `<<` se poate citi înapoi cu `>>`. Exemplu pentru `Player`:

```
"Dima" 1 2 3 0 0 3 5.5 0 100 100 20 10 12.5 100 2 1 1 2 0 "Flashlight" 0 100 100 0 0 0 1 9 0.8 0 0 2 "EMF Meter" 0 100 100 0 0 0 1 1 11 1.2
```

Primele valori sunt câmpurile jucătorului, apoi numărul de unelte, apoi fiecare unealtă (tip, nume, câmpurile comune, câmpurile proprii).

### Clasa abstractă pentru redare

`IRenderer` (`IRenderer.hpp`) este clasa de bază abstractă pentru redarea obiectelor de joc. Toate metodele ei sunt pur virtuale (`Init`, `Clear`, `Present`, `DrawScene`, `DrawHUD`) și are destructor virtual, deci nu se poate crea un obiect `IRenderer` direct. `Renderer` o moștenește și implementează metodele cu `override`. Un alt desenator (în consolă, OpenGL) se adaugă prin simpla moștenire din `IRenderer`.

### Utilizarea LLM

La acest laborator a fost folosit un LLM pentru scrierea constructorilor și a operatorilor. Codul a fost verificat cu un program de test care compară obiectele după copiere, atribuire și citire / scriere, rulat cu AddressSanitizer și UBSan.