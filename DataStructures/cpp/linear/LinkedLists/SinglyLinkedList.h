/**
 * @file SinglyLinkedList.h
 * @brief Singly Linked List Implementation
 * @date 2025-02-16
 * 
 * This file implements a singly linked list with comprehensive features
 * and STL-like interface.
 * 
 * Time Complexities:
 * - Access: O(n)
 * - Search: O(n)
 * - Insertion at beginning: O(1)
 * - Insertion at end with tail pointer: O(1)
 * - Insertion at position: O(n)
 * - Deletion at beginning: O(1)
 * - Deletion at end: O(n)
 * - Deletion at position: O(n)
 * 
 * Space Complexity:
 * - O(n) where n is the number of elements
 * - Additional O(1) per node for next pointer
 */

 #ifndef SINGLY_LINKED_LIST_H
 #define SINGLY_LINKED_LIST_H
 
 #include <memory>
 #include <iterator>
 #include <initializer_list>
 #include <stdexcept>
 
 namespace ds {
 
 template<typename T, typename Allocator = std::allocator<T>>
 class SinglyLinkedList {
 private:
     // Node structure
     struct Node {
         T data;
         Node* next;
         
         Node(const T& value) : data(value), next(nullptr) {}
         Node(T&& value) : data(std::move(value)), next(nullptr) {}
     };
 
     // Node allocator type
     using NodeAllocator = typename std::allocator_traits<Allocator>::
         template rebind_alloc<Node>;
     using NodeAllocTraits = std::allocator_traits<NodeAllocator>;
 
 public:
     // Iterator class
     class iterator {
     public:
         using iterator_category = std::forward_iterator_tag;
         using value_type = T;
         using difference_type = std::ptrdiff_t;
         using pointer = T*;
         using reference = T&;
 
         iterator() noexcept : current_(nullptr) {}
         explicit iterator(Node* node) noexcept : current_(node) {}
 
         reference operator*() const { return current_->data; }
         pointer operator->() const { return &current_->data; }
         
         iterator& operator++() {
             current_ = current_->next;
             return *this;
         }
         
         iterator operator++(int) {
             iterator tmp = *this;
             current_ = current_->next;
             return tmp;
         }
 
         bool operator==(const iterator& other) const {
             return current_ == other.current_;
         }
         
         bool operator!=(const iterator& other) const {
             return !(*this == other);
         }
 
     private:
         Node* current_;
         friend class SinglyLinkedList;
     };
 
     // Const iterator class
     class const_iterator {
     public:
         using iterator_category = std::forward_iterator_tag;
         using value_type = const T;
         using difference_type = std::ptrdiff_t;
         using pointer = const T*;
         using reference = const T&;
 
         const_iterator() noexcept : current_(nullptr) {}
         explicit const_iterator(const Node* node) noexcept : current_(node) {}
         const_iterator(const iterator& it) noexcept : current_(it.current_) {}
 
         reference operator*() const { return current_->data; }
         pointer operator->() const { return &current_->data; }
         
         const_iterator& operator++() {
             current_ = current_->next;
             return *this;
         }
         
         const_iterator operator++(int) {
             const_iterator tmp = *this;
             current_ = current_->next;
             return tmp;
         }
 
         bool operator==(const const_iterator& other) const {
             return current_ == other.current_;
         }
         
         bool operator!=(const const_iterator& other) const {
             return !(*this == other);
         }
 
     private:
         const Node* current_;
         friend class SinglyLinkedList;
     };
 
     // Type definitions for STL compatibility
     using value_type = T;
     using allocator_type = Allocator;
     using size_type = std::size_t;
     using difference_type = std::ptrdiff_t;
     using reference = value_type&;
     using const_reference = const value_type&;
     using pointer = typename std::allocator_traits<Allocator>::pointer;
     using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
 
     /**
      * @brief Default constructor
      * @complexity Time: O(1), Space: O(1)
      */
     SinglyLinkedList() noexcept;
 
     /**
      * @brief Constructor with allocator
      * @param alloc Allocator instance
      * @complexity Time: O(1), Space: O(1)
      */
     explicit SinglyLinkedList(const Allocator& alloc) noexcept;
 
     /**
      * @brief Constructor with initial values
      * @param count Number of elements
      * @param value Value to fill the list with
      * @complexity Time: O(n), Space: O(n)
      */
     SinglyLinkedList(size_type count, const T& value,
                      const Allocator& alloc = Allocator());
 
     /**
      * @brief Constructor from initializer list
      * @param init Initializer list
      * @complexity Time: O(n), Space: O(n)
      */
     SinglyLinkedList(std::initializer_list<T> init,
                      const Allocator& alloc = Allocator());
 
     /**
      * @brief Copy constructor
      * @param other List to copy from
      * @complexity Time: O(n), Space: O(n)
      */
     SinglyLinkedList(const SinglyLinkedList& other);
 
     /**
      * @brief Move constructor
      * @param other List to move from
      * @complexity Time: O(1), Space: O(1)
      */
     SinglyLinkedList(SinglyLinkedList&& other) noexcept;
 
     /**
      * @brief Destructor
      * @complexity Time: O(n), Space: O(1)
      */
     ~SinglyLinkedList();
 
     // Element access
     reference front();
     const_reference front() const;
 
     // Iterators
     iterator begin() noexcept;
     const_iterator begin() const noexcept;
     const_iterator cbegin() const noexcept;
     iterator end() noexcept;
     const_iterator end() const noexcept;
     const_iterator cend() const noexcept;
 
     // Capacity
     bool empty() const noexcept;
     size_type size() const noexcept;
 
     // Modifiers
     void clear() noexcept;
     iterator insert(const_iterator pos, const T& value);
     iterator insert(const_iterator pos, T&& value);
     iterator erase(const_iterator pos);
     void push_front(const T& value);
     void push_front(T&& value);
     void pop_front();
     void push_back(const T& value);
     void push_back(T&& value);
 
     // Operations
     void reverse() noexcept;
     void remove(const T& value);
     template<class UnaryPredicate>
     void remove_if(UnaryPredicate p);
     void unique();
     void sort();
     void merge(SinglyLinkedList& other);
 
     // Operators
     SinglyLinkedList& operator=(const SinglyLinkedList& other);
     SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept;
     bool operator==(const SinglyLinkedList& other) const;
     bool operator!=(const SinglyLinkedList& other) const;
 
 private:
     NodeAllocator node_alloc_;    // Node allocator
     Node* head_;                  // Pointer to first node
     Node* tail_;                  // Pointer to last node
     size_type size_;             // Number of nodes
 
     // Helper functions
     Node* create_node(const T& value);
     Node* create_node(T&& value);
     void destroy_node(Node* node);
     void initialize_empty() noexcept;
     void copy_from(const SinglyLinkedList& other);
     void move_from(SinglyLinkedList& other) noexcept;
 };
 
 } // namespace ds
 
 #endif // SINGLY_LINKED_LIST_H