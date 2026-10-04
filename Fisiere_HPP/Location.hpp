#pragma once

#include <string>
#include <vector>

#include "Types.hpp"

struct Interactable
{

    Interactable();

    
    std::string name;
    Vec3 position;
    bool used = false;
    std::string loreText;
};

struct Location
{
    Location();

    void MarkVisited();
    bool IsVisited() const;


    int id = 0;
    std::string name;
    std::string description;

    bool isDark = true;
    float ambientLight = 0.0f;
    bool hasBackLight = false;
    std::string ambientSound;

    Vec3 size;

    std::vector<int> exits;
    std::vector<int> events;

    std::vector<Interactable> interactables;

    bool visited = false;
};