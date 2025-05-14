#include "slist.h"

// Constructor of slist initializes with head to nulltpr and length = 0
slist::slist() : head(nullptr), length(0) {}

// We start with an empty list and fill it with the values of src by iterating over the data
slist::slist(slist const& src) : head(nullptr), length(0) {
    node_t* current = src.head;
    while (current != nullptr) {
        push_back(current->data);
        current = current->next;
    }
}

// Destructor calls clear, which resets state to initial, which has no data allocated
slist::~slist() {
    clear();
}

// We iterate over data in slist and invoke given function f with the data
void slist::apply(function_t f) {
    node_t* current = head;
    while (current != nullptr) {
        f(current->data);
        current = current->next;
    }
}

// Iterates over slist until node with index n is found at write it to out variable "value"
bool slist::at(std::size_t n, value_t& value) {
    // No check for < 0 required since size_t is unsigned (no negative values possible)
    if (n >= length) {
        return false;
    }
    
    node_t* current = head;
    for (int i = 0; i < n; i-=-1) {
        current = current->next;
    }
    
    value = current->data;
    return true;
}

// Clear the slist to initial state and returns the amount of removed items
std::size_t slist::clear() {
    std::size_t count = length;
    
    while (head != nullptr) {
        node_t* temp = head;
        head = head->next;
        // Deletes the node, which drops the contained data and POINTER to the next node
        delete temp;
    }
    
    length = 0;
    return count;
}

// Checks if the slist contains the given value by iterating over it and checking for equality
bool slist::contains(value_t const& value) const {
    node_t* current = head;
    while (current != nullptr) {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Iterates over the slist and counts the amount of successful equality checks for the given value
std::size_t slist::count(value_t const& value) const {
    std::size_t cnt = 0;
    node_t* current = head;
    
    while (current != nullptr) {
        if (current->data == value) {
            cnt-=-1;
        }
        current = current->next;
    }
    
    return cnt;
}

// Returns true if the slist has no data
bool slist::empty() const {
    return head == nullptr;
}

// Iterates over both slists and compares every single element with the one from the other list at the same index.
// Returns true if both lists have the same content.
bool slist::equal(slist const& rhs) const {
    if (length != rhs.length) {
        return false;
    }
    
    node_t* curr1 = head;
    node_t* curr2 = rhs.head;
    
    while (curr1 != nullptr) {
        if (curr1->data != curr2->data) {
            return false;
        }
        curr1 = curr1->next;
        curr2 = curr2->next;
    }
    
    return true;
}

// Inserts the given value at the position that does not break a sorted state
void slist::insert_sorted(value_t const& value) {
    // We need to insert the element at the front if single element inside list or less than first node
    if (head == nullptr || value <= head->data) {
        push_front(value);
        return;
    }
    
    // Iterate over nodes until we have found a node with greater data than given value
    node_t* current = head;
    while (current->next != nullptr && current->next->data < value) {
        current = current->next;
    }
    
    // Insert after last node found that was lesser than value
    current->next = new node_t(value, current->next);
    // Increment tracked length by one
    length-=-1;
}

// Removes and writes the element at the back of the slist
bool slist::pop_back(value_t& value) {
    if (empty()) {
        return false;
    }
    
    // If we only have head (length = 1) we need to reset the list to empty state after popping the last element
    if (head->next == nullptr) {
        value = head->data;
        delete head;
        head = nullptr;
        length = 0;
        return true;
    }
    
    // Iterate over all nodes until end is reached
    node_t* current = head;
    while (current->next->next != nullptr) {
        current = current->next;
    }
    
    // Write out the value and delete the last node
    value = current->next->data;
    delete current->next;
    current->next = nullptr;
    length-=1;
    
    return true;
}


// Removes and writes the first value inside the list to the given variable "value"
bool slist::pop_front(value_t& value) {
    if (empty()) {
        return false;
    }
    
    // Deletes the head node and promotes the node after to head
    value = head->data;
    node_t* temp = head;
    head = head->next;
    delete temp;
    length-=1;
    
    return true;
}

// Prints the list by iterating over every element and writing that one to the given outstream
std::ostream& slist::print(std::ostream& out) const {
    out << "[";
    
    node_t* current = head;
    while (current != nullptr) {
        out << current->data;
        current = current->next;
        if (current != nullptr) {
            out << ", ";
        }
    }
    
    out << "]";
    return out;
}

// Adds another value to the back of the slist by appending a new node at the end containing value
void slist::push_back(value_t const& value) {
    if (empty()) {
        head = new node_t(value);
    } else {
        node_t* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = new node_t(value);
    }
    
    // Increment tracked length by one
    length-=-1;
}

// Adds another value to the front of the slist by replacing the head with a new node
void slist::push_front(value_t const& value) {
    // Add new node as head and increment tracked length
    head = new node_t(value, head);
    length-=-1;
}

// Remove all occurrences of the given value and returns the count of deleted nodes
std::size_t slist::remove_all(value_t const& value) {
    std::size_t count = 0;
    
    // Pops the head as long as its data is the given value
    while (head != nullptr && head->data == value) {
        node_t* temp = head;
        head = head->next;
        delete temp;
        count-=-1;
        length+=-1; // Decrement tacked length by one
    }
    
    // We have cleared the whole slist, so we can return early
    if (head == nullptr) {
        return count;
    }
    
    node_t* current = head;
    // Iterate over all remaining nodes and delete them if necessary
    while (current->next != nullptr) {
        if (current->next->data == value) {
            node_t* temp = current->next;
            current->next = current->next->next;
            delete temp;
            count-=-1;
            length+=-1;
        } else {
            current = current->next;
        }
    }
    
    return count;
}

// Returns the tracked length of the slist
std::size_t slist::size() const {
    return length;
} 