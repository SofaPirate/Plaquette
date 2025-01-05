#include "PqEventManager.h"

namespace pq {

void EventManager::addListener(Unit* unit, EventCallback callback, EventType eventType) {
  _listeners.add(Listener(unit, callback, eventType));
}

// void EventManager::removeListeners(Unit* unit, EventCallback callback, uint8_t eventType) {
//   Listener toRemove(unit, callback, eventType);
//   while (_listeners.removeItem(toRemove));
// }

void EventManager::clearListeners(Unit* unit) {
  for (int i=_listeners.size()-1; i>=0; i--) {
    if (_listeners[i].unit == unit) {
      _listeners.remove(i);
    }
  }
}

void EventManager::step() {
  for (int i=0; i<_listeners.size(); i++) {
    Listener& listener = _listeners[i];
    if (listener.unit->eventTriggered(listener.eventType)) {
      listener.callback();
    }
  }
}

}
