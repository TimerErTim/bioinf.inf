//
// test.cpp
//
// Umfassende Unit-Tests für die DoublyLinkedList-Implementierung.
// Tests folgen dem AAA-Prinzip (Arrange-Act-Assert).
//
// Autor: Tim Peko
// Datum: WS 2025/26
//

#include "pch.h"
#include "DoublyLinkedList.h"
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <type_traits>
#include <memory>

using namespace DLL;

// ============================================================================
// TEST FIXTURE
// ============================================================================

class DoublyLinkedListTest : public ::testing::Test {
protected:
    DoublyLinkedList<int> emptyList;
    DoublyLinkedList<int> singleElementList;
    DoublyLinkedList<int> multiElementList;

    void SetUp() override {
        singleElementList.push_back(42);
        
        multiElementList.push_back(1);
        multiElementList.push_back(2);
        multiElementList.push_back(3);
        multiElementList.push_back(4);
        multiElementList.push_back(5);
    }
};

// ============================================================================
// KONSTRUKTOR-TESTS
// ============================================================================

TEST(DoublyLinkedListConstructor, DefaultConstructor_CreatesEmptyList) {
    // Arrange & Act
    DoublyLinkedList<int> list;

    // Assert
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

TEST(DoublyLinkedListConstructor, InitializerListConstructor_CreatesListWithElements) {
    // Arrange & Act
    DoublyLinkedList<int> list = {1, 2, 3, 4, 5};

    // Assert
    EXPECT_EQ(list.size(), 5);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 5);
}

TEST(DoublyLinkedListConstructor, InitializerListConstructor_EmptyInitializerList) {
    // Arrange & Act
    DoublyLinkedList<int> list = {};

    // Assert
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0);
}

TEST(DoublyLinkedListConstructor, CopyConstructor_CreatesIndependentCopy) {
    // Arrange
    DoublyLinkedList<int> original = {1, 2, 3};

    // Act
    DoublyLinkedList<int> copy(original);

    // Assert
    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy, original);

    // Verifiziere Unabhängigkeit
    copy.push_back(4);
    EXPECT_NE(copy.size(), original.size());
}

TEST(DoublyLinkedListConstructor, CopyConstructor_EmptyList) {
    // Arrange
    DoublyLinkedList<int> original;

    // Act
    DoublyLinkedList<int> copy(original);

    // Assert
    EXPECT_TRUE(copy.empty());
    EXPECT_EQ(copy, original);
}

TEST(DoublyLinkedListConstructor, MoveConstructor_TransfersOwnership) {
    // Arrange
    DoublyLinkedList<int> original = {1, 2, 3};

    // Act
    DoublyLinkedList<int> moved(std::move(original));

    // Assert
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.front(), 1);
    EXPECT_EQ(moved.back(), 3);
    EXPECT_TRUE(original.empty()); // Ursprüngliche Liste ist jetzt leer
}

TEST(DoublyLinkedListConstructor, CopyAssignment_CopiesToExistingList) {
    // Arrange
    DoublyLinkedList<int> original = {1, 2, 3};
    DoublyLinkedList<int> target = {10, 20};

    // Act
    target = original;

    // Assert
    EXPECT_EQ(target.size(), 3);
    EXPECT_EQ(target, original);
}

