#include "Location.hpp"

#include <algorithm>
#include <iomanip>
#include <istream>
#include <iterator>
#include <ostream>

// ---------- Interactable ----------

Interactable::Interactable() {}

Interactable::Interactable(const std::string &_name, const Vec3 &_position, const std::string &_loreText)
    : name(_name), position(_position), loreText(_loreText) {}

Interactable::Interactable(const Interactable &other)
    : name(other.name), position(other.position), used(other.used), loreText(other.loreText) {}

Interactable &Interactable::operator=(const Interactable &other)
{
    if (this != &other)
    {
        name = other.name;
        position = other.position;
        used = other.used;
        loreText = other.loreText;
    }
    return *this;
}

bool Interactable::operator==(const Interactable &other) const
{
    return name == other.name && position == other.position && used == other.used &&
           loreText == other.loreText;
}

bool Interactable::operator!=(const Interactable &other) const { return !(*this == other); }

std::ostream &operator<<(std::ostream &os, const Interactable &item)
{
    return os << std::quoted(item.name) << ' ' << item.position << ' ' << item.used << ' '
              << std::quoted(item.loreText);
}

std::istream &operator>>(std::istream &is, Interactable &item)
{
    return is >> std::quoted(item.name) >> item.position >> item.used >> std::quoted(item.loreText);
}

// ---------- Location ----------

Location::Location() {}

Location::Location(int _id, const std::string &_name, const std::string &_description, bool _isDark)
    : id(_id), name(_name), description(_description), isDark(_isDark) {}

Location::Location(const Location &other)
    : id(other.id), name(other.name), description(other.description), isDark(other.isDark),
      ambientLight(other.ambientLight), hasBackLight(other.hasBackLight),
      ambientSound(other.ambientSound), size(other.size), exits(other.exits), events(other.events),
      interactables(other.interactables), visited(other.visited) {}

Location &Location::operator=(const Location &other)
{
    if (this != &other)
    {
        id = other.id;
        name = other.name;
        description = other.description;
        isDark = other.isDark;
        ambientLight = other.ambientLight;
        hasBackLight = other.hasBackLight;
        ambientSound = other.ambientSound;
        size = other.size;
        exits = other.exits;
        events = other.events;
        interactables = other.interactables;
        visited = other.visited;
    }
    return *this;
}

bool Location::operator==(const Location &other) const
{
    return id == other.id && name == other.name && description == other.description &&
           isDark == other.isDark && ambientLight == other.ambientLight &&
           hasBackLight == other.hasBackLight && ambientSound == other.ambientSound &&
           size == other.size && exits == other.exits && events == other.events &&
           interactables == other.interactables && visited == other.visited;
}

bool Location::operator!=(const Location &other) const { return !(*this == other); }

bool Location::operator<(const Location &other) const { return id < other.id; }

void Location::MarkVisited() { visited = true; }
bool Location::IsVisited() const { return visited; }

std::ostream &operator<<(std::ostream &os, const Location &l)
{
    os << l.id << ' ' << std::quoted(l.name) << ' ' << std::quoted(l.description) << ' '
       << l.isDark << ' ' << l.ambientLight << ' ' << l.hasBackLight << ' '
       << std::quoted(l.ambientSound) << ' ' << l.size << ' ' << l.visited;

    // std::for_each scrie fiecare element, dupa numarul de elemente.
    os << ' ' << l.exits.size();
    std::for_each(l.exits.begin(), l.exits.end(), [&os](int exit) { os << ' ' << exit; });

    os << ' ' << l.events.size();
    std::for_each(l.events.begin(), l.events.end(), [&os](int event) { os << ' ' << event; });

    os << ' ' << l.interactables.size();
    std::for_each(l.interactables.begin(), l.interactables.end(),
                  [&os](const Interactable &item) { os << ' ' << item; });

    return os;
}

std::istream &operator>>(std::istream &is, Location &l)
{
    std::size_t count = 0;

    is >> l.id >> std::quoted(l.name) >> std::quoted(l.description) >> l.isDark >>
        l.ambientLight >> l.hasBackLight >> std::quoted(l.ambientSound) >> l.size >> l.visited;

    is >> count;
    l.exits.clear();
    for (std::size_t i = 0; i < count && is; ++i)
    {
        int exit = 0;
        if (is >> exit)
            l.exits.insert(exit);
    }

    is >> count;
    l.events.clear();
    for (std::size_t i = 0; i < count && is; ++i)
    {
        int event = 0;
        if (is >> event)
            l.events.insert(event);
    }

    is >> count;
    l.interactables.clear();
    for (std::size_t i = 0; i < count && is; ++i)
    {
        Interactable item;
        if (is >> item)
            l.interactables.push_back(item);
    }

    return is;
}