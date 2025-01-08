/*
 * PqEvents.cpp
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
#include "PqEvents.h"

namespace pq {

void EventManager::addListener(Unit* unit, EventCallback callback, EventType eventType) {
  _listeners.add(Listener(unit, callback, eventType));
}

// void EventManager::removeListeners(Unit* unit, EventCallback callback, uint8_t eventType) {
//   Listener toRemove(unit, callback, eventType);
//   while (_listeners.removeItem(toRemove));
// }

void EventManager::clearListeners(Unit* unit) {
  for (int i=(int)_listeners.size()-1; i>=0; i--) {
    if (_listeners[i].unit == unit) {
      _listeners.remove(i);
    }
  }
}

void EventManager::step() {
  for (int i=0; i<(int)_listeners.size(); i++) {
    Listener& listener = _listeners[i];
    if (listener.unit->eventTriggered(listener.eventType)) {
      listener.callback();
    }
  }
}

}