TEST(DoublyLinkedListConstructor, CopyAssignment_SelfAssignment) {
    // Arrange
    DoublyLinkedList<int> list = {1, 2, 3};

    // Act
    list = list;

    // Assert
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST(DoublyLinkedListConstructor, MoveAssignment_TransfersOwnership) {
    // Arrange
    DoublyLinkedList<int> original = {1, 2, 3};
    DoublyLinkedList<int> target = {10, 20};

    // Act
    target = std::move(original);

    // Assert
    EXPECT_EQ(target.size(), 3);
    EXPECT_EQ(target.front(), 1);
    EXPECT_TRUE(original.empty());
}

// ============================================================================
// PUSH_FRONT TESTS
// ============================================================================

TEST_F(DoublyLinkedListTest, PushFront_OnEmptyList_AddsElement) {
    // Arrange - emptyList ist bereits leer

    // Act
    emptyList.push_front(42);

    // Assert
    EXPECT_EQ(emptyList.size(), 1);
    EXPECT_EQ(emptyList.front(), 42);
    EXPECT_EQ(emptyList.back(), 42);
}

TEST_F(DoublyLinkedListTest, PushFront_OnNonEmptyList_AddsAtBeginning) {
    // Arrange - multiElementList hat bereits {1, 2, 3, 4, 5}

    // Act
    multiElementList.push_front(0);

    // Assert
    EXPECT_EQ(multiElementList.size(), 6);
    EXPECT_EQ(multiElementList.front(), 0);
    EXPECT_EQ(multiElementList.back(), 5);
}

TEST_F(DoublyLinkedListTest, PushFront_MultipleElements_MaintainsOrder) {
    // Arrange
    DoublyLinkedList<int> list;

    // Act
    list.push_front(3);
    list.push_front(2);
    list.push_front(1);

    // Assert
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST_F(DoublyLinkedListTest, PushFront_MoveSemantics_MovesValue) {
    // Arrange
    DoublyLinkedList<std::string> list;
    std::string str = "Hello, World!";

    // Act
    list.push_front(std::move(str));

    // Assert
    EXPECT_EQ(list.front(), "Hello, World!");
    EXPECT_TRUE(str.empty()); // String wurde verschoben
}

// ============================================================================
// PUSH_BACK TESTS
// ============================================================================

TEST_F(DoublyLinkedListTest, PushBack_OnEmptyList_AddsElement) {
    // Arrange - emptyList ist bereits leer

    // Act
    emptyList.push_back(42);

    // Assert
    EXPECT_EQ(emptyList.size(), 1);
    EXPECT_EQ(emptyList.front(), 42);
    EXPECT_EQ(emptyList.back(), 42);
}

TEST_F(DoublyLinkedListTest, PushBack_OnNonEmptyList_AddsAtEnd) {
    // Arrange - multiElementList hat bereits {1, 2, 3, 4, 5}

    // Act
    multiElementList.push_back(6);

    // Assert
    EXPECT_EQ(multiElementList.size(), 6);
    EXPECT_EQ(multiElementList.front(), 1);
    EXPECT_EQ(multiElementList.back(), 6);
}

TEST_F(DoublyLinkedListTest, PushBack_MultipleElements_MaintainsOrder) {
    // Arrange
    DoublyLinkedList<int> list;

    // Act
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    // Assert
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list.front(), 1);
    EXPECT_EQ(list.back(), 3);
}

TEST_F(DoublyLinkedListTest, PushBack_MoveSemantics_MovesValue) {
    // Arrange
    DoublyLinkedList<std::string> list;
    std::string str = "Hello, World!";

    // Act
    list.push_back(std::move(str));

    // Assert
    EXPECT_EQ(list.back(), "Hello, World!");
    EXPECT_TRUE(str.empty());
}

// ============================================================================
// SIZE TESTS
// ============================================================================

TEST_F(DoublyLinkedListTest, Size_EmptyList_ReturnsZero) {
    EXPECT_EQ(emptyList.size(), 0);
}

TEST_F(DoublyLinkedListTest, Size_SingleElement_ReturnsOne) {
    EXPECT_EQ(singleElementList.size(), 1);
}

TEST_F(DoublyLinkedListTest, Size_MultipleElements_ReturnsCorrectCount) {
    EXPECT_EQ(multiElementList.size(), 5);
}

TEST_F(DoublyLinkedListTest, Size_AfterPushAndPop_ReturnsCorrectCount) {
    // Arrange
    DoublyLinkedList<int> list;

    // Act & Assert
    list.push_back(1);
    EXPECT_EQ(list.size(), 1);

    list.push_back(2);
    EXPECT_EQ(list.size(), 2);

    list.pop_back();
    EXPECT_EQ(list.size(), 1);

    list.pop_front();
    EXPECT_EQ(list.size(), 0);
}

// ============================================================================
// EMPTY TESTS
// ============================================================================

TEST_F(DoublyLinkedListTest, Empty_EmptyList_ReturnsTrue) {
    EXPECT_TRUE(emptyList.empty());
}

TEST_F(DoublyLinkedListTest, Empty_NonEmptyList_ReturnsFalse) {
    EXPECT_FALSE(singleElementList.empty());
    EXPECT_FALSE(multiElementList.empty());
}

TEST_F(DoublyLinkedListTest, Empty_AfterClear_ReturnsTrue) {
    // Arrange
    DoublyLinkedList<int> list = {1, 2, 3};

    // Act
    list.clear();

    // Assert
    EXPECT_TRUE(list.empty());
}

// ============================================================================
// FIND TESTS
// ============================================================================

TEST_F(DoublyLinkedListTest, Find_ExistingElement_ReturnsValidIterator) {
    // Arrange - multiElementList hat {1, 2, 3, 4, 5}

    // Act
    auto it = multiElementList.find(3);

    // Assert
    EXPECT_NE(it, multiElementList.end());
    EXPECT_EQ(*it, 3);
}

TEST_F(DoublyLinkedListTest, Find_NonExistingElement_ReturnsEndIterator) {
    // Arrange - multiElementList hat {1, 2, 3, 4, 5}

    // Act
    auto it = multiElementList.find(42);

    // Assert
    EXPECT_EQ(it, multiElementList.end());
}

TEST_F(DoublyLinkedListTest, Find_EmptyList_ReturnsEndIterator) {
    // Act
    auto it = emptyList.find(42);

    // Assert
    EXPECT_EQ(it, emptyList.end());
}

TEST_F(DoublyLinkedListTest, Find_FirstElement_ReturnsBeginIterator) {
    // Arrange - multiElementList hat {1, 2, 3, 4, 5}

    // Act
    auto it = multiElementList.find(1);

    // Assert
    EXPECT_EQ(it, multiElementList.begin());
    EXPECT_EQ(*it, 1);
}

TEST_F(DoublyLinkedListTest, Find_LastElement_ReturnsValidIterator) {
    // Arrange - multiElementList hat {1, 2, 3, 4, 5}

    // Act
    auto it = multiElementList.find(5);

    // Assert
    EXPECT_NE(it, multiElementList.end());
    EXPECT_EQ(*it, 5);
}

TEST_F(DoublyLinkedListTest, Find_DuplicateElements_ReturnsFirstOccurrence) {
    // Arrange
    DoublyLinkedList<int> list = {1, 2, 3, 2, 4};

    // Act
    auto it = list.find(2);

    // Assert
    EXPECT_NE(it, list.end());
    EXPECT_EQ(*it, 2);

    // Verifiziere, dass es die erste Vorkommnis ist
    ++it;
    EXPECT_EQ(*it, 3);
}

TEST_F(DoublyLinkedListTest, Find_WithStrings_FindsCorrectly) {
    // Arrange
    DoublyLinkedList<std::string> list = {"apple", "banana", "cherry"};

    // Act
    auto it = list.find("banana");

    // Assert
    EXPECT_NE(it, list.end());
    EXPECT_EQ(*it, "banana");
}

TEST_F(DoublyLinkedListTest, Find_ConstList_ReturnsConstIterator) {
    // Arrange
    const DoublyLinkedList<int> constList = {1, 2, 3};

    // Act
    auto it = constList.find(2);

    // Assert
    EXPECT_NE(it, constList.end());
    EXPECT_EQ(*it, 2);
    
    // Statische Prüfung, dass es ein const_iterator ist
    static_assert(std::is_same<decltype(it), DoublyLinkedList<int>::const_iterator>::value, 
                  "find auf const Liste sollte const_iterator zurückgeben");
}

// ============================================================================
// CONTAINS TESTS
// ============================================================================

TEST_F(DoublyLinkedListTest, Contains_ExistingElement_ReturnsTrue) {
    EXPECT_TRUE(multiElementList.contains(3));
}

TEST_F(DoublyLinkedListTest, Contains_NonExistingElement_ReturnsFalse) {
    EXPECT_FALSE(multiElementList.contains(42));
}

TEST_F(DoublyLinkedListTest, Contains_EmptyList_ReturnsFalse) {
    EXPECT_FALSE(emptyList.contains(1));
}

// ============================================================================
// FRONT UND BACK TESTS
// ============================================================================

TEST_F(DoublyLinkedListTest, Front_NonEmptyList_ReturnsFirstElement) {
    EXPECT_EQ(multiElementList.front(), 1);
}

TEST_F(DoublyLinkedListTest, Back_NonEmptyList_ReturnsLastElement) {
    EXPECT_EQ(multiElementList.back(), 5);
}

TEST_F(DoublyLinkedListTest, Front_EmptyList_ThrowsException) {
    EXPECT_THROW(emptyList.front(), std::out_of_range);
}

TEST_F(DoublyLinkedListTest, Back_EmptyList_ThrowsException) {
    EXPECT_THROW(emptyList.back(), std::out_of_range);
}

TEST_F(DoublyLinkedListTest, Front_Modifiable_ChangesElement) {
    // Arrange
    DoublyLinkedList<int> list = {1, 2, 3};

    // Act
    list.front() = 100;

    // Assert
    EXPECT_EQ(list.front(), 100);
}

TEST_F(DoublyLinkedListTest, Back_Modifiable_ChangesElement) {
    // Arrange
    DoublyLinkedList<int> list = {1, 2, 3};

    // Act
    list.back() = 100;

    // Assert
    EXPECT_EQ(list.back(), 100);
}

// ============================================================================
// POP_FRONT UND POP_BACK TESTS
// ============================================================================

TEST_F(DoublyLinkedListTest, PopFront_NonEmptyList_RemovesFirstElement) {
    // Arrange - multiElementList hat {1, 2, 3, 4, 5}

    // Act
    multiElementList.pop_front();

    // Assert
    EXPECT_EQ(multiElementList.size(), 4);
    EXPECT_EQ(multiElementList.front(), 2);
}

TEST_F(DoublyLinkedListTest, PopBack_NonEmptyList_RemovesLastElement) {
    // Arrange - multiElementList hat {1, 2, 3, 4, 5}

    // Act
    multiElementList.pop_back();

    // Assert
    EXPECT_EQ(multiElementList.size(), 4);
    EXPECT_EQ(multiElementList.back(), 4);
}

TEST_F(DoublyLinkedListTest, PopFront_SingleElement_MakesListEmpty) {
    // Arrange - singleElementList hat {42}

    // Act
    singleElementList.pop_front();

    // Assert
    EXPECT_TRUE(singleElementList.empty());
}

TEST_F(DoublyLinkedListTest, PopBack_SingleElement_MakesListEmpty) {
    // Arrange
    DoublyLinkedList<int> list = {42};

    // Act
    list.pop_back();

    // Assert
    EXPECT_TRUE(list.empty());
}

TEST_F(DoublyLinkedListTest, PopFront_EmptyList_ThrowsException) {
    EXPECT_THROW(emptyList.pop_front(), std::out_of_range);
}

TEST_F(DoublyLinkedListTest, PopBack_EmptyList_ThrowsException) {
    EXPECT_THROW(emptyList.pop_back(), std::out_of_range);
}

// ============================================================================
// CLEAR TESTS
// ============================================================================

TEST_F(DoublyLinkedListTest, Clear_NonEmptyList_MakesListEmpty) {
    // Arrange - multiElementList hat Elemente

    // Act
    multiElementList.clear();

    // Assert
    EXPECT_TRUE(multiElementList.empty());
    EXPECT_EQ(multiElementList.size(), 0);
}

TEST_F(DoublyLinkedListTest, Clear_EmptyList_RemainsEmpty) {
    // Act
    emptyList.clear();

    // Assert
    EXPECT_TRUE(emptyList.empty());
}

TEST_F(DoublyLinkedListTest, Clear_ListCanBeReused) {
    // Arrange
    DoublyLinkedList<int> list = {1, 2, 3};
    list.clear();

    // Act
    list.push_back(10);
    list.push_front(5);

    // Assert
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 5);
    EXPECT_EQ(list.back(), 10);
}

