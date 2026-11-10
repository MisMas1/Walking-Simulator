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
| `Renderer.hpp` | clasa de bază abstractă pentru redarea obiectelor de joc (date de afișare + metode pur virtuale) |
| `Consolerenderer.hpp` | desenatorul concret, moștenește `Renderer` și scrie în consolă |
| `GameEngine.hpp` | motorul jocului, ține toate componentele |

---

## Construirea proiectului

Proiectul este scris în C++17 și se construiește din linia de comandă, cu compilatorul **MinGW G++**, utilitarul **make** și biblioteca externă **SQLite**.

### Cerințe
- Git
- MinGW G++ (compilator C++)
- make
- SQLite 3 (fișiere de dezvoltare, inclusiv `sqlite3.h` și `libsqlite3`)
- `pkg-config` pentru detectarea opțiunilor SQLite

### Pași

1. Clonează repozitoriul și intră în ramura `lab05`:
   ```
   git checkout lab05
   git pull
   ```

2. Din directorul proiectului, curăță eventualele fișiere vechi și construiește aplicația:
   ```
   make clean
   make
   ```

   Comanda `make` compilează modulele de date în biblioteci statice (`build/lib/libtypes.a`, `libtool.a`, `libtools.a`, `libplayer.a`, `libevent.a`, `liblocation.a` și `libnarrationline.a`), apoi le leagă împreună cu modulele aplicației și SQLite în `GonjiamGame.exe`. Fișierele obiect și bibliotecile se reconstruiesc automat când se modifică sursele sau antetele.

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

- **`Makefile`** — construiește bibliotecile statice și executabilul final, legând SQLite cu opțiunile raportate de `pkg-config`.
- **`.gitignore`** — exclude din git fișierele generate (`*.o`, `*.exe`) și fișierele specifice editorului.
- **`Prompt_lab01.md`** — prompturile folosite la generarea structurii inițiale a proiectului (Laboratorul 01).

---

## Laborator 05: biblioteci și stocarea datelor

Modulele care conțin datele jocului sunt construite ca biblioteci statice:

| Bibliotecă | Clase / structuri |
|---|---|
| `libtypes.a` | `Vec3`, `Color` și enum-uri |
| `libtool.a` | clasa de bază `Tool` |
| `libtools.a` | `Flashlight`, `Photo`, `PolaroidCamera`, `EMFMeter`, `SpiritPod`, `DowsingRods` |
| `libplayer.a` | `Player` |
| `libevent.a` | `Event` |
| `liblocation.a` | `Location`, `Interactable` |
| `libnarrationline.a` | `NarrationLine` |

Pentru stocarea externă a datelor, `GameEngine::SaveGame` și `GameEngine::LoadGame` folosesc SQLite. Baza `savegame.db` conține tabelul `game_state`; starea serializată a jocului este salvată tranzacțional și este citită în variabile temporare înainte de a actualiza motorul. Executabilul se leagă cu `-lsqlite3`, iar conținutul bazei nu trebuie adăugat în Git.

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

`Renderer` (`Renderer.hpp`) este clasa de bază abstractă pentru redarea obiectelor de joc. Metodele de desenare (`Init`, `Clear`, `Present`, `DrawScene`, `DrawHUD`) sunt pur virtuale (`= 0`) și clasa are destructor virtual, deci nu se poate crea un obiect `Renderer` direct. Datele de afișare (fereastră, lanternă, subtitrări) rămân câmpuri ale clasei de bază. `ConsoleRenderer` o moștenește și implementează metodele cu `override`. Un alt desenator (OpenGL, de exemplu) se adaugă prin simpla moștenire din `Renderer`.

## Laborator 04: biblioteca standard C++

Codul din lab03 a fost rescris cu biblioteca standard, fără pointeri bruți și fără `new` / `delete`.

### Containere STL

| Container | Unde | De ce |
|---|---|---|
| `std::map<int, Location>` | `GameEngine::locations` | locațiile se găsesc după id și rămân ordonate |
| `std::set<int>` | `Location::exits`, `Location::events` | id-uri unice, ordonate automat |
| `std::vector` | `Player::items`, `GameEngine::events`, `StoryManager::lines`, `Location::interactables`, `PolaroidCamera::photos` | secvențe cu acces după index |
| `std::map<std::string, bool>` | `StoryManager::endingFlags` | indicatori ai finalului |
| `std::map<int, Action>` | `Listener::keyBindings` | asocierea tastă → acțiune |
| `std::optional<std::string>` | `StoryManager::PlayNext` | „poate nu există replică", fără pointer nul |

