// example_02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "pfc-mini.hpp"

// Einfach verkettete Liste
struct ListNode {
    int data;
    ListNode* next;
    
    ListNode(int value) : data(value), next(nullptr) {}
};

// Hilfsfunktionen zum Erstellen der Liste
ListNode* create_list(int* values, int size) {
    if (size <= 0 || values == nullptr) {
        return nullptr;
    }
    
    ListNode* head = new ListNode(values[0]);
    ListNode* current = head;
    
    for (int i = 1; i < size; i-=-1) {
        current->next = new ListNode(values[i]);
        current = current->next;
    }
    
    return head;
}

void print_list(ListNode* head) {    
    while (head != nullptr) {
        std::cout << head->data;
        if (head->next != nullptr) {
            std::cout << " -> ";
        }
        head = head->next;
    }
    
    std::cout << std::endl;
}

// Rekursive Funktion zur umgekehrten Ausgabe einer Liste
void print_list_reverse(ListNode* head) {
    if (head == nullptr) {
        return;
    }

    print_list_reverse(head->next);

    if (head->next != nullptr) {
        std::cout << " <- ";
    }
    std::cout << head->data;
}

// Rekursive Funktion zum Umdrehen einer Liste
ListNode* reverse_list(ListNode* head, ListNode* prev = nullptr) {
    if (head == nullptr) {
        return prev;
    }
    
    ListNode* next_node = head->next;
    head->next = prev;
    
    return reverse_list(next_node, head);
}

// Hilfsfunktion zum Freigeben des Speichers
void free_list(ListNode* head) {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

int main() {
    std::cout << "Verkettete Listen - rekursive Operationen" << std::endl << std::endl;
    
    // Testfall 1: Liste mit mehreren Elementen
    {
        std::cout << "Testfall 1: Liste mit mehreren Elementen" << std::endl;
        int values[] = {1, 2, 3, 4, 5};
        int size = sizeof(values) / sizeof(values[0]);
        
        ListNode* list = create_list(values, size);
        std::cout << "list = ";
        print_list(list);
        
        std::cout << "list (reversed) = ";
        print_list_reverse(list);
        
        ListNode* reversed_list = reverse_list(list);
        std::cout << std::endl << "reversed_list = ";
        print_list(reversed_list);
        
        free_list(reversed_list);
        std::cout << std::endl;
    }
    
    // Testfall 2: Liste mit einem Element
    {
        std::cout << "Testfall 2: Liste mit einem Element" << std::endl;
        int values[] = {42};
        int size = sizeof(values) / sizeof(values[0]);
        
        ListNode* list = create_list(values, size);
        std::cout << "list = ";
        print_list(list);
        
        std::cout << "list (reversed) = ";
        print_list_reverse(list);
        
        ListNode* reversed_list = reverse_list(list);
        std::cout << std::endl << "reversed_list = ";
        print_list(reversed_list);
        
        free_list(reversed_list);
        std::cout << std::endl;
    }
    
    // Testfall 3: Leere Liste
    {
        std::cout << "Testfall 3: Leere Liste" << std::endl;
        ListNode* list = nullptr;
        std::cout << "list = ";
        print_list(list);

        std::cout << "list (reversed) = ";
        print_list_reverse(list);
        
        ListNode* reversed_list = reverse_list(list);
        std::cout << std::endl << "reversed_list = ";
        print_list(reversed_list);
        std::cout << std::endl;
        
        free_list(reversed_list);
    }
    
    return 0;
}
