/*
 * ArrayList.h
 *
 * (c) 2024 Brayden Anderson
 * 
 * This file was borrowed from the Arduino ArrayList library, which can be found at:
 * https://reference.arduino.cc/reference/en/libraries/arraylist/
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

#ifndef PQ_ARRAYLIST_H
#define PQ_ARRAYLIST_H

#include <Arduino.h>

template <typename T>
class ArrayList {
public:
    enum SizeType { FIXED, DYNAMIC }; // Size type
    enum SortAlgorithm { BUBBLE_SORT, QUICK_SORT }; // Sorting algorithms
    //Constructor and Destructor
    
    /**
     * @brief Constructs a new ArrayList.
     *
     * This constructor creates a new ArrayList with the specified size type, initial size, and debug flag.
     * The size type determines whether the size of the ArrayList is static or dynamic. The initial size specifies
     * the initial capacity of the ArrayList. The debug flag determines whether debug messages should be printed.
     *
     * @param type The size type of the ArrayList. This should be either DYNAMIC or STATIC.
     * @param initialSize The initial capacity of the ArrayList.
     * @param debug A flag indicating whether debug messages should be printed.
    */
    ArrayList(SizeType type = DYNAMIC, size_t initialSize = 8)
        : sizeType(type), arrayCapacity(initialSize), count(0) {
        array = new T[arrayCapacity];
    }

    /**
     * @brief Destroys the ArrayList.
     *
     * This destructor deletes the ArrayList and frees the memory allocated for it.
     * If the debug flag is set, it prints a message indicating that the ArrayList is being deleted.
    */
    ~ArrayList() {
      delete[] array;
    }
   
   /**
     * @brief Adds an item to the ArrayList.
     *
     * This function adds the specified item to the ArrayList. If the size type of the ArrayList is DYNAMIC and the ArrayList is full,
     * it resizes the ArrayList to accommodate the new item. If the ArrayList is not full, it adds the item to the ArrayList.
     * If the ArrayList is full and its size type is STATIC, it prints an error message and does not add the item.
     * (Only if the debug flag is set to true)
     * @param item The item to add to the ArrayList.
    */
    void add(T item) {
        if(sizeType == FIXED && count >= arrayCapacity){
            return;
        }
        
        // Calculate the load factor
        float loadFactor = (float)count / arrayCapacity;

        // Resize the array if the load factor is greater than or equal to 0.8
        if (sizeType == DYNAMIC && loadFactor >= 0.8) {
            resize();
        }
        // Add the item to the array
        if (count < arrayCapacity) {
            array[count++] = item;
        }
    }

    /**
     * @brief Adds all items from another ArrayList to this ArrayList.
     *
     * This function adds all items from the specified ArrayList to this ArrayList. If the size type of this ArrayList is DYNAMIC and 
     * the total count of items in both ArrayLists exceeds the capacity of this ArrayList, it resizes this ArrayList to accommodate the new items.
     * If the total count of items in both ArrayLists does not exceed the capacity of this ArrayList, it adds the items from the other ArrayList to this ArrayList.
     * If the total count of items in both ArrayLists exceeds the capacity of this ArrayList and its size type is STATIC, it prints an error message and does not add the items.
     * (only if debug flag is set to true)
     * @param other The ArrayList whose items should be added to this ArrayList.
     * @return true if the items were added successfully, false otherwise.
    */
    bool addAll(const ArrayList<T>& other) {
        if (sizeType == DYNAMIC && count + other.count > arrayCapacity) {
            resize();
        }
        if (count + other.count <= arrayCapacity) {
            memcpy(array + count, other.array, other.count * sizeof(T));
            count += other.count;
            return true;
        }
        return false;
    }

    /**
     * @brief Adds all items from an array to this ArrayList.
     *
     * This function adds all items from the specified array to this ArrayList. If the size type of this ArrayList is DYNAMIC and 
     * the total count of items in the ArrayList and the array exceeds the capacity of this ArrayList, it resizes this ArrayList to accommodate the new items.
     * If the total count of items in the ArrayList and the array does not exceed the capacity of this ArrayList, it adds the items from the array to this ArrayList.
     * If the total count of items in the ArrayList and the array exceeds the capacity of this ArrayList and its size type is STATIC, it prints an error message and does not add the items.
     *
     * @param other The array/List whose items should be added to this ArrayList.
     * @param length The length of the array/List.
     * @return true if the items were added successfully, false otherwise.
    */
    bool addAll(const T* other, size_t length) {
        if (sizeType == DYNAMIC && count + length > arrayCapacity) {
            resize();
        }
        if (count + length <= arrayCapacity) {
            memcpy(array + count, other, length * sizeof(T));
            count += length;
            return true;
        }
        return false;
    }

    /**
     * @brief Inserts an item at a specific index in the ArrayList.
     *
     * This function inserts the specified item at the specified index in the ArrayList. If the index is greater than the count of items in the ArrayList,
     * it prints an error message and returns false. If the ArrayList is full, it resizes the ArrayList if its size type is DYNAMIC, or prints an error message and returns false if its size type is STATIC.
     * If the index is valid and the ArrayList is not full, or has been resized successfully, it shifts all items from the index to the end of the ArrayList one position to the right, inserts the item at the index, and increments the count of items.
     *
     * @param index The index at which to insert the item.
     * @param item The item to insert.
     * @return true if the item was inserted successfully, false otherwise.
    */
    bool insert(size_t index, T item) {
        if (index > count) {
            return false;
        }
        if (count == arrayCapacity) {
            if (sizeType == DYNAMIC) {
                resize();
            } else {
                return false;
            }
        }
        for (size_t i = count; i > index; --i) {
            array[i] = array[i - 1];
        }
        array[index] = item;
        ++count;
        return true;
    }

    /**
     * @brief Inserts all items from another ArrayList at a specific index in this ArrayList.
     *
     * This function inserts all items from the specified ArrayList at the specified index in this ArrayList. If the index is greater than the count of items in this ArrayList,
     * it prints an error message and returns false. If the ArrayList is full, it resizes the ArrayList if its size type is DYNAMIC, or prints an error message and returns false if its size type is STATIC.
     * If the index is valid and the ArrayList is not full, or has been resized successfully, it shifts all items from the index to the end of the ArrayList one position to the right, inserts the items from the other ArrayList at the index, and increments the count of items.
     *
     * @param index The index at which to insert the items.
     * @param other The ArrayList whose items should be inserted.
     * @return true if the items were inserted successfully, false otherwise.
    */
    bool insertAll(size_t index, const ArrayList<T>& other) {
        if (index > count) {
            return false;
        }
        if (count + other.count > arrayCapacity) {
            if (sizeType == DYNAMIC) {
                resize();
            } else {
                return false;
            }
        }
        for (size_t i = count + other.count - 1; i >= index + other.count; --i) {
            array[i] = array[i - other.count];
        }
        memcpy(array + index, other.array, other.count * sizeof(T));
        count += other.count;
        return true;
    }

    /**
     * @brief Inserts all items from an array/List at a specific index in this ArrayList.
     *
     * This function inserts all items from the specified array/list at the specified index in this ArrayList. If the index is greater than the count of items in this ArrayList,
     * it prints an error message and returns false. If the ArrayList is full, it resizes the ArrayList if its size type is DYNAMIC, or prints an error message and returns false if its size type is STATIC.
     * If the index is valid and the ArrayList is not full, or has been resized successfully, it shifts all items from the index to the end of the ArrayList one position to the right, inserts the items from the array at the index, and increments the count of items.
     *
     * @param index The index at which to insert the items.
     * @param other The array/List whose items should be inserted.
     * @param length The length of the array/List.
     * @return true if the items were inserted successfully, false otherwise.
    */
    bool insertAll(size_t index, const T* other, size_t length) {
        if (index > count) {
            return false;
        }
        if (count + length > arrayCapacity) {
            if (sizeType == DYNAMIC) {
                resize();
            } else {
                return false;
            }
        }
        for (size_t i = count + length - 1; i >= index + length; --i) {
            array[i] = array[i - length];
        }
        memcpy(array + index, other, length * sizeof(T));
        count += length;
        return true;
    }
    
    /**
     * @brief Removes an item from the ArrayList.
     *
     * This function removes the specified item from the ArrayList. If the item is not found in the ArrayList, it prints an error message and returns false.
     * If the item is found in the ArrayList, it removes the item from the ArrayList and returns true.
     *
     * @param item The item to remove from the ArrayList.
     * @return true if the item was removed successfully, false otherwise.
    */
    bool removeItem(T item) {
        int index = indexOf(item);
        if (index >= 0) {
            remove(index);
            return true;
        }else{
            return false;
        }
    }
    
    /**
     * @brief Removes an item at a specific index from the ArrayList.
     *
     * This function removes the item at the specified index from the ArrayList. If the index is less than the count of items in the ArrayList,
     * it shifts all items from the index + 1 to the end of the ArrayList one position to the left, and decrements the count of items.
     *
     * @param index The index of the item to remove.
    */
    void remove(size_t index) {
        if (index < count) {
            for (size_t i = index; i < count - 1; ++i) {
                array[i] = array[i + 1];
            }
            --count;
        }
    }

    /**
     * @brief Removes all items from the ArrayList that satisfy a predicate.
     *
     * This function removes all items from the ArrayList that satisfy the specified predicate. The predicate is a function that takes an item of type T and returns a boolean.
     * If the predicate returns true for an item, that item is removed from the ArrayList. The function shifts all items that do not satisfy the predicate to the left to fill the gaps left by the removed items.
     * The function can use the Predicates library, which can be found at: https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/lib
     *
     * @param predicate The predicate to test the items against.
     * @return true if any items were removed, false otherwise.
    */
    bool removeIf(bool (*predicate)(T)) {
        size_t removed = 0;
        for (size_t i = 0; i < count; ++i) {
            if (predicate(array[i])) {
                ++removed;
            } else {
                array[i - removed] = array[i];
            }
        }
        if (removed > 0) {
            count -= removed;
            return true;
        }
        return false;
    }

    /**
     * @brief Removes a range of items from the ArrayList.
     *
     * This function removes a range of items from the ArrayList, from the specified fromIndex (inclusive) to the specified toIndex (exclusive).
     * If fromIndex is greater than toIndex, or if either index is out of bounds, it prints an error message and returns without removing any items.
     * If the indices are valid, it shifts all items from toIndex to the end of the ArrayList to the left to fill the gaps left by the removed items, and decrements the count of items by the number of removed items.
     *
     * @param fromIndex The start index of the range to remove (inclusive).
     * @param toIndex The end index of the range to remove (exclusive).
    */
    void removeRange(size_t fromIndex, size_t toIndex) {
        if (fromIndex > toIndex) {
            return;
        }
        if (fromIndex < 0 || toIndex > count) {
            return;
        }
        size_t removed = toIndex - fromIndex;
        for (size_t i = fromIndex; i < count - removed; ++i) {
            array[i] = array[i + removed];
        }
        count -= removed;
    }

    /**
     * @brief Retains only the items in this ArrayList that are contained in the specified ArrayList.
     *
     * This function removes from this ArrayList all of its items that are not contained in the specified ArrayList.
     * After this call, this ArrayList will contain only the items that are also contained in the other ArrayList.
     *
     * @param other The ArrayList containing the items to be retained in this ArrayList.
     * @return true if this ArrayList changed as a result of the call, false otherwise.
    */
    bool retainAll(const ArrayList<T>& other) {
        size_t removed = 0;
        for (size_t i = 0; i < count; ++i) {
            if (!other.contains(array[i])) {
                ++removed;
            } else {
                array[i - removed] = array[i];
            }
        }
        if (removed > 0) {
            count -= removed;
            return true;
        }
        return false;
    }

    /**
     * @brief Clears all items from the ArrayList.
     *
     * This function clears all items from the ArrayList and sets the count of items to 0.
    */
    void clear() {
        delete[] array;
        array = new T[arrayCapacity];
        count = 0;
    }

    //Query and Access
    
    /**
     * @brief Retrieves the item at a specific index in the ArrayList.
     *
     * This function retrieves the item at the specified index in the ArrayList. If the index is less than the count of items in the ArrayList,
     * it returns the item at the index. If the index is out of bounds, it returns a default-constructed instance of type T.
     *
     * @param index The index of the item to retrieve.
     * @return The item at the specified index, or a default-constructed instance of type T if the index is out of bounds.
    */
    T get(size_t index) const {
        if (index < count) {
            return array[index];
        }
        return T();// Return default value if index is out of bounds
    }

    /**
     * @brief Retrieves the item at a specific index in the ArrayList.
     *
     * This operator retrieves the item at the specified index in the ArrayList. If the index is less than the count of items in the ArrayList,
     * it returns the item at the index. If the index is out of bounds, it returns a default-constructed instance of type T.
     *
     * @param index The index of the item to retrieve.
     * @return The item at the specified index, or a default-constructed instance of type T if the index is out of bounds.
    */
    T operator[](size_t index) const {
        return get(index);
    }

    /**
     * @brief Retrieves the item at a specific index in the ArrayList as a String.
     *
     * This function retrieves the item at the specified index in the ArrayList and converts it to a String. If the index is less than the count of items in the ArrayList,
     * it returns the item at the index as a String. If the index is out of bounds, it returns a default-constructed instance of type T as a String.
     *
     * @param index The index of the item to retrieve.
     * @return The item at the specified index as a String, or a default-constructed instance of type T as a String if the index is out of bounds.
    */
    String getAsString(size_t index) const {
        if (index < count) {
            return toString(array[index]);
        }
        return toString(T());// Return default value if index is out of bounds
    }

    /**
     * @brief Converts a value of type T to a String.
     *
     * This function converts the specified value of type T to a String using the String constructor.
     * The type T must be a type that the String constructor can accept.
     *
     * @param value The value to convert to a String.
     * @return The String representation of the value.
    */
    String toString(const T& value) {
        return String(value);
    }

    /**
     * @brief Checks if the ArrayList contains an item.
     *
     * This function checks if the ArrayList contains the specified item. If the item is found in the ArrayList, it returns true.
     * If the item is not found in the ArrayList, it returns false.
     *
     * @param item The item to check for.
     * @return true if the ArrayList contains the item, false otherwise.
    */
    bool contains(T item) const {
        for (size_t i = 0; i < count; ++i) {
            if (array[i] == item) {
                return true;
            }
        }
        return false;
    }
    
    /**
    * @brief Finds the index of the first occurrence of the specified item in the ArrayList.
    *
    * This function returns the index of the first occurrence of the specified item in the ArrayList, or -1 if the ArrayList does not contain the item.
    * It iterates over the ArrayList from the beginning to the end, comparing each item with the specified item using the == operator.
    * If it finds a match, it returns the index of the match. If it does not find a match, it prints an error message (if debug is true) and returns -1.
    *
    * @param item The item to find in the ArrayList.
    * @return The index of the first occurrence of the item in the ArrayList, or -1 if the ArrayList does not contain the item.
    */
    size_t indexOf(T item) const {
        for (size_t i = 0; i < count; ++i) {
            if (array[i] == item) {
                return i;
            }
        }
        return static_cast<size_t>(-1); // Indicate not found
    } 

    /**
     * @brief Retrieves the capacity of the ArrayList.
     *
     * This function returns the current capacity of the ArrayList, which is the maximum number of items it can hold without resizing.
     *
     * @return The capacity of the ArrayList.
    */
    size_t capacity() const {
        return arrayCapacity;
    }

    /**
     * @brief Retrieves the count of items in the ArrayList.
     *
     * This function returns the current count of items in the ArrayList.
     *
     * @return The count of items in the ArrayList.
    */
    size_t size() const {
        return count;
    }

    /**
     * @brief Checks if the ArrayList is empty.
     *
     * This function checks if the ArrayList is empty. If the ArrayList is empty, it returns true. If the ArrayList is not empty, it returns false.
     *
     * @return true if the ArrayList is empty, false otherwise.
    */
    bool isEmpty() const {
        return count == 0;
    }

    /**
     * @brief Sets the item at a specific index in the ArrayList.
     *
     * This function sets the item at the specified index in the ArrayList to the specified item.
     * If the index is less than the count of items in the ArrayList, it sets the item at the index and checks if the item was set correctly.
     * If the item was set correctly, it returns true. If the index is out of bounds, it prints an error message (if debug is true) and returns false.
     *
     * @param index The index at which to set the item.
     * @param item The item to set.
     * @return true if the item was set correctly, false otherwise.
    */
    bool set(size_t index, T item) {
        if (index < count) {
            array[index] = item;
            if(array[index] == item){
                return true;
            }
        }
        if(index >= count){
            return false;
        }
    }

    /**
     * @brief Replaces each item in the ArrayList with the result of applying an operator function to that item.
     *
     * This function applies the specified operator function to each item in the ArrayList, and replaces the item with the result of the function.
     * The operator function is a function that takes an item of type T and returns a new item of type T.
     * The function uses the Operators library, which can be found at: https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/lib
     *
     * @note You can utilize any Operator function from the Operators library, or create your own.
     * @param operatorFunction The operator function to apply to each item.
    */
    void replaceAll(T (*operatorFunction)(T)) {
        for (size_t i = 0; i < count; ++i) {
            array[i] = operatorFunction(array[i]);
        }
    }


    //Utility Functions
    /**
     * @brief Performs an action on each item in the ArrayList.
     *
     * This function applies the specified consumer function to each item in the ArrayList.
     * The consumer function is a function that takes an item of type T and performs an action on it.
     * The function does not return a result.
     * The function can use the Operators library, if needed, which can be found at: https://github.com/braydenanderson2014/C-Arduino-Libraries/tree/main/lib
     *
     * @param consumer The consumer function to apply to each item.
    */
    void forEach(void (*consumer)(T)) const {
        for (size_t i = 0; i < count; ++i) {
            consumer(array[i]);
        }
    }

    /**
     * @brief Converts the ArrayList to an array.
     *
     * This function copies the items from the ArrayList into the specified output array.
     * The output array must be large enough to hold all items in the ArrayList.
     * The function uses the memcpy function to copy the items, so the type T must be trivially copyable.
     *
     * @param outputArray The array into which to copy the items.
     * @return The output array.
    */
    T* toArray(T* outputArray) const {
        memcpy(outputArray, array, count * sizeof(T));
        return outputArray;
    }

    /**
     * @brief Creates a sublist of the ArrayList.
     *
     * This function creates a new ArrayList that contains the items in the original ArrayList from the specified fromIndex, inclusive, to the specified toIndex, exclusive.
     * If fromIndex is greater than toIndex, or if either index is out of bounds, the function returns nullptr.
     *
     * @param fromIndex The start index of the sublist, inclusive.
     * @param toIndex The end index of the sublist, exclusive.
     * @return A new ArrayList that contains the items in the sublist, or nullptr if the indices are invalid.
    */
    ArrayList<T>* sublist(size_t fromIndex, size_t toIndex) const {
        if (fromIndex > toIndex) {
            return nullptr;
        }
        if (fromIndex < 0 || toIndex > count) {
            return nullptr;
        }
        ArrayList<T>* newList = new ArrayList<T>(sizeType, toIndex - fromIndex);
        for (size_t i = fromIndex; i < toIndex; ++i) {
            newList->add(array[i]);
        }
        return newList;
    }

    /**
     * @brief Creates a clone of the ArrayList.
     *
     * This function creates a new ArrayList that is a clone of the original ArrayList.
     * The new ArrayList has the same size type, capacity, and items as the original ArrayList.
     *
     * @return A new ArrayList that is a clone of the original ArrayList.
    */
    ArrayList<T>* clone() const {
        ArrayList<T>* clone = new ArrayList<T>(sizeType, arrayCapacity);
        clone->addAll(*this);
        return clone;
    }

    /**
     * @brief Ensures that the ArrayList can hold at least the specified number of items without needing to resize.
     *
     * This function checks if the ArrayList's current capacity is less than the specified minimum capacity.
     * If it is, it resizes the ArrayList to the specified minimum capacity.
     * The function uses the memcpy function to copy the items to the new array, so the type T must be trivially copyable.
     *
     * @param minCapacity The minimum capacity that the ArrayList should be able to hold without resizing.
    */
    void ensureCapacity(size_t minCapacity) {
        if (minCapacity > arrayCapacity) {
            T* newArray = new T[minCapacity];
            memcpy(newArray, array, count * sizeof(T));
            delete[] array;
            array = newArray;
            arrayCapacity = minCapacity;
        }
    }

    /**
     * @brief Trims the capacity of the ArrayList to its current size.
     *
     * This function reduces the capacity of the ArrayList to its current size, i.e., the number of items it contains.
     * If the ArrayList is dynamic and its capacity is greater than its size, it creates a new array with a capacity equal to the size, copies the items to the new array, and deletes the old array.
     * If the ArrayList is already trimmed or is fixed size, it prints an error message (if debug is true).
    */
    void trimToSize() {
        if (sizeType == DYNAMIC && count < arrayCapacity) {
            T* newArray = new T[count];
            memcpy(newArray, array, count * sizeof(T));
            delete[] array;
            array = newArray;
            arrayCapacity = count;
        }
    }

    /**
     * @brief Sorts the ArrayList.
     *
     * This function sorts the ArrayList using the specified comparator function and sorting algorithm.
     * The comparator function is a function that takes two items of type T and returns true if the first item is less than the second item, or false otherwise.
     * The sorting algorithm is an enum value that specifies the sorting algorithm to use. The available sorting algorithms are BUBBLE_SORT and QUICK_SORT.
     * The function uses the bubble sort algorithm by default.
     * The function can use the Predicates library, which can be found at:
    */ 
    void sort(bool (*comparator)(T, T), SortAlgorithm algorithm = BUBBLE_SORT) { 
        switch (algorithm) { 
            case BUBBLE_SORT: 
                bubbleSort(comparator); 
            break; 
            case QUICK_SORT: 
                quickSort(comparator, 0, count - 1); 
            break; // Add cases for additional sorting algorithms 
        } 
    }

    /**
     * @brief Sorts the ArrayList using the bubble sort algorithm.
     *
     * This function sorts the items in the ArrayList using the bubble sort algorithm.
     * The order of the items is determined by the specified comparator function.
     * The comparator function should take two items of type T and return true if the first item should come after the second item in the sorted ArrayList, and false otherwise.
     *
     * @param comparator The comparator function that determines the order of the items.
    */
    void bubbleSort(bool (*comparator)(T, T)) {
        for (size_t i = 0; i < count - 1; ++i) {
            for (size_t j = 0; j < count - i - 1; ++j) {
                if (comparator(array[j], array[j + 1])) {
                    T temp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temp;
                }
            }
        }
    }

    /**
     * @brief Sorts the ArrayList using the quick sort algorithm.
     *
     * This function sorts the items in the ArrayList using the quick sort algorithm.
     * The order of the items is determined by the specified comparator function.
     * The comparator function should take two items of type T and return true if the first item should come after the second item in the sorted ArrayList, and false otherwise.
     *
     * @param comparator The comparator function that determines the order of the items.
    */
    void quickSort(bool (*comparator)(T, T)) {
        quickSortHelper(comparator, 0, count - 1);
    }


    /**
     * @brief Returns a pointer to the first item in the ArrayList.
     *
     * This function returns a pointer to the first item in the ArrayList.
     * This can be used to iterate over the ArrayList using pointer arithmetic.
     *
     * @return A pointer to the first item in the ArrayList.
    */
    T* begin() const { 
        return &array[0]; 
    }
    
    /**
     * @brief Returns a pointer to the item after the last item in the ArrayList.
     *
     * This function returns a pointer to the item after the last item in the ArrayList.
     * This can be used to iterate over the ArrayList using pointer arithmetic.
     *
     * @return A pointer to the item after the last item in the ArrayList.
    */
    T* end() const { 
        return &array[count]; 
    }