// ============================================================================
// ITERATOR TESTS - begin() und end()
// ============================================================================

TEST_F(DoublyLinkedListTest, Begin_NonEmptyList_ReturnsIteratorToFirstElement) {
    auto it = multiElementList.begin();
    EXPECT_EQ(*it, 1);
}

TEST_F(DoublyLinkedListTest, End_NonEmptyList_ReturnsIteratorPastLastElement) {
    auto it = multiElementList.end();
    --it;
    EXPECT_EQ(*it, 5);
}

TEST_F(DoublyLinkedListTest, BeginEnd_EmptyList_AreEqual) {
    EXPECT_EQ(emptyList.begin(), emptyList.end());
}

TEST_F(DoublyLinkedListTest, CBegin_ReturnsConstIterator) {
    const auto& list = multiElementList;
    auto it = list.cbegin();
    EXPECT_EQ(*it, 1);
    
    static_assert(std::is_same<decltype(it), DoublyLinkedList<int>::const_iterator>::value,
                  "cbegin sollte const_iterator zurückgeben");
}

// ============================================================================
// ITERATOR TESTS - Operationen
// ============================================================================

TEST_F(DoublyLinkedListTest, Iterator_PreIncrement_MovesToNextElement) {
    auto it = multiElementList.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
}

TEST_F(DoublyLinkedListTest, Iterator_PostIncrement_MovesToNextElement) {
    auto it = multiElementList.begin();
    auto old = it++;
    EXPECT_EQ(*old, 1);
    EXPECT_EQ(*it, 2);
}

