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

Fiecare fișier `.hpp` are fișierul `.cpp` corespunzător cu implementările metodelor. `Types.hpp` și `Config.hpp` nu declară clase, doar tipuri de bază și constante, deci `.cpp`-urile lor conțin doar `#include`.

| Fișier | Conține |
|---|---|
| `Types.hpp` | `Vec3`, `Color` și enum-urile `GameState`, `Action`, `ToolType`, `EventType` |
| `Config.hpp` | constantele de configurare ale jocului |
| `Tool.hpp` | clasa de bază abstractă pentru unelte |
| `Tools.hpp` | `Flashlight`, `PolaroidCamera`, `EMFMeter`, `SpiritPod`, `DowsingRods`, `Photo` |
| `Player.hpp` | jucătorul (poziție, stamina, frică, inventar) |
| `Event.hpp` | evenimentele paranormale (scaruri scriptate, false alarme) |
| `Location.hpp` | locațiile jocului și obiectele interactive |
| `NarrationLine.hpp` | o replică din povestea narată |
| `StoryManager.hpp` | gestionarea poveștii și a replicilor |
| `Listener.hpp` | starea input-ului (tastatură, mouse) |
| `Renderer.hpp` | datele de afișare (fereastră, lanternă, HUD) |
| `GameEngine.hpp` | motorul jocului, ține toate componentele |

---

## Construirea proiectului

Proiectul este scris în C++17 și se construiește din linia de comandă, cu compilatorul **MinGW G++** și utilitarul **make**.

### Cerințe
- Git
- MinGW G++ (compilator C++)
- make

### Pași

1. Clonează repozitoriul și intră în ramura `lab02`:
   ```
   git checkout lab02
   git pull
   ```

2. Din directorul proiectului, curăță eventualele fișiere vechi și construiește aplicația:
   ```
   make clean
   make
   ```

   Comanda `make` compilează fiecare fișier `.cpp` într-un fișier obiect `.o` (de exemplu `g++ Player.cpp -o Player.o -c`), apoi le leagă pe toate într-un singur executabil, `GonjiamGame.exe`.

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
g++ Player.cpp -o Player.o -c
```
Cheia `-o` indică fișierul de ieșire, iar `-c` spune compilatorului să oprească după crearea fișierului obiect, fără linking.

### Fișiere de proiect relevante

- **`Makefile`** — construiește automat toate fișierele `.o` și executabilul final.
- **`.gitignore`** — exclude din git fișierele generate (`*.o`, `*.exe`) și fișierele specifice editorului.
- **`prompts_lab01.md`** — prompturile folosite la generarea structurii inițiale a proiectului (Laboratorul 01).