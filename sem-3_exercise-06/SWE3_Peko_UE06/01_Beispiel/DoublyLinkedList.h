//
// DoublyLinkedList.h
// 
// Eine generische, doppelt verkettete Liste mit bidirektionalem Iterator.
// Implementiert für den Kurs "Software Engineering 3".
//
// Autor: Tim Peko
// Datum: WS 2025/26
//

#pragma once

#include <cstddef>      // std::size_t, std::ptrdiff_t
#include <iterator>     // std::bidirectional_iterator_tag
#include <stdexcept>    // std::out_of_range
#include <functional>   // std::function
#include <utility>      // std::move, std::forward

namespace DLL {

    // ============================================================================
    // Vorwärtsdeklarationen
    // ============================================================================
    template <typename T>
    class DoublyLinkedList;

    template <typename T>
    class ListIterator;

    template <typename T>
    class ConstListIterator;

    // ============================================================================
    // Node - Interner Knoten der doppelt verketteten Liste
    // ============================================================================
    // 
    // Struktur:
    //   +------+    +------+    +------+
    //   | prev |<-->| Node |<-->| next |
    //   +------+    +------+    +------+
    //              | data |
    //              +------+
    //
    // Jeder Knoten enthält:
    // - data: Das gespeicherte Element vom Typ T
    // - prev: Zeiger auf den vorherigen Knoten (nullptr für den ersten Knoten)
    // - next: Zeiger auf den nächsten Knoten (nullptr für den letzten Knoten)
    // ============================================================================
    template <typename T>
    struct Node {
        T data;
        Node* prev;
        Node* next;

        // Konstruktor mit perfekter Weiterleitung
        template <typename U>
        explicit Node(U&& value, Node* p = nullptr, Node* n = nullptr)
            : data(std::forward<U>(value)), prev(p), next(n) {}

        // Default-Konstruktor für Sentinel-Knoten
        Node() : data{}, prev(nullptr), next(nullptr) {}
    };

    // ============================================================================
    // ListIterator - Bidirektionaler Iterator (mutable)
    // ============================================================================
    //
    // Design-Entscheidung: Bidirektionaler Iterator
    // 
    // Ein bidirektionaler Iterator wurde gewählt, weil:
    // 1. Er natürlich zur Struktur einer doppelt verketteten Liste passt
    // 2. Er sowohl Vorwärts- als auch Rückwärtsiteration ermöglicht
    // 3. Er mit STL-Algorithmen kompatibel ist, die bidirektionale Iteratoren benötigen
    //
    // Iterator-Kategorie: std::bidirectional_iterator_tag
    // 
    // Operationen:
    // - ++it, it++: Bewegung zum nächsten Element
    // - --it, it--: Bewegung zum vorherigen Element
    // - *it: Zugriff auf das aktuelle Element
    // - it->: Zeigerzugriff auf das aktuelle Element
    // - it1 == it2, it1 != it2: Vergleichsoperationen
    // ============================================================================
    template <typename T>
    class ListIterator {
    public:
        // STL-konformer Iterator-Typ-Alias (C++ Standard Library Konventionen)
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

    private:
        Node<T>* current_;
        friend class DoublyLinkedList<T>;
        friend class ConstListIterator<T>;

    public:
        // Konstruktoren
        ListIterator() : current_(nullptr) {}
        explicit ListIterator(Node<T>* node) : current_(node) {}

        // Dereferenzierung
        reference operator*() const {
            return current_->data;
        }

        pointer operator->() const {
            return &(current_->data);
        }

        // Inkrement (Präfix)
        ListIterator& operator++() {
            current_ = current_->next;
            return *this;
        }

        // Inkrement (Postfix)
        ListIterator operator++(int) {
            ListIterator temp = *this;
            current_ = current_->next;
            return temp;
        }

        // Dekrement (Präfix)
        ListIterator& operator--() {
            current_ = current_->prev;
            return *this;
        }

        // Dekrement (Postfix)
        ListIterator operator--(int) {
            ListIterator temp = *this;
            current_ = current_->prev;
            return temp;
        }

        // Vergleichsoperatoren
        bool operator==(const ListIterator& other) const {
            return current_ == other.current_;
        }

