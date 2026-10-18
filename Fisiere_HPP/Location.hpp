#pragma once

#include <iosfwd>
#include <string>
#include <vector>

#include "Types.hpp"

struct Interactable
{

    Interactable();
    Interactable(const std::string &_name, const Vec3 &_position, const std::string &_loreText);
    Interactable(const Interactable &other);
    Interactable &operator=(const Interactable &other);

    bool operator==(const Interactable &other) const;
    bool operator!=(const Interactable &other) const;

    std::string name;
    Vec3 position;
    bool used = false;
    std::string loreText;
};

std::ostream &operator<<(std::ostream &os, const Interactable &item);
std::istream &operator>>(std::istream &is, Interactable &item);

struct Location
{
    Location();
    Location(int _id, const std::string &_name, const std::string &_description, bool _isDark);
    Location(const Location &other);
    Location &operator=(const Location &other);

    bool operator==(const Location &other) const;
    bool operator!=(const Location &other) const;
    bool operator<(const Location &other) const; // ordine dupa id

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

std::ostream &operator<<(std::ostream &os, const Location &location);
std::istream &operator>>(std::istream &is, Location &location);