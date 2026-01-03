/*
 * HybridArrayList.h
 *
 * (c) 2023 Sofian Audry        :: info(@)sofianaudry(.)com
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

#ifndef HYBRID_ARRAY_LIST_H_
#define HYBRID_ARRAY_LIST_H_

#include "pq_math.h"

#define HYBRID_ARRAY_LIST_DEFAULT_STATIC_CAPACITY 8
#define HYBRID_ARRAY_LIST_DYNAMIC_GROWTH_FACTOR 1.5

namespace pq {

/**
 * @brief A hybrid array list that starts with a static array and switches to dynamic allocation when full.
 *
 * @tparam T The type of elements stored in the list.
 * @tparam STATIC_CAPACITY The initial static size of the array.
 */
template<typename T, int STATIC_CAPACITY = HYBRID_ARRAY_LIST_DEFAULT_STATIC_CAPACITY>
class HybridArrayList {

public:
    /**
     * Constructs a new Hybrid Array List object.
     */
    HybridArrayList() :  _dynamicArray(0), _size(0), _dynamicCapacity(0) {}

    /**
     * Destroys the Hybrid Array List object, deallocating any dynamic memory used.
     */
    ~HybridArrayList() {
        if (_dynamicArray)
          delete[] _dynamicArray;
    }

    /**
     * Adds an element to the end of the list.
     *
     * @param element The element to be added.
     */
    void add(const T& item) {
        // Ensure there's room for one more element.
        _ensureCapacity();

        // Adding in the static array.
        if (_size < STATIC_CAPACITY) {
            _staticArray[_size] = item;
        }
        // Adding in the dynamic array.
        else {
            _dynamicArray[_size - STATIC_CAPACITY] = item;
        }
        _size++;
    }

    /**
     * Inserts an element at the specified position in the list.
     *
     * @param index The position at which the element should be inserted.
     * @param element The element to insert.
     */
    void insert(int index, const T& item) {
        // If index is out of bounds, do nothing.
        if (index < 0 || (size_t)index > _size) {
            return;
        }

        // Ensure there's room for one more element.
        _ensureCapacity();

        // Inserting within the static array.
        if (index < STATIC_CAPACITY) {

            // Only affects static array.
            if (_size < STATIC_CAPACITY) {
                // Shift static array elements to the right.
                for (int i = _size; i > index; --i) {
                    _staticArray[i] = _staticArray[i - 1];
                }
                // Add item.
                _staticArray[index] = item;
            }
            // Affects both static and dynamic arrays.
            else {
                // Last element of static array moves to dynamic array.
                T temp = _staticArray[STATIC_CAPACITY - 1];
                for (int i = STATIC_CAPACITY - 1; i > index; --i) {
                    _staticArray[i] = _staticArray[i - 1];
                }
                // Add item.
                _staticArray[index] = item;
                // Now insert temp into the first position of dynamic array, shifting others.
                insert(STATIC_CAPACITY, temp);
            }
        }
        // Inserting within the dynamic array.
        else {
            // Shift dynamic array elements to the right.
            for (int i = _size - STATIC_CAPACITY; i > index - STATIC_CAPACITY; --i) {
                _dynamicArray[i] = _dynamicArray[i - 1];
            }
            // Add item.
            _dynamicArray[index - STATIC_CAPACITY] = item;
        }
        // Increase size.
        _size++;
    }

    /**
     * Finds the first occurrence of the specified element in this list.
     *
     * @param item The element to search for.
     * @return The index of the first occurrence of the specified element in this list, or -1 if this list does not contain the element.
     */
    int indexOf(const T& item) {
        // Search in static array.
        for (size_t i = 0; i < STATIC_CAPACITY && i < _size; ++i) {
            if (_staticArray[i] == item) {
                return (int)i;
            }
        }
        // Search in dynamic array.
        for (size_t i = 0; i < _size - STATIC_CAPACITY; ++i) {
            if (_dynamicArray[i] == item) {
                return (int)(i + STATIC_CAPACITY);
            }
        }
        // Not found.
        return (-1);
    }

