#pragma once

#include <iostream>

#include "Renderer.hpp"

// Desenatorul concret al jocului; implementeaza clasa abstracta Renderer.
// Afiseaza in consola prin fluxul de iesire primit ca referinta (implicit std::cout).
struct ConsoleRenderer : Renderer
{
    explicit ConsoleRenderer(std::ostream &_out = std::cout);

    bool Init() override;
    void Clear() override;
    void Present() override;
    void DrawScene(const Location &location, const Player &player) override;
    void DrawHUD(const Player &player) override;

    std::ostream &out; // referinta, nu pointer: fluxul exista mai mult decat desenatorul
};