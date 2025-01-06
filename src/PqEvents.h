/*
 * PqEvents.h
 *
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PqCore.h"

#ifndef PQ_EVENT_MANAGER_H_
#define PQ_EVENT_MANAGER_H_

namespace pq {

class Unit;

/// Callback type for event listeners.
typedef void (*EventCallback)();

enum EventType {  
  EVENT_NONE,
  EVENT_CHANGE,
  EVENT_RISE,
  EVENT_FALL,
  EVENT_BANG,
};

/// Manages event listeners for Plaquette units.
class EventManager {

private:
  // A listener is a tuple (unit, callback, eventType)
  struct Listener {
    Unit* unit;
    EventCallback callback;
    EventType eventType;

    Listener() : unit(NULL), callback(NULL), eventType(EVENT_NONE) {}

    Listener(Unit* unit, EventCallback callback, EventType eventType) : 
      unit(unit), callback(callback), eventType(eventType) {
    }

    bool operator==(const Listener& other) const {
      return unit == other.unit && 
             callback == other.callback && 
             eventType == other.eventType;
    }
  };
public:
  /// Adds a listener to the event manager.
  void addListener(Unit* unit, EventCallback callback, EventType eventType);
  // void removeListeners(Unit* unit, EventCallback callback, uint8_t eventType);

  /// Clears all listeners for a given unit.
  void clearListeners(Unit* unit);

  /// Performs a single step of the event manager.
  void step();

private:
  // Contains all listeners.
  HybridArrayList<Listener, 4> _listeners;
};
}

#endif