### Fluxuri de intrare / ieșire

- `operator<<` și `operator>>` pentru toate elementele de joc (lab03), acum scrise cu `std::for_each`.
- `GameEngine::SaveGame` și `GameEngine::LoadGame` folosesc SQLite; `LoadGame` citește totul în variabile locale și modifică motorul abia după ce citirea a reușit, deci o salvare coruptă nu strică jocul.
- `ConsoleRenderer` scrie în orice `std::ostream` primit ca referință (implicit `std::cout`).
- Afișarea în consolă se face cu `std::cout`, `std::ostream_iterator` și `std::flush`.

### Referințe și smart pointers în locul pointerilor

| Înainte | Acum |
|---|---|
| `dynamic_cast<const Flashlight *>(&other)` | `typeid(*this) != typeid(other)` și `static_cast<const Flashlight &>(other)` |
| căutarea unei unelte ar fi întors `Tool *` | `Player::GetTool` și `Player::ActiveTool` întorc `Tool &` și aruncă `std::out_of_range` dacă nu există |
| `GameEngine::FindLocation` / `FindEvent` | întorc `Location &` / `Event &` |
| replică inexistentă ar fi însemnat `nullptr` | `std::optional<std::string>` |
| desenatorul alocat cu `new` | `std::unique_ptr<Renderer>` creat cu `std::make_unique<ConsoleRenderer>()` |
| fluxul de ieșire al desenatorului | `std::ostream &out` (referință) |

Uneltele rămân în `std::vector<std::unique_ptr<Tool>>`, iar copierea lor se face prin `Clone()`.

### Șiruri de caractere

Peste tot se folosește `std::string`, fără `char *` sau tablouri de `char`. Enum-urile au funcții `ToString(...)` care întorc `std::string` pentru afișare.

### Algoritmi STL

| Scop | Algoritm | Unde |
|---|---|---|
| căutare | `std::find_if` | `Player::GetTool`, `StoryManager::PlayNext`, `GameEngine::FindEvent` |
| căutare | `std::any_of` | `Player::HasTool` |
| căutare | `std::find` | `StoryManager::HasLore` |
| căutare | `std::upper_bound` | `StoryManager::AddLine` (poziția unde se inserează replica) |
| numărare | `std::count_if` | `StoryManager::RemainingLines`, `operator<<` pentru `Player` |
| sortare | `std::sort` | `GameEngine::Init` și `LoadGame` ordonează evenimentele după id (`Event::operator<`) |
| comparare | `std::equal` | `Player::operator==` compară inventarele |
| comparare | `std::tie` | `Vec3` și `Color` compară lexicografic, ca `std::lexicographical_compare` |
| copiere | `std::transform` | copierea profundă a inventarului (`Clone()` pentru fiecare unealtă) |
| copiere | `std::copy_if` | `GameEngine::EventsInLocation` |
| copiere | `std::copy` | `ConsoleRenderer::DrawScene` scrie ieșirile locației |
| parcurgere | `std::for_each` | scrierea în fluxuri, `GameEngine::TriggerEvents` |
| limitare | `std::clamp` | `Player::AddFear`, `Update`, `Rotate` |

### Ce face jocul acum

`Init()` creează jucătorul cu 3 unelte, 4 locații, 4 evenimente și 5 replici. `Run()` parcurge locațiile, afișează scena, replicile lui Dima și evenimentele declanșate. Mecanica din GDD funcționează: alarmele false cresc contorul `safeCorners`, iar scarul scriptat apare doar după ce contorul a ajuns la 2. La final, jocul se salvează în `savegame.db`, se încarcă într-un motor nou și se compară datele.

### Utilizarea LLM

La laboratoarele 03 și 04 a fost folosit un LLM pentru scrierea constructorilor, a operatorilor și pentru rescrierea codului cu biblioteca standard. Codul a fost verificat cu un program de test care compară obiectele după copiere, atribuire și citire / scriere, rulat cu AddressSanitizer și UBSan.