private:
    T* array;
    SizeType sizeType;
    size_t arrayCapacity;
    size_t count;

    /**
     * @brief Resizes the ArrayList.
     *
     * This function resizes the ArrayList to 1.5 times its current capacity.
     * It creates a new array with the new capacity, copies the items to the new array, and deletes the old array.
     * If the new capacity is greater than the maximum size_t value, it prints an error message (if debug is true) and does not resize the ArrayList.
    */
    void resize() {
        size_t newCapacity = arrayCapacity * 1.5;
        T* newArray = new T[newCapacity];
        memcpy(newArray, array, count * sizeof(T));
        delete[] array;
        array = newArray;
        size_t tempCapacity = newCapacity;
        int oldCapacity = arrayCapacity;
        arrayCapacity = newCapacity;
        if(tempCapacity != arrayCapacity){
            arrayCapacity = oldCapacity;
        }
    }

    /**
     * @brief Sorts the ArrayList using the quick sort algorithm.
     *
     * This function sorts the items in the ArrayList using the quick sort algorithm.
     * The order of the items is determined by the specified comparator function.
     * The comparator function should take two items of type T and return true if the first item should come after the second item in the sorted ArrayList, and false otherwise.
     *
     * @param comparator The comparator function that determines the order of the items.
     * @param low The lowest index of the sublist to sort.
     * @param high The highest index of the sublist to sort.
    */
    int partition(bool (*comparator)(T, T), int low, int high) { 
        T pivot = array[high]; 
        int i = (low - 1); 
        for (int j = low; j <= high - 1; j++) { 
            if (comparator(array[j], pivot)) { 
                i++; 
                swap(&array[i], &array[j]); 
            } 
        } 
        swap(&array[i + 1], &array[high]); 
        return (i + 1); 
    } 

    /**
     * @brief Sorts the ArrayList using the quick sort algorithm.
     *
     * This function sorts the items in the ArrayList using the quick sort algorithm.
     * The order of the items is determined by the specified comparator function.
     * The comparator function should take two items of type T and return true if the first item should come after the second item in the sorted ArrayList, and false otherwise.
     *
     * @param comparator The comparator function that determines the order of the items.
     * @param low The lowest index of the sublist to sort.
     * @param high The highest index of the sublist to sort.
    */
    void quickSortHelper(bool (*comparator)(T, T), int low, int high) {
        if (low < high) { 
            int pi = partition(comparator, low, high); 
            quickSortHelper(comparator, low, pi - 1); 
            quickSortHelper(comparator, pi + 1, high);
        } 
    }
 
    /**
     * @brief Swaps two items in the ArrayList.
     *
     * This function swaps the items at the specified indices in the ArrayList.
     *
     * @param a The index of the first item to swap.
     * @param b The index of the second item to swap.
    */
    void swap(T* a, T* b) { 
        T t = *a; 
        *a = *b; 
        *b = t; 
    } 
};

#endif // ARRAYLIST_H