TEST_F(DoublyLinkedListTest, Iterator_PreDecrement_MovesToPreviousElement) {
    auto it = multiElementList.end();
    --it;
    EXPECT_EQ(*it, 5);
    --it;
    EXPECT_EQ(*it, 4);
}

TEST_F(DoublyLinkedListTest, Iterator_PostDecrement_MovesToPreviousElement) {
    auto it = multiElementList.end();
    --it;
    auto old = it--;
    EXPECT_EQ(*old, 5);
    EXPECT_EQ(*it, 4);
}

TEST_F(DoublyLinkedListTest, Iterator_ArrowOperator_AccessesMember) {
    DoublyLinkedList<std::string> list = {"hello"};
    auto it = list.begin();
    EXPECT_EQ(it->length(), 5);
}

TEST_F(DoublyLinkedListTest, Iterator_Equality_ComparesCorrectly) {
    auto it1 = multiElementList.begin();
    auto it2 = multiElementList.begin();
    EXPECT_EQ(it1, it2);
    
    ++it1;
    EXPECT_NE(it1, it2);
}

TEST_F(DoublyLinkedListTest, Iterator_BidirectionalTraversal_WorksCorrectly) {
    // Arrange
    DoublyLinkedList<int> list = {1, 2, 3};
    
    // Act - Vorwärts durchlaufen
    auto it = list.begin();
    EXPECT_EQ(*it++, 1);
    EXPECT_EQ(*it++, 2);
    EXPECT_EQ(*it++, 3);
    EXPECT_EQ(it, list.end());
    
    // Act - Rückwärts durchlaufen
    --it;
    EXPECT_EQ(*it--, 3);
    EXPECT_EQ(*it--, 2);
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(it, list.begin());
}

TEST_F(DoublyLinkedListTest, Iterator_Modification_ChangesListElement) {
    auto it = multiElementList.begin();
    *it = 100;
    EXPECT_EQ(multiElementList.front(), 100);
}

// ============================================================================
// ITERATOR TESTS - STL-Kompatibilität
// ============================================================================

TEST_F(DoublyLinkedListTest, Iterator_Category_IsBidirectional) {
    using iterator = DoublyLinkedList<int>::iterator;
    static_assert(std::is_same<iterator::iterator_category, std::bidirectional_iterator_tag>::value,
                  "Iterator-Kategorie sollte bidirectional_iterator_tag sein");
}

TEST_F(DoublyLinkedListTest, Iterator_TypeAliases_AreCorrect) {
    using iterator = DoublyLinkedList<int>::iterator;
    static_assert(std::is_same<iterator::value_type, int>::value, "value_type");
    static_assert(std::is_same<iterator::reference, int&>::value, "reference");
    static_assert(std::is_same<iterator::pointer, int*>::value, "pointer");
    static_assert(std::is_same<iterator::difference_type, std::ptrdiff_t>::value, "difference_type");
}