        bool operator!=(const ListIterator& other) const {
            return current_ != other.current_;
        }

        // Zugriff auf den internen Knoten (für die Liste)
        Node<T>* node() const {
            return current_;
        }
    };

    // ============================================================================
    // ConstListIterator - Bidirektionaler Iterator (const)
    // ============================================================================
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
        ConstListIterator() : current_(nullptr) {}
        explicit ConstListIterator(const Node<T>* node) : current_(node) {}

        // Konvertierung von mutable Iterator
        ConstListIterator(const ListIterator<T>& other) : current_(other.current_) {}

        reference operator*() const {
            return current_->data;
        }

        pointer operator->() const {
            return &(current_->data);
        }

        ConstListIterator& operator++() {
            current_ = current_->next;
            return *this;
        }

        ConstListIterator operator++(int) {
            ConstListIterator temp = *this;
            current_ = current_->next;
            return temp;
        }

        ConstListIterator& operator--() {
            current_ = current_->prev;
            return *this;
        }

        ConstListIterator operator--(int) {
            ConstListIterator temp = *this;
            current_ = current_->prev;
            return temp;
        }

        bool operator==(const ConstListIterator& other) const {
            return current_ == other.current_;
        }

        bool operator!=(const ConstListIterator& other) const {
            return current_ != other.current_;
        }
    };

    // ============================================================================
    // DoublyLinkedList - Doppelt verkettete Liste
    // ============================================================================
    //
    // Interne Struktur:
    //
    //     head_                                              tail_
    //       |                                                  |
    //       v                                                  v
    //    +------+     +------+     +------+     +------+    +------+
    //    |Sentl.|<--->| Node |<--->| Node |<--->| Node |<-->|Sentl.|
    //    | head |     |  A   |     |  B   |     |  C   |    | tail |
    //    +------+     +------+     +------+     +------+    +------+
    //
    // Design-Entscheidungen:
    // 
    // 1. Sentinel-Knoten: Wir verwenden Sentinel-Knoten am Anfang (head_) und
    //    Ende (tail_), um Sonderfälle bei Einfüge- und Löschoperationen zu
    //    vermeiden. Dies vereinfacht den Code erheblich.
    //
    // 2. Template-basiert: Die Liste ist als Template implementiert, um
    //    beliebige Datentypen zu unterstützen.
    //
    // 3. Bidirektionaler Iterator: Der Iterator ermöglicht sowohl Vorwärts-
    //    als auch Rückwärtsiteration und ist STL-kompatibel.
    //
    // 4. O(1) Komplexität: push_front und push_back sind in O(1) implementiert,
    //    da wir direkten Zugriff auf head_ und tail_ haben.
    //
    // Komplexitätsanalyse:
    // - push_front: O(1)
    // - push_back:  O(1)
    // - size:       O(1) (Größe wird gecached)
    // - find:       O(n)
    // - begin/end:  O(1)
    // - foreach:    O(n)
    // ============================================================================
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
        Node<T>* head_;     // Sentinel-Knoten am Anfang
        Node<T>* tail_;     // Sentinel-Knoten am Ende
        size_type size_;    // Gecachte Größe für O(1) Abfrage

        // Hilfsmethode: Verbindet zwei Knoten
        void link(Node<T>* a, Node<T>* b) {
            a->next = b;
            b->prev = a;
        }

    public:
        // ========================================================================
        // Konstruktoren und Destruktor
        // ========================================================================

        // Default-Konstruktor: Erstellt eine leere Liste
        DoublyLinkedList() : head_(new Node<T>()), tail_(new Node<T>()), size_(0) {
            link(head_, tail_);
        }

        // Destruktor: Gibt alle Knoten frei
        ~DoublyLinkedList() {
            clear();
            delete head_;
            delete tail_;
        }

        // Copy-Konstruktor
        DoublyLinkedList(const DoublyLinkedList& other)
            : head_(new Node<T>()), tail_(new Node<T>()), size_(0) {
            link(head_, tail_);
            for (const auto& elem : other) {
                push_back(elem);
            }
        }

        // Move-Konstruktor
        DoublyLinkedList(DoublyLinkedList&& other) noexcept
            : head_(other.head_), tail_(other.tail_), size_(other.size_) {
            other.head_ = new Node<T>();
            other.tail_ = new Node<T>();
            other.link(other.head_, other.tail_);
            other.size_ = 0;
        }

        // Copy-Zuweisung
        DoublyLinkedList& operator=(const DoublyLinkedList& other) {
            if (this != &other) {
                clear();
                for (const auto& elem : other) {
                    push_back(elem);
                }
            }
            return *this;
        }

        // Move-Zuweisung
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

        // Initializer-List-Konstruktor
        DoublyLinkedList(std::initializer_list<T> init)
            : head_(new Node<T>()), tail_(new Node<T>()), size_(0) {
            link(head_, tail_);
            for (const auto& elem : init) {
                push_back(elem);
            }
        }

        // ========================================================================
        // Kapazität
        // ========================================================================

        // Gibt die Anzahl der Elemente zurück - O(1)
        size_type size() const noexcept {
            return size_;
        }

        // Prüft, ob die Liste leer ist
        bool empty() const noexcept {
            return size_ == 0;
        }

        // ========================================================================
        // Elementzugriff
        // ========================================================================

        // Zugriff auf das erste Element
        reference front() {
            if (empty()) {
                throw std::out_of_range("Liste ist leer");
            }
            return head_->next->data;
        }

        const_reference front() const {
            if (empty()) {
                throw std::out_of_range("Liste ist leer");
            }
            return head_->next->data;
        }

        // Zugriff auf das letzte Element
        reference back() {
            if (empty()) {
                throw std::out_of_range("Liste ist leer");
            }
            return tail_->prev->data;
        }

        const_reference back() const {
            if (empty()) {
                throw std::out_of_range("Liste ist leer");
            }
            return tail_->prev->data;
        }

        // ========================================================================
        // Modifikatoren
        // ========================================================================

        // Einfügen am Anfang - O(1)
        void push_front(const T& value) {
            Node<T>* newNode = new Node<T>(value);
            Node<T>* firstReal = head_->next;

            link(head_, newNode);
            link(newNode, firstReal);

            ++size_;
        }

        // Einfügen am Anfang (Move-Semantik) - O(1)
        void push_front(T&& value) {
            Node<T>* newNode = new Node<T>(std::move(value));
            Node<T>* firstReal = head_->next;

            link(head_, newNode);
            link(newNode, firstReal);

            ++size_;
        }

        // Einfügen am Ende - O(1)
        void push_back(const T& value) {
            Node<T>* newNode = new Node<T>(value);
            Node<T>* lastReal = tail_->prev;

            link(lastReal, newNode);
            link(newNode, tail_);

            ++size_;
        }

        // Einfügen am Ende (Move-Semantik) - O(1)
        void push_back(T&& value) {
            Node<T>* newNode = new Node<T>(std::move(value));
            Node<T>* lastReal = tail_->prev;

            link(lastReal, newNode);
            link(newNode, tail_);

            ++size_;
        }

        // Entfernen am Anfang
        void pop_front() {
            if (empty()) {
                throw std::out_of_range("pop_front auf leerer Liste");
            }

            Node<T>* toDelete = head_->next;
            link(head_, toDelete->next);

            delete toDelete;
            --size_;
        }

        // Entfernen am Ende
        void pop_back() {
            if (empty()) {
                throw std::out_of_range("pop_back auf leerer Liste");
            }

            Node<T>* toDelete = tail_->prev;
            link(toDelete->prev, tail_);

            delete toDelete;
            --size_;
        }

        // Einfügen vor der Position des Iterators
        iterator insert(const_iterator pos, const T& value) {
            Node<T>* posNode = const_cast<Node<T>*>(pos.current_);
            Node<T>* newNode = new Node<T>(value);
            Node<T>* prevNode = posNode->prev;

            link(prevNode, newNode);
            link(newNode, posNode);

            ++size_;
            return iterator(newNode);
        }

        // Entfernen an der Position des Iterators
        // Gibt einen Iterator auf das nächste Element zurück
        iterator erase(const_iterator pos) {
            if (pos.current_ == head_ || pos.current_ == tail_) {
                throw std::out_of_range("Ungültige Iterator-Position");
            }

            Node<T>* toDelete = const_cast<Node<T>*>(pos.current_);
            Node<T>* nextNode = toDelete->next;
            Node<T>* prevNode = toDelete->prev;

            link(prevNode, nextNode);

            delete toDelete;
            --size_;

            return iterator(nextNode);
        }

        // Entfernen eines Bereichs
        iterator erase(const_iterator first, const_iterator last) {
            while (first != last) {
                first = erase(first);
            }
            return iterator(const_cast<Node<T>*>(last.current_));
        }

        // Löscht alle Elemente
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

        // ========================================================================
        // Suche
        // ========================================================================

        // Sucht ein Element und gibt einen Iterator darauf zurück - O(n)
        iterator find(const T& value) {
            for (iterator it = begin(); it != end(); ++it) {
                if (*it == value) {
                    return it;
                }
            }
            return end();
        }

        // Const-Version der Suche
        const_iterator find(const T& value) const {
            for (const_iterator it = begin(); it != end(); ++it) {
                if (*it == value) {
                    return it;
                }
            }
            return end();
        }

        // Prüft, ob ein Element in der Liste enthalten ist
        bool contains(const T& value) const {
            return find(value) != end();
        }

        // ========================================================================
        // Iteratoren
        // ========================================================================

        // Iterator zum ersten Element
        iterator begin() noexcept {
            return iterator(head_->next);
        }

        const_iterator begin() const noexcept {
            return const_iterator(head_->next);
        }

        const_iterator cbegin() const noexcept {
            return const_iterator(head_->next);
        }

        // Iterator hinter dem letzten Element (Sentinel)
        iterator end() noexcept {
            return iterator(tail_);
        }

        const_iterator end() const noexcept {
            return const_iterator(tail_);
        }

        const_iterator cend() const noexcept {
            return const_iterator(tail_);
        }

        // Reverse Iteratoren (nutzt std::reverse_iterator)
        // Nicht implementiert, da außerhalb der Anforderungen

        // ========================================================================
        // Foreach-Methode
        // ========================================================================
        //
        // Design-Entscheidung: Die foreach-Methode akzeptiert jeden aufrufbaren
        // Typ (Funktor, Lambda, Funktionszeiger) durch Template-Parameter.
        // Dies ermöglicht maximale Flexibilität und Effizienz (Inline-Optimierung).
        //
        // Beispielverwendung:
        //   list.foreach([](int& x) { x *= 2; });           // Lambda
        //   list.foreach(MyFunctor{});                       // Funktor
        //   list.foreach(&myFunction);                       // Funktionszeiger
        // ========================================================================

        // Foreach mit Funktor/Lambda/Funktionszeiger (mutable)
        template <typename Func>
        void foreach(Func func) {
            for (iterator it = begin(); it != end(); ++it) {
                func(*it);
            }
        }

        // Foreach mit Funktor/Lambda/Funktionszeiger (const)
        template <typename Func>
        void foreach(Func func) const {
            for (const_iterator it = begin(); it != end(); ++it) {
                func(*it);
            }
        }

        // Alternative: foreach mit std::function (weniger effizient, aber flexibel)
        void foreach(std::function<void(T&)> func) {
            for (iterator it = begin(); it != end(); ++it) {
                func(*it);
            }
        }

        // ========================================================================
        // Sichere Iteration mit Löschung
        // ========================================================================
        //
        // Diese Methode ermöglicht sicheres Löschen während der Iteration.
        // Der Prädikat-Funktor entscheidet, ob ein Element gelöscht werden soll.
        // Gibt die Anzahl der gelöschten Elemente zurück.
        //
        // Implementierung: Wir speichern den nächsten Iterator VOR dem Aufruf
        // des Prädikats und vor dem potentiellen Löschen.
        // ========================================================================
        template <typename Predicate>
        size_type remove_if(Predicate pred) {
            size_type removed = 0;
            iterator it = begin();
            while (it != end()) {
                if (pred(*it)) {
                    it = erase(it);
                    ++removed;
                }
                else {
                    ++it;
                }
            }
            return removed;
        }

        // ========================================================================
        // Vergleichsoperatoren
        // ========================================================================

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

        bool operator!=(const DoublyLinkedList& other) const {
            return !(*this == other);
        }
    };

} // namespace DLL