    /**
     * Removes the first occurrence of the specified element from this list, if it is present.
     *
     * @param item The element to be removed from this list, if present.
     */
    bool removeItem(const T& item) {
        // Try to find the item.
        int index = indexOf(item);
        // If it exists, remove it.
        if (index >= 0) {
            remove(index);
            return true;
        }
        // If it doesn't exist, do nothing.
        else
          return false;
    }

    /**
     * Removes the element at the specified position in the list.
     *
     * @param index The position of the element to remove.
     */
     void remove(int index) {
        // If index is out of bounds, do nothing.
        if (index < 0 || (size_t)index >= _size) {
            return;
        }
        // Index is within static array.
        if (index < STATIC_CAPACITY) {
            // Shift static array to the left, overwriting item.
            for (size_t i = index; i < STATIC_CAPACITY - 1; i++) {
                _staticArray[i] = _staticArray[i + 1];
            }
            // If there is a dynamic array, also shift items there to the left.
            if (_size > STATIC_CAPACITY) {
                _staticArray[STATIC_CAPACITY - 1] = _dynamicArray[0];
                for (size_t i = 0; i < _size - STATIC_CAPACITY - 1; i++) {
                      _dynamicArray[i] = _dynamicArray[i + 1];
                }
            }
        }
        // Index is within dynamic array.
        else {
            // Shift dynamic array to the left.
            for (size_t i = index - STATIC_CAPACITY; i < _size - STATIC_CAPACITY - 1; i++) {
                _dynamicArray[i] = _dynamicArray[i + 1];
            }
        }
        // Reduce size.
        _size--;
        // Optional: Shrink dynamic array if too much unused space
    }

    // Operator[] for element access, behaving like get()
    T& operator[](int index) {
        index = constrain(index, 0, (int)_size - 1);
        return index < STATIC_CAPACITY ? _staticArray[index] : _dynamicArray[index - STATIC_CAPACITY];
    }

    // Const version of operator[] to work with const objects
    const T& operator[](int index) const {
        return this->operator[](index);
    }

    /**
     * Retrieves the element at the specified position in the list.
     *
     * @param index The position of the element to retrieve.
     * @return The element at the specified position.
     */
    T get(int index) const {
        return this->operator[](index);
    }

    /**
     * Removes all items from the list without changing its capacity.
    */
    void removeAll() {
      _size = 0;
    }

    // Return size.
    size_t size() const {
        return _size;
    }

    // Returns capacity.
    size_t capacity() const {
        return STATIC_CAPACITY + _dynamicCapacity;
    }

private:
    T _staticArray[STATIC_CAPACITY]; ///< The static array used initially.
    T* _dynamicArray; ///< The dynamic array used when the static array is full.
    size_t _size; ///< The current number of elements.
    size_t _dynamicCapacity; ///< The current capacity of the dynamic array.

    /**
     * Ensures there is enough capacity for new elements, resizing the dynamic array if necessary.
     */
    void _ensureCapacity() {
        // If we have filled up static array, create dynamic array.
        if (_size == STATIC_CAPACITY) {
            // If dynamic array doesn't exist, create it.
            if (!_dynamicArray) {
              _dynamicCapacity = STATIC_CAPACITY * (HYBRID_ARRAY_LIST_DYNAMIC_GROWTH_FACTOR - 1);
              _dynamicArray = new T[_dynamicCapacity];
            }
        }
        // If we have filled up both arrays, increase dynamic array size.
        else if (_size >= STATIC_CAPACITY + _dynamicCapacity) {
            // Create new dynamic array with increased capacity.
            size_t newDynamicCapacity = capacity() * HYBRID_ARRAY_LIST_DYNAMIC_GROWTH_FACTOR - STATIC_CAPACITY;
            T* newDynamicArray = new T[newDynamicCapacity];
            // Copy elements from old dynamic array to new dynamic array.
            memcpy(newDynamicArray, _dynamicArray, _dynamicCapacity * sizeof(T));
            // Delete old dynamic array and update pointer and capacity.
            delete[] _dynamicArray;
            _dynamicArray = newDynamicArray;
            _dynamicCapacity = newDynamicCapacity;
        }
  }
};

}

#endif // HYBRID_ARRAY_LIST_H_