TEST_F(DoublyLinkedListTest, ConstIterator_TypeAliases_AreCorrect) {
    using const_iterator = DoublyLinkedList<int>::const_iterator;
    static_assert(std::is_same<const_iterator::value_type, int>::value, "value_type");
    static_assert(std::is_same<const_iterator::reference, const int&>::value, "reference");
    static_assert(std::is_same<const_iterator::pointer, const int*>::value, "pointer");
}

// ============================================================================
// RANGE-BASED FOR LOOP TESTS
// ============================================================================

TEST_F(DoublyLinkedListTest, RangeBasedFor_TraversesAllElements) {
    std::vector<int> result;
    
    for (int x : multiElementList) {
        result.push_back(x);
    }
    
    EXPECT_EQ(result, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST_F(DoublyLinkedListTest, RangeBasedFor_ModifiesElements) {
    for (int& x : multiElementList) {
        x *= 2;
    }
    
    std::vector<int> result;
    for (int x : multiElementList) {
        result.push_back(x);
    }
    
    EXPECT_EQ(result, (std::vector<int>{2, 4, 6, 8, 10}));
}

TEST_F(DoublyLinkedListTest, RangeBasedFor_EmptyList_NoIterations) {
    int count = 0;
    for (int x : emptyList) {
        (void)x;
        ++count;
    }
    EXPECT_EQ(count, 0);
}

// ============================================================================
// FOREACH TESTS
// ============================================================================

TEST_F(DoublyLinkedListTest, Foreach_Lambda_AppliesFunction) {
    // Arrange
    int sum = 0;
    
    // Act
    multiElementList.foreach([&sum](int x) { sum += x; });
    
    // Assert
    EXPECT_EQ(sum, 15); // 1+2+3+4+5 = 15
}

TEST_F(DoublyLinkedListTest, Foreach_Lambda_ModifiesElements) {
    // Act
    multiElementList.foreach([](int& x) { x *= 2; });
    
    // Assert
    EXPECT_EQ(multiElementList.front(), 2);
    EXPECT_EQ(multiElementList.back(), 10);
}

TEST_F(DoublyLinkedListTest, Foreach_EmptyList_NoInvocations) {
    int count = 0;
    emptyList.foreach([&count](int) { ++count; });
    EXPECT_EQ(count, 0);
}

// Funktor-Klasse für Tests
struct Doubler {
    void operator()(int& x) const {
        x *= 2;
    }
};

TEST_F(DoublyLinkedListTest, Foreach_Functor_AppliesFunction) {
    // Arrange
    DoublyLinkedList<int> list = {1, 2, 3};
    
    // Act
    list.foreach(Doubler{});
    
    // Assert
    EXPECT_EQ(list.front(), 2);
    EXPECT_EQ(list.back(), 6);
}

// Freie Funktion für Tests
void tripler(int& x) {
    x *= 3;
}

TEST_F(DoublyLinkedListTest, Foreach_FunctionPointer_AppliesFunction) {
    // Arrange
    DoublyLinkedList<int> list = {1, 2, 3};
    
    // Act
    list.foreach(&tripler);
    
    // Assert
    EXPECT_EQ(list.front(), 3);
    EXPECT_EQ(list.back(), 9);
}

TEST_F(DoublyLinkedListTest, Foreach_ConstList_OnlyReadsElements) {
    // Arrange
    const DoublyLinkedList<int> constList = {1, 2, 3, 4, 5};
    int sum = 0;
    
    // Act
    constList.foreach([&sum](int x) { sum += x; });
    
    // Assert
    EXPECT_EQ(sum, 15);
}

TEST_F(DoublyLinkedListTest, Foreach_WithCapturingLambda_AccessesExternalState) {
    // Arrange
    std::vector<int> collected;
    
    // Act
    multiElementList.foreach([&collected](int x) {
        collected.push_back(x * x);
    });
    
    // Assert
    EXPECT_EQ(collected, (std::vector<int>{1, 4, 9, 16, 25}));
}

TEST_F(DoublyLinkedListTest, Foreach_StdFunction_AppliesFunction) {
    // Arrange
    DoublyLinkedList<int> list = {1, 2, 3};
    std::function<void(int&)> func = [](int& x) { x += 10; };
    
    // Act
    list.foreach(func);
    
    // Assert
    EXPECT_EQ(list.front(), 11);
    EXPECT_EQ(list.back(), 13);
}

// ============================================================================
// INSERT TESTS
// ============================================================================

TEST_F(DoublyLinkedListTest, Insert_AtBegin_AddsAtStart) {
    // Arrange
    auto it = multiElementList.begin();
    
    // Act
    auto result = multiElementList.insert(it, 0);
    
    // Assert
    EXPECT_EQ(multiElementList.size(), 6);
    EXPECT_EQ(multiElementList.front(), 0);
    EXPECT_EQ(*result, 0);
}

TEST_F(DoublyLinkedListTest, Insert_AtEnd_AddsAtEnd) {
    // Arrange
    auto it = multiElementList.end();
    
    // Act
    auto result = multiElementList.insert(it, 6);
    
    // Assert
    EXPECT_EQ(multiElementList.size(), 6);
    EXPECT_EQ(multiElementList.back(), 6);
    EXPECT_EQ(*result, 6);
}

TEST_F(DoublyLinkedListTest, Insert_InMiddle_InsertsCorrectly) {
    // Arrange
    DoublyLinkedList<int> list = {1, 3};
    auto it = list.begin();
    ++it; // Zeigt auf 3
    
    // Act
    list.insert(it, 2);
    
    // Assert
    std::vector<int> result;
    for (int x : list) {
        result.push_back(x);
    }
    EXPECT_EQ(result, (std::vector<int>{1, 2, 3}));
}

// ============================================================================
// ERASE TESTS
// ============================================================================

TEST_F(DoublyLinkedListTest, Erase_FirstElement_RemovesCorrectly) {
    // Arrange
    auto it = multiElementList.begin();
    
    // Act
    auto next = multiElementList.erase(it);
    
    // Assert
    EXPECT_EQ(multiElementList.size(), 4);
    EXPECT_EQ(multiElementList.front(), 2);
    EXPECT_EQ(*next, 2);
}

TEST_F(DoublyLinkedListTest, Erase_LastElement_RemovesCorrectly) {
    // Arrange
    auto it = multiElementList.end();
    --it; // Zeigt auf 5
    
    // Act
    auto next = multiElementList.erase(it);
    
    // Assert
    EXPECT_EQ(multiElementList.size(), 4);
    EXPECT_EQ(multiElementList.back(), 4);
    EXPECT_EQ(next, multiElementList.end());
}

TEST_F(DoublyLinkedListTest, Erase_MiddleElement_RemovesCorrectly) {
    // Arrange
    auto it = multiElementList.begin();
    ++it;
    ++it; // Zeigt auf 3
    
    // Act
    auto next = multiElementList.erase(it);
    
    // Assert
    EXPECT_EQ(multiElementList.size(), 4);
    EXPECT_EQ(*next, 4);
    EXPECT_FALSE(multiElementList.contains(3));
}

TEST_F(DoublyLinkedListTest, Erase_Range_RemovesMultipleElements) {
    // Arrange
    auto first = multiElementList.begin();
    ++first; // Zeigt auf 2
    auto last = first;
    ++last;
    ++last; // Zeigt auf 4
    
    // Act
    auto result = multiElementList.erase(first, last);
    
    // Assert
    EXPECT_EQ(multiElementList.size(), 3);
    EXPECT_EQ(*result, 4);
    
    std::vector<int> remaining;
    for (int x : multiElementList) {
        remaining.push_back(x);
    }
    EXPECT_EQ(remaining, (std::vector<int>{1, 4, 5}));
}

// ============================================================================
// REMOVE_IF TESTS (Sicheres Löschen während Iteration)
// ============================================================================

TEST_F(DoublyLinkedListTest, RemoveIf_RemovesMatchingElements) {
    // Arrange - multiElementList hat {1, 2, 3, 4, 5}
    
    // Act - Entferne alle geraden Zahlen
    size_t removed = multiElementList.remove_if([](int x) { return x % 2 == 0; });
    
    // Assert
    EXPECT_EQ(removed, 2);
    EXPECT_EQ(multiElementList.size(), 3);
    
    std::vector<int> remaining;
    for (int x : multiElementList) {
        remaining.push_back(x);
    }
    EXPECT_EQ(remaining, (std::vector<int>{1, 3, 5}));
}

TEST_F(DoublyLinkedListTest, RemoveIf_NoMatches_RemovesNothing) {
    // Act
    size_t removed = multiElementList.remove_if([](int x) { return x > 100; });
    
    // Assert
    EXPECT_EQ(removed, 0);
    EXPECT_EQ(multiElementList.size(), 5);
}

TEST_F(DoublyLinkedListTest, RemoveIf_AllMatch_ClearsList) {
    // Act
    size_t removed = multiElementList.remove_if([](int) { return true; });
    
    // Assert
    EXPECT_EQ(removed, 5);
    EXPECT_TRUE(multiElementList.empty());
}

TEST_F(DoublyLinkedListTest, RemoveIf_EmptyList_ReturnsZero) {
    size_t removed = emptyList.remove_if([](int) { return true; });
    EXPECT_EQ(removed, 0);
}

TEST_F(DoublyLinkedListTest, RemoveIf_ConsecutiveElements_RemovesCorrectly) {
    // Arrange
    DoublyLinkedList<int> list = {1, 2, 2, 2, 3};
    
    // Act - Entferne alle 2er
    size_t removed = list.remove_if([](int x) { return x == 2; });
    
    // Assert
    EXPECT_EQ(removed, 3);
    EXPECT_EQ(list.size(), 2);
}

// ============================================================================
// VERGLEICHSOPERATOR TESTS
// ============================================================================

TEST_F(DoublyLinkedListTest, Equality_SameLists_ReturnsTrue) {
    DoublyLinkedList<int> list1 = {1, 2, 3};
    DoublyLinkedList<int> list2 = {1, 2, 3};
    EXPECT_EQ(list1, list2);
}

TEST_F(DoublyLinkedListTest, Equality_DifferentElements_ReturnsFalse) {
    DoublyLinkedList<int> list1 = {1, 2, 3};
    DoublyLinkedList<int> list2 = {1, 2, 4};
    EXPECT_NE(list1, list2);
}

TEST_F(DoublyLinkedListTest, Equality_DifferentSizes_ReturnsFalse) {
    DoublyLinkedList<int> list1 = {1, 2, 3};
    DoublyLinkedList<int> list2 = {1, 2};
    EXPECT_NE(list1, list2);
}

TEST_F(DoublyLinkedListTest, Equality_BothEmpty_ReturnsTrue) {
    DoublyLinkedList<int> list1;
    DoublyLinkedList<int> list2;
    EXPECT_EQ(list1, list2);
}

// ============================================================================
// EDGE CASE TESTS
// ============================================================================

TEST(DoublyLinkedListEdgeCases, LargeList_HandlesCorrectly) {
    // Arrange
    DoublyLinkedList<int> list;
    const int N = 10000;
    
    // Act
    for (int i = 0; i < N; ++i) {
        list.push_back(i);
    }
    
    // Assert
    EXPECT_EQ(list.size(), N);
    EXPECT_EQ(list.front(), 0);
    EXPECT_EQ(list.back(), N - 1);
    
    // Verifiziere vollständige Traversierung
    int count = 0;
    for (int x : list) {
        EXPECT_EQ(x, count);
        ++count;
    }
    EXPECT_EQ(count, N);
}

TEST(DoublyLinkedListEdgeCases, AlternatingPushFrontBack_MaintainsOrder) {
    // Arrange
    DoublyLinkedList<int> list;
    
    // Act
    list.push_back(3);    // {3}
    list.push_front(2);   // {2, 3}
    list.push_back(4);    // {2, 3, 4}
    list.push_front(1);   // {1, 2, 3, 4}
    list.push_back(5);    // {1, 2, 3, 4, 5}
    
    // Assert
    std::vector<int> result;
    for (int x : list) {
        result.push_back(x);
    }
    EXPECT_EQ(result, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(DoublyLinkedListEdgeCases, StringType_WorksCorrectly) {
    // Arrange
    DoublyLinkedList<std::string> list;
    
    // Act
    list.push_back("Hello");
    list.push_back("World");
    list.push_front("!");
    
    // Assert
    EXPECT_EQ(list.front(), "!");
    EXPECT_EQ(list.back(), "World");
    EXPECT_TRUE(list.contains("Hello"));
}

TEST(DoublyLinkedListEdgeCases, CustomType_WorksCorrectly) {
    // Arrange
    struct Point {
        int x, y;
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };
    
    DoublyLinkedList<Point> list;
    
    // Act
    list.push_back({1, 2});
    list.push_back({3, 4});
    
    // Assert
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front().x, 1);
    EXPECT_TRUE(list.contains({3, 4}));
}

TEST(DoublyLinkedListEdgeCases, UniquePtr_MoveOnlyType) {
    // Arrange
    DoublyLinkedList<std::unique_ptr<int>> list;
    
    // Act
    list.push_back(std::make_unique<int>(42));
    list.push_front(std::make_unique<int>(10));
    
    // Assert
    EXPECT_EQ(*list.front(), 10);
    EXPECT_EQ(*list.back(), 42);
}

TEST(DoublyLinkedListEdgeCases, RepeatedClearAndRefill_WorksCorrectly) {
    // Arrange
    DoublyLinkedList<int> list;
    
    // Act & Assert - mehrfaches Leeren und Füllen
    for (int round = 0; round < 5; ++round) {
        for (int i = 0; i < 100; ++i) {
            list.push_back(i);
        }
        EXPECT_EQ(list.size(), 100);
        
        list.clear();
        EXPECT_TRUE(list.empty());
    }
}

TEST(DoublyLinkedListEdgeCases, IteratorInvalidation_AfterPush) {
    // Hinweis: In einer doppelt verketteten Liste werden Iteratoren
    // bei push_front/push_back NICHT invalidiert
    
    // Arrange
    DoublyLinkedList<int> list = {1, 2, 3};
    auto it = list.begin();
    ++it; // Zeigt auf 2
    
    // Act
    list.push_front(0);
    list.push_back(4);
    
    // Assert - Iterator zeigt immer noch auf 2
    EXPECT_EQ(*it, 2);
}

TEST(DoublyLinkedListEdgeCases, BackwardIteration_FromEnd) {
    // Arrange
    DoublyLinkedList<int> list = {1, 2, 3, 4, 5};
    std::vector<int> reversed;
    
    // Act
    auto it = list.end();
    while (it != list.begin()) {
        --it;
        reversed.push_back(*it);
    }
    
    // Assert
    EXPECT_EQ(reversed, (std::vector<int>{5, 4, 3, 2, 1}));
}

// ============================================================================
// STL-ALGORITHMUS-KOMPATIBILITÄT TESTS
// ============================================================================

TEST_F(DoublyLinkedListTest, StdFind_FindsElement) {
    auto it = std::find(multiElementList.begin(), multiElementList.end(), 3);
    EXPECT_NE(it, multiElementList.end());
    EXPECT_EQ(*it, 3);
}

TEST_F(DoublyLinkedListTest, StdCount_CountsElements) {
    DoublyLinkedList<int> list = {1, 2, 2, 3, 2, 4};
    auto count = std::count(list.begin(), list.end(), 2);
    EXPECT_EQ(count, 3);
}

TEST_F(DoublyLinkedListTest, StdFindIf_FindsMatchingElement) {
    auto it = std::find_if(multiElementList.begin(), multiElementList.end(),
                           [](int x) { return x > 3; });
    EXPECT_NE(it, multiElementList.end());
    EXPECT_EQ(*it, 4);
}

TEST_F(DoublyLinkedListTest, StdTransform_ModifiesElements) {
    DoublyLinkedList<int> source = {1, 2, 3};
    DoublyLinkedList<int> dest;
    dest.push_back(0);
    dest.push_back(0);
    dest.push_back(0);
    
    std::transform(source.begin(), source.end(), dest.begin(),
                   [](int x) { return x * 2; });
    
    std::vector<int> result;
    for (int x : dest) {
        result.push_back(x);
    }
    EXPECT_EQ(result, (std::vector<int>{2, 4, 6}));
}

TEST_F(DoublyLinkedListTest, StdAccumulate_SumsElements) {
    int sum = std::accumulate(multiElementList.begin(), multiElementList.end(), 0);
    EXPECT_EQ(sum, 15);
}

TEST_F(DoublyLinkedListTest, StdAllOf_ChecksAllElements) {
    bool allPositive = std::all_of(multiElementList.begin(), multiElementList.end(),
                                    [](int x) { return x > 0; });
    EXPECT_TRUE(allPositive);
}

TEST_F(DoublyLinkedListTest, StdAnyOf_ChecksSomeElements) {
    bool hasEven = std::any_of(multiElementList.begin(), multiElementList.end(),
                               [](int x) { return x % 2 == 0; });
    EXPECT_TRUE(hasEven);
}

TEST_F(DoublyLinkedListTest, StdNoneOf_ChecksNoElements) {
    bool noneNegative = std::none_of(multiElementList.begin(), multiElementList.end(),
                                      [](int x) { return x < 0; });
    EXPECT_TRUE(noneNegative);
}

TEST_F(DoublyLinkedListTest, StdForEach_AppliesFunction) {
    int sum = 0;
    std::for_each(multiElementList.begin(), multiElementList.end(),
                  [&sum](int x) { sum += x; });
    EXPECT_EQ(sum, 15);
}

// ============================================================================
// MEMORY/RESOURCE MANAGEMENT TESTS
// ============================================================================

TEST(DoublyLinkedListMemory, DestructorFreesAllMemory) {
    // Dieser Test ist eher konzeptionell - wir können nicht direkt testen,
    // ob Speicher freigegeben wird, aber wir können sicherstellen,
    // dass keine Exceptions geworfen werden
    
    {
        DoublyLinkedList<int> list;
        for (int i = 0; i < 1000; ++i) {
            list.push_back(i);
        }
    } // Destruktor wird hier aufgerufen
    
    // Wenn wir hier ankommen, wurde der Speicher erfolgreich freigegeben
    SUCCEED();
}

TEST(DoublyLinkedListMemory, ClearFreesElementMemory) {
    DoublyLinkedList<std::string> list;
    
    for (int i = 0; i < 1000; ++i) {
        list.push_back(std::string(100, 'x'));
    }
    
    list.clear();
    EXPECT_TRUE(list.empty());
}

// ============================================================================
// CONST-CORRECTNESS TESTS
// ============================================================================

TEST_F(DoublyLinkedListTest, ConstList_AllowsReadOperations) {
    const DoublyLinkedList<int> constList = {1, 2, 3, 4, 5};
    
    // Diese sollten alle ohne Fehler kompilieren und funktionieren
    EXPECT_EQ(constList.size(), 5);
    EXPECT_FALSE(constList.empty());
    EXPECT_EQ(constList.front(), 1);
    EXPECT_EQ(constList.back(), 5);
    EXPECT_TRUE(constList.contains(3));
    EXPECT_NE(constList.find(3), constList.end());
    
    // Iteration
    int sum = 0;
    for (const int& x : constList) {
        sum += x;
    }
    EXPECT_EQ(sum, 15);
}

// ============================================================================
// STRESS TESTS
// ============================================================================

TEST(DoublyLinkedListStress, ManyPushPop_MaintainsIntegrity) {
    DoublyLinkedList<int> list;
    
    // Abwechselnd hinzufügen und entfernen
    for (int i = 0; i < 1000; ++i) {
        list.push_back(i);
        list.push_front(-i);
        
        if (i % 3 == 0 && !list.empty()) {
            list.pop_front();
        }
        if (i % 5 == 0 && !list.empty()) {
            list.pop_back();
        }
    }
    
    // Verifiziere Integrität durch vollständige Traversierung
    size_t count = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        ++count;
    }
    EXPECT_EQ(count, list.size());
    
    // Rückwärts-Traversierung
    count = 0;
    auto it = list.end();
    while (it != list.begin()) {
        --it;
        ++count;
    }
    EXPECT_EQ(count, list.size());
}

TEST(DoublyLinkedListStress, ManyCopies_WorkCorrectly) {
    DoublyLinkedList<int> original = {1, 2, 3, 4, 5};
    
    for (int i = 0; i < 100; ++i) {
        DoublyLinkedList<int> copy = original;
        EXPECT_EQ(copy, original);
        
        copy.push_back(6);
        EXPECT_NE(copy, original);
    }
}
