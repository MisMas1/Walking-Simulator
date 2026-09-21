# Prompturi folosite pentru Laboratorul 01

**Model LLM folosit:** Claude (Anthropic), prin interfața web claude.ai

**Scop:** structurarea proiectului (walking simulator horror în C++) și crearea fișierelor `.hpp` de bază cerute în laborator: motor de joc, desenator, ascultător, plus structurile auxiliare.

---

## Prompt 1: stabilirea structurii proiectului

**Descriere:** I-am prezentat modelului ideea jocului (atmosferă horror, personaj YouTuber, locație abandonată, aparate de investigat fenomene paranormale, momente de sperietură scriptate) și cerințele laboratorului. Am cerut o propunere de organizare a codului: ce fișiere `.hpp` sunt necesare, cum se numesc și ce rol are fiecare, fără cod complet, doar direcții.

**Rezultat:** Modelul a propus cele 3 fișiere obligatorii (motor, desenator, ascultător) și fișiere auxiliare pentru jucător, unelte, locații, evenimente, poveste și constante. Pentru fiecare a descris rolul și conținutul așteptat. A adăugat și pașii de lucru cu Git și recomandări generale pentru fișiere header.

**Observații / corecturi:** Enunțul laboratorului are un punct repetat (README cu descrierea proiectului). L-am tratat ca pe o greșeală de copiere și am folosit un singur `README.md`.

---

## Prompt 2: prioritizarea fișierelor

**Descriere:** Am cerut o estimare a numărului de fișiere care trebuie create în această etapă.

**Rezultat:** Modelul a împărțit fișierele în trei grupe: minimul pentru laborator, setul recomandat pentru primul commit și fișierele care pot fi adăugate ulterior.

**Observații / corecturi:** Nu au fost necesare corecturi.

---

## Prompt 3: conținutul fișierelor obligatorii

**Descriere:** Am cerut o descriere a rolului în joc pentru motor, desenator și ascultător, împreună cu variabilele care ar trebui să intre în ele. Am precizat că nu am nevoie încă de metode, doar de câmpuri, oferind ca model exemplul clasei jucătorului (viteză, stamină, obiecte).

**Rezultat:** Pentru fiecare fișier am primit un tabel cu câmpurile propuse, tipul de date și rolul lor.

**Observații / corecturi:** Nu au fost necesare corecturi.

---

## Prompt 4: conținutul celorlalte fișiere

**Descriere:** Am cerut același tip de descriere (rol în joc și variabile) pentru toate fișierele rămase, mai puțin README.

**Rezultat:** Modelul a generat tabele pentru fișierele auxiliare (tipuri comune, jucător, unelte, locații, evenimente, poveste, constante) și o ordine recomandată a include-urilor.

**Observații / corecturi:** Pozițiile erau inițial gândite în 2D. Le-am schimbat în 3D după promptul 6.

---

## Prompt 5: prima variantă de cod pentru desenator

**Descriere:** Am cerut transpunerea în cod a fișierului pentru desenator.

**Rezultat:** Modelul a scris un header cu câmpuri și declarații de metode.

**Observații / corecturi:** Versiunea conținea metode, iar pentru acest laborator am nevoie doar de variabile. Am corectat la promptul 8. Un comentariu din fișier făcea referire la coordonate 2D și a fost actualizat după promptul 6.

---

## Prompt 6: clarificări despre cod și spațiul 3D

**Descriere:** Am cerut o explicație a codului generat, linie cu linie, și o clarificare despre cum se reprezintă coordonatele într-un joc 3D și în ce fișier ar trebui definite.

**Rezultat:** Modelul a explicat rolul fiecărei linii și a arătat că poziția aparține obiectelor din joc, nu desenatorului. A propus o structură cu trei axe pentru poziții și două unghiuri pentru rotația jucătorului.

**Observații / corecturi:** Am înlocuit reprezentarea 2D cu una 3D în toate structurile care rețin poziții.

---

## Prompt 7: generarea tuturor fișierelor

**Descriere:** Am cerut crearea completă, în cod, a tuturor fișierelor propuse.

**Rezultat:** Modelul a creat fișierele header și un README cu cele trei secțiuni cerute. A verificat că fișierele compilează împreună, fără erori de sintaxă.

**Observații / corecturi:** Fișierele conțineau metode (constructori, getteri, metode virtuale), deși în această etapă am nevoie doar de variabile. Am cerut rescrierea la promptul 8.

---

## Prompt 8: simplificarea la structuri de date

**Descriere:** Am precizat că în această etapă fișierele trebuie să conțină doar declarații de variabile cu tipurile lor și includerea reciprocă a fișierelor, metodele urmând să fie adăugate la laboratorul următor.

**Rezultat:** Modelul a rescris toate fișierele ca structuri cu câmpuri și valori implicite, păstrând include-urile și moștenirea uneltelor. A verificat din nou compilarea și a confirmat că nu a rămas nicio metodă.

**Observații / corecturi:** Desenatorul nu mai are nevoie de declarații anticipate, deoarece nu mai are metode. Modelul a atras atenția că la laboratorul următor clasa de bază pentru unelte va avea nevoie de un destructor virtual.

---

## Prompt 9: documentarea folosirii LLM-ului

**Descriere:** Am cerut redactarea acestui fișier, conform cerinței laboratorului, ca descriere de ansamblu a interacțiunilor cu modelul, nu ca transcriere a mesajelor mele.

**Rezultat:** Fișierul de față.

**Observații / corecturi:** Am verificat că descrierile corespund cu ce s-a întâmplat în conversație.

---

