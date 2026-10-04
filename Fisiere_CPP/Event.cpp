#include "Event.hpp"

Event::Event() {}
bool Event::ShouldTrigger(const Vec3& playerPosition, int safeCorners) const { return false;}
void Event::Trigger() {}