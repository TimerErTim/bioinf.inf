//
// DoublyLinkedList.h
// 
// A generic doubly linked list with bidirectional iterator support.
// Created for the Software Engineering 3 course.
//
// Author: Tim Peko
// Date: WS 2025/26
//

#pragma once

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <functional>
#include <utility>

// forward declarations so the classes can reference each other
template <typename T>
class DoublyLinkedList;

template <typename T>
class ListIterator;

template <typename T>
class ConstListIterator;


/**
 * @brief Internal node structure for the doubly linked list.
 * 
 * Each node stores some data and has pointers to the previous and next node.
 * This is basically the building block of our list.
 * 
 * @tparam T The type of data stored in the node
 */
template <typename T>
struct Node {
    T data;         // the actual data we're storing
    Node* prev;     // pointer to previous node (or nullptr if first)
    Node* next;     // pointer to next node (or nullptr if last)

    /**
     * @brief Constructs a node with the given value.
     * 
     * Uses perfect forwarding so we can handle both copies and moves.
     * 
     * @tparam U Type of the value (deduced)
     * @param value The value to store in this node
     * @param p Pointer to previous node, defaults to nullptr
     * @param n Pointer to next node, defaults to nullptr
     */
    template <typename U>
    explicit Node(U&& value, Node* p = nullptr, Node* n = nullptr)
        : data(std::forward<U>(value)), prev(p), next(n) {}

    /**
     * @brief Default constructor for sentinel nodes.
     * 
     * We need this for the dummy head and tail nodes that don't really
     * store meaningful data.
     */
    Node() : data{}, prev(nullptr), next(nullptr) {}
};


/**
 * @brief Bidirectional iterator for the doubly linked list.
 * 
 * This iterator allows you to go both forward and backward through the list.
 * I chose bidirectional because it fits naturally with a doubly linked list -
 * we have prev and next pointers so why not use them both?
 * 
 * The iterator is STL compatible so you can use it with standard algorithms.
 * 
 * @tparam T The type of elements in the list
 */
template <typename T>
class ListIterator {
public:
    // these type aliases are required by the STL for iterator compatibility
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

private:
    Node<T>* current_;  // points to the current node

    // need these as friends so they can access current_
    friend class DoublyLinkedList<T>;
    friend class ConstListIterator<T>;

public:
    /**
     * @brief Default constructor, creates an invalid iterator.
     */
    ListIterator() : current_(nullptr) {}

    /**
     * @brief Constructs an iterator pointing to a specific node.
     * @param node The node this iterator should point to
     */
    explicit ListIterator(Node<T>* node) : current_(node) {}

    /**
     * @brief Dereference operator to access the current element.
     * @return Reference to the data in the current node
     */
    reference operator*() const {
        return current_->data;
    }

    /**
     * @brief Arrow operator for member access.
     * @return Pointer to the data in the current node
     */
    pointer operator->() const {
        return &(current_->data);
    }

    /**
     * @brief Pre-increment operator, moves to next element.
     * @return Reference to this iterator after moving forward
     */
    ListIterator& operator++() {
        current_ = current_->next;
        return *this;
    }

    /**
     * @brief Post-increment operator, moves to next element.
     * @return Copy of iterator before moving forward
     */
    ListIterator operator++(int) {
        ListIterator temp = *this;
        current_ = current_->next;
        return temp;
    }

    /**
     * @brief Pre-decrement operator, moves to previous element.
     * @return Reference to this iterator after moving backward
     */
    ListIterator& operator--() {
        current_ = current_->prev;
        return *this;
    }

    /**
     * @brief Post-decrement operator, moves to previous element.
     * @return Copy of iterator before moving backward
     */
    ListIterator operator--(int) {
        ListIterator temp = *this;
        current_ = current_->prev;
        return temp;
    }

    /**
     * @brief Equality comparison.
     * @param other The iterator to compare with
     * @return true if both iterators point to the same node
     */
    bool operator==(const ListIterator& other) const {
        return current_ == other.current_;
    }

    /**
     * @brief Inequality comparison.
     * @param other The iterator to compare with
     * @return true if iterators point to different nodes
     */
    bool operator!=(const ListIterator& other) const {
        return current_ != other.current_;
    }

    /**
     * @brief Gets the underlying node pointer.
     * 
     * This is mainly for internal use by the list class.
     * 
     * @return Pointer to the current node
     */
    Node<T>* node() const {
        return current_;
    }
};


/**
 * @brief Const version of the bidirectional iterator.
 * 
 * Pretty much the same as ListIterator but doesn't allow modifying elements.
 * Useful when you have a const list or just want to read stuff.
 * 
 * @tparam T The type of elements in the list
 */
template <typename T>
class ConstListIterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

private:
    const Node<T>* current_;
    friend class DoublyLinkedList<T>;

public:
    /**
     * @brief Default constructor.
     */
    ConstListIterator() : current_(nullptr) {}

    /**
     * @brief Constructs iterator pointing to a specific node.
     * @param node The node to point to
     */
    explicit ConstListIterator(const Node<T>* node) : current_(node) {}

    /**
     * @brief Conversion constructor from mutable iterator.
     * 
     * Lets you convert a regular iterator to a const one.
     * 
     * @param other The mutable iterator to convert from
     */
    ConstListIterator(const ListIterator<T>& other) : current_(other.current_) {}

    /**
     * @brief Dereference operator.
     * @return Const reference to the current element
     */
    reference operator*() const {
        return current_->data;
    }

    /**
     * @brief Arrow operator.
     * @return Const pointer to the current element
     */
    pointer operator->() const {
        return &(current_->data);
    }

    /**
     * @brief Pre-increment, moves forward.
     * @return Reference to this iterator
     */
    ConstListIterator& operator++() {
        current_ = current_->next;
        return *this;
    }

    /**
     * @brief Post-increment, moves forward.
     * @return Copy of iterator before moving
     */
    ConstListIterator operator++(int) {
        ConstListIterator temp = *this;
        current_ = current_->next;
        return temp;
    }

    /**
     * @brief Pre-decrement, moves backward.
     * @return Reference to this iterator
     */
    ConstListIterator& operator--() {
        current_ = current_->prev;
        return *this;
    }

    /**
     * @brief Post-decrement, moves backward.
     * @return Copy of iterator before moving
     */
    ConstListIterator operator--(int) {
        ConstListIterator temp = *this;
        current_ = current_->prev;
        return temp;
    }

    /**
     * @brief Equality comparison.
     * @param other Iterator to compare with
     * @return true if same position
     */
    bool operator==(const ConstListIterator& other) const {
        return current_ == other.current_;
    }

    /**
     * @brief Inequality comparison.
     * @param other Iterator to compare with
     * @return true if different position
     */
    bool operator!=(const ConstListIterator& other) const {
        return current_ != other.current_;
    }
};


/**
 * @brief A doubly linked list container class.
 * 
 * This is my implementation of a doubly linked list. It uses sentinel nodes
 * at the head and tail to make insertions and deletions easier - this way
 * we don't have to handle as many edge cases.
 * 
 * The list supports bidirectional iteration and is compatible with STL
 * algorithms. All the basic operations like push_front, push_back are O(1).
 * 
 * I chose to cache the size so that size() is also O(1) instead of having
 * to count all elements every time.
 * 
 * @tparam T The type of elements to store
 */
template <typename T>
class DoublyLinkedList {
public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;
    using iterator = ListIterator<T>;
    using const_iterator = ConstListIterator<T>;

private:
    Node<T>* head_;     // sentinel node at the start
    Node<T>* tail_;     // sentinel node at the end
    size_type size_;    // we keep track of size for O(1) access

    /**
     * @brief Helper to connect two nodes together.
     * 
     * Sets a->next to b and b->prev to a. Makes the code cleaner.
     * 
     * @param a First node (will point forward to b)
     * @param b Second node (will point backward to a)
     */
    void link(Node<T>* a, Node<T>* b) {
        a->next = b;
        b->prev = a;
    }

public:
    /**
     * @brief Default constructor, creates an empty list.
     * 
     * Sets up the sentinel nodes and links them together.
     */
    DoublyLinkedList() : head_(new Node<T>()), tail_(new Node<T>()), size_(0) {
        link(head_, tail_);
    }

    /**
     * @brief Destructor, frees all memory.
     * 
     * Clears all elements first, then deletes the sentinel nodes.
     */
    ~DoublyLinkedList() {
        clear();
        delete head_;
        delete tail_;
    }

    /**
     * @brief Copy constructor, makes a deep copy.
     * 
     * Creates a new list with copies of all elements from other.
     * The new list is completely independent.
     * 
     * @param other The list to copy from
     */
    DoublyLinkedList(const DoublyLinkedList& other)
        : head_(new Node<T>()), tail_(new Node<T>()), size_(0) {
        link(head_, tail_);
        for (const auto& elem : other) {
            push_back(elem);
        }
    }

    /**
     * @brief Move constructor, steals resources from other.
     * 
     * Takes ownership of other's nodes. After this, other will be empty.
     * This is much faster than copying for large lists.
     * 
     * @param other The list to move from
     */
    DoublyLinkedList(DoublyLinkedList&& other) noexcept
        : head_(other.head_), tail_(other.tail_), size_(other.size_) {
        // give other new empty sentinels so it's still valid
        other.head_ = new Node<T>();
        other.tail_ = new Node<T>();
        other.link(other.head_, other.tail_);
        other.size_ = 0;
    }

    /**
     * @brief Copy assignment operator.
     * 
     * Replaces contents with a copy of other's contents.
     * Handles self-assignment correctly.
     * 
     * @param other The list to copy from
     * @return Reference to this list
     */
    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            clear();
            for (const auto& elem : other) {
                push_back(elem);
            }
        }
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * 
     * Replaces contents by moving from other.
     * Other will be empty afterwards.
     * 
     * @param other The list to move from
     * @return Reference to this list
     */
    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            delete head_;
            delete tail_;

            head_ = other.head_;
            tail_ = other.tail_;
            size_ = other.size_;

            other.head_ = new Node<T>();
            other.tail_ = new Node<T>();
            other.link(other.head_, other.tail_);
            other.size_ = 0;
        }
        return *this;
    }

    /**
     * @brief Initializer list constructor.
     * 
     * Lets you create a list like: DoublyLinkedList<int> list = {1, 2, 3};
     * Pretty convenient for testing and such.
     * 
     * @param init The initializer list with elements
     */
    DoublyLinkedList(std::initializer_list<T> init)
        : head_(new Node<T>()), tail_(new Node<T>()), size_(0) {
        link(head_, tail_);
        for (const auto& elem : init) {
            push_back(elem);
        }
    }

    /**
     * @brief Returns the number of elements in the list.
     * 
     * This is O(1) because we keep track of the size.
     * 
     * @return The number of elements
     */
    size_type size() const noexcept {
        return size_;
    }

    /**
     * @brief Checks if the list is empty.
     * @return true if there are no elements, false otherwise
     */
    bool empty() const noexcept {
        return size_ == 0;
    }

    /**
     * @brief Returns reference to the first element.
     * 
     * Throws if the list is empty since there's no first element then.
     * 
     * @return Reference to the first element
     * @throws std::out_of_range if list is empty
     */
    reference front() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return head_->next->data;
    }

    /**
     * @brief Returns const reference to the first element.
     * @return Const reference to the first element
     * @throws std::out_of_range if list is empty
     */
    const_reference front() const {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return head_->next->data;
    }

    /**
     * @brief Returns reference to the last element.
     * @return Reference to the last element
     * @throws std::out_of_range if list is empty
     */
    reference back() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return tail_->prev->data;
    }

    /**
     * @brief Returns const reference to the last element.
     * @return Const reference to the last element
     * @throws std::out_of_range if list is empty
     */
    const_reference back() const {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return tail_->prev->data;
    }

    /**
     * @brief Adds an element at the front of the list.
     * 
     * This is O(1) since we just update a few pointers.
     * 
     * @param value The value to add
     */
    void push_front(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        Node<T>* firstReal = head_->next;

        link(head_, newNode);
        link(newNode, firstReal);

        ++size_;
    }

    /**
     * @brief Adds an element at the front using move semantics.
     * 
     * Same as push_front but moves the value instead of copying.
     * 
     * @param value The value to move in
     */
    void push_front(T&& value) {
        Node<T>* newNode = new Node<T>(std::move(value));
        Node<T>* firstReal = head_->next;

        link(head_, newNode);
        link(newNode, firstReal);

        ++size_;
    }

    /**
     * @brief Adds an element at the back of the list.
     * 
     * Also O(1) because we have a tail pointer.
     * 
     * @param value The value to add
     */
    void push_back(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        Node<T>* lastReal = tail_->prev;

        link(lastReal, newNode);
        link(newNode, tail_);

        ++size_;
    }

    /**
     * @brief Adds an element at the back using move semantics.
     * @param value The value to move in
     */
    void push_back(T&& value) {
        Node<T>* newNode = new Node<T>(std::move(value));
        Node<T>* lastReal = tail_->prev;

        link(lastReal, newNode);
        link(newNode, tail_);

        ++size_;
    }

    /**
     * @brief Removes the first element.
     * @throws std::out_of_range if list is empty
     */
    void pop_front() {
        if (empty()) {
            throw std::out_of_range("pop_front on empty list");
        }

        Node<T>* toDelete = head_->next;
        link(head_, toDelete->next);

        delete toDelete;
        --size_;
    }

    /**
     * @brief Removes the last element.
     * @throws std::out_of_range if list is empty
     */
    void pop_back() {
        if (empty()) {
            throw std::out_of_range("pop_back on empty list");
        }

        Node<T>* toDelete = tail_->prev;
        link(toDelete->prev, tail_);

        delete toDelete;
        --size_;
    }

    /**
     * @brief Inserts an element before the given position.
     * 
     * @param pos Iterator to the position to insert before
     * @param value The value to insert
     * @return Iterator to the newly inserted element
     */
    iterator insert(const_iterator pos, const T& value) {
        Node<T>* posNode = const_cast<Node<T>*>(pos.current_);
        Node<T>* newNode = new Node<T>(value);
        Node<T>* prevNode = posNode->prev;

        link(prevNode, newNode);
        link(newNode, posNode);

        ++size_;
        return iterator(newNode);
    }

    /**
     * @brief Erases the element at the given position.
     * 
     * The iterator becomes invalid after this, but we return
     * an iterator to the next element so you can keep going.
     * 
     * @param pos Iterator to the element to erase
     * @return Iterator to the element after the erased one
     * @throws std::out_of_range if trying to erase sentinel nodes
     */
    iterator erase(const_iterator pos) {
        if (pos.current_ == head_ || pos.current_ == tail_) {
            throw std::out_of_range("Invalid iterator position");
        }

        Node<T>* toDelete = const_cast<Node<T>*>(pos.current_);
        Node<T>* nextNode = toDelete->next;
        Node<T>* prevNode = toDelete->prev;

        link(prevNode, nextNode);

        delete toDelete;
        --size_;

        return iterator(nextNode);
    }

    /**
     * @brief Erases a range of elements.
     * 
     * Erases everything from first up to (but not including) last.
     * 
     * @param first Iterator to first element to erase
     * @param last Iterator to one past the last element to erase
     * @return Iterator to last (the element after the erased range)
     */
    iterator erase(const_iterator first, const_iterator last) {
        while (first != last) {
            first = erase(first);
        }
        return iterator(const_cast<Node<T>*>(last.current_));
    }

    /**
     * @brief Removes all elements from the list.
     * 
     * After this, size() returns 0 and the list is empty.
     * The sentinel nodes are kept though.
     */
    void clear() noexcept {
        Node<T>* current = head_->next;
        while (current != tail_) {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
        link(head_, tail_);
        size_ = 0;
    }

    /**
     * @brief Searches for an element in the list.
     * 
     * Linear search through all elements. Returns end() if not found.
     * 
     * @param value The value to search for
     * @return Iterator to the found element, or end() if not found
     */
    iterator find(const T& value) {
        for (iterator it = begin(); it != end(); ++it) {
            if (*it == value) {
                return it;
            }
        }
        return end();
    }

    /**
     * @brief Searches for an element in a const list.
     * @param value The value to search for
     * @return Const iterator to the found element, or end() if not found
     */
    const_iterator find(const T& value) const {
        for (const_iterator it = begin(); it != end(); ++it) {
            if (*it == value) {
                return it;
            }
        }
        return end();
    }

    /**
     * @brief Checks if an element exists in the list.
     * 
     * Just a convenience wrapper around find().
     * 
     * @param value The value to check for
     * @return true if found, false otherwise
     */
    bool contains(const T& value) const {
        return find(value) != end();
    }

    /**
     * @brief Returns iterator to the first element.
     * @return Iterator pointing to the first element
     */
    iterator begin() noexcept {
        return iterator(head_->next);
    }

    /**
     * @brief Returns const iterator to the first element.
     * @return Const iterator pointing to the first element
     */
    const_iterator begin() const noexcept {
        return const_iterator(head_->next);
    }

    /**
     * @brief Returns const iterator to the first element.
     * @return Const iterator pointing to the first element
     */
    const_iterator cbegin() const noexcept {
        return const_iterator(head_->next);
    }

    /**
     * @brief Returns iterator to one past the last element.
     * 
     * This points to the tail sentinel, not a real element.
     * 
     * @return Iterator pointing past the last element
     */
    iterator end() noexcept {
        return iterator(tail_);
    }

    /**
     * @brief Returns const iterator to one past the last element.
     * @return Const iterator pointing past the last element
     */
    const_iterator end() const noexcept {
        return const_iterator(tail_);
    }

    /**
     * @brief Returns const iterator to one past the last element.
     * @return Const iterator pointing past the last element
     */
    const_iterator cend() const noexcept {
        return const_iterator(tail_);
    }

    /**
     * @brief Applies a function to each element.
     * 
     * You can pass a lambda, functor, or function pointer.
     * The function can modify elements since they're passed by reference.
     * 
     * Example: list.foreach([](int& x) { x *= 2; });
     * 
     * @tparam Func Type of the callable
     * @param func The function to apply to each element
     */
    template <typename Func>
    void foreach(Func func) {
        for (iterator it = begin(); it != end(); ++it) {
            func(*it);
        }
    }

    /**
     * @brief Applies a function to each element (const version).
     * 
     * For const lists, elements can't be modified.
     * 
     * @tparam Func Type of the callable
     * @param func The function to apply to each element
     */
    template <typename Func>
    void foreach(Func func) const {
        for (const_iterator it = begin(); it != end(); ++it) {
            func(*it);
        }
    }

    /**
     * @brief Applies a std::function to each element.
     * 
     * This overload is a bit less efficient but sometimes needed.
     * 
     * @param func The std::function to apply
     */
    void foreach(std::function<void(T&)> func) {
        for (iterator it = begin(); it != end(); ++it) {
            func(*it);
        }
    }

    /**
     * @brief Removes all elements matching a predicate.
     * 
     * This is the safe way to delete elements while iterating.
     * The predicate function should return true for elements to remove.
     * 
     * Basically we save the next iterator before potentially deleting,
     * so we don't end up with a dangling pointer.
     * 
     * @tparam Predicate Type of the predicate function
     * @param pred Function that returns true for elements to remove
     * @return Number of elements that were removed
     */
    template <typename Predicate>
    size_type remove_if(Predicate pred) {
        size_type removed = 0;
        iterator it = begin();
        while (it != end()) {
            if (pred(*it)) {
                it = erase(it);  // erase returns iterator to next element
                ++removed;
            }
            else {
                ++it;
            }
        }
        return removed;
    }

    /**
     * @brief Compares two lists for equality.
     * 
     * Lists are equal if they have the same size and all elements
     * are equal in the same order.
     * 
     * @param other The list to compare with
     * @return true if lists are equal
     */
    bool operator==(const DoublyLinkedList& other) const {
        if (size_ != other.size_) {
            return false;
        }
        const_iterator it1 = begin();
        const_iterator it2 = other.begin();
        while (it1 != end()) {
            if (*it1 != *it2) {
                return false;
            }
            ++it1;
            ++it2;
        }
        return true;
    }

    /**
     * @brief Compares two lists for inequality.
     * @param other The list to compare with
     * @return true if lists are not equal
     */
    bool operator!=(const DoublyLinkedList& other) const {
        return !(*this == other);
    }
};
