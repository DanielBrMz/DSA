/**
 * @file DoublyLinkedList.h
 * @brief Doubly Linked List Implementation
 * @date 2025-02-16
 * 
 * This file implements a doubly linked list with bidirectional traversal
 * and efficient operations at both ends.
 * 
 * Time Complexities:
 * - Access: O(n)
 * - Search: O(n)
 * - Insertion at beginning/end: O(1)
 * - Insertion at position: O(n)
 * - Deletion at beginning/end: O(1)
 * - Deletion at position: O(n)
 * - Reverse traversal: O(1) per step
 * 
 * Space Complexity:
 * - O(n) where n is the number of elements
 * - Additional O(2) per node for next/prev pointers
 */

 #ifndef DOUBLY_LINKED_LIST_H
 #define DOUBLY_LINKED_LIST_H
 
 #include <memory>
 #include <iterator>
 #include <initializer_list>
 #include <stdexcept>
 
 namespace ds {
 
 template<typename T, typename Allocator = std::allocator<T>>
 class DoublyLinkedList {
 private:
     // Node structure
     struct Node {
         T data;
         Node* prev;
         Node* next;
         
         Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
         Node(T&& value) : data(std::move(value)), prev(nullptr), next(nullptr) {}
     };
 
     using NodeAllocator = typename std::allocator_traits<Allocator>::
         template rebind_alloc<Node>;
     using NodeAllocTraits = std::allocator_traits<NodeAllocator>;
 
 public:
     // Bidirectional iterator class
     class iterator {
     public:
         using iterator_category = std::bidirectional_iterator_tag;
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
 
         iterator& operator--() {
             current_ = current_->prev;
             return *this;
         }
         
         iterator operator--(int) {
             iterator tmp = *this;
             current_ = current_->prev;
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
         friend class DoublyLinkedList;
     };
 
     // Const iterator class
     class const_iterator {
     public:
         using iterator_category = std::bidirectional_iterator_tag;
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
 
         const_iterator& operator--() {
             current_ = current_->prev;
             return *this;
         }
         
         const_iterator operator--(int) {
             const_iterator tmp = *this;
             current_ = current_->prev;
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
         friend class DoublyLinkedList;
     };
 
     // Type definitions
     using value_type = T;
     using allocator_type = Allocator;
     using size_type = std::size_t;
     using difference_type = std::ptrdiff_t;
     using reference = value_type&;
     using const_reference = const value_type&;
     using pointer = typename std::allocator_traits<Allocator>::pointer;
     using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
     using reverse_iterator = std::reverse_iterator<iterator>;
     using const_reverse_iterator = std::reverse_iterator<const_iterator>;
 
     // Constructors and destructor
     DoublyLinkedList() noexcept;
     explicit DoublyLinkedList(const Allocator& alloc) noexcept;
     DoublyLinkedList(size_type count, const T& value, const Allocator& alloc = Allocator());
     DoublyLinkedList(std::initializer_list<T> init, const Allocator& alloc = Allocator());
     DoublyLinkedList(const DoublyLinkedList& other);
     DoublyLinkedList(DoublyLinkedList&& other) noexcept;
     ~DoublyLinkedList();
 
     // Element access
     reference front();
     const_reference front() const;
     reference back();
     const_reference back() const;
 
     // Iterators
     iterator begin() noexcept;
     const_iterator begin() const noexcept;
     const_iterator cbegin() const noexcept;
     iterator end() noexcept;
     const_iterator end() const noexcept;
     const_iterator cend() const noexcept;
     reverse_iterator rbegin() noexcept;
     const_reverse_iterator rbegin() const noexcept;
     reverse_iterator rend() noexcept;
     const_reverse_iterator rend() const noexcept;
 
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
     void pop_back();
 
     // Operations
     void reverse() noexcept;
     void remove(const T& value);
     template<class UnaryPredicate>
     void remove_if(UnaryPredicate p);
     void unique();
     void sort();
     void merge(DoublyLinkedList& other);
     void splice(const_iterator pos, DoublyLinkedList& other);
     void splice(const_iterator pos, DoublyLinkedList& other, const_iterator it);
     void splice(const_iterator pos, DoublyLinkedList& other, 
                 const_iterator first, const_iterator last);
 
     // Operators
     DoublyLinkedList& operator=(const DoublyLinkedList& other);
     DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept;
     bool operator==(const DoublyLinkedList& other) const;
     bool operator!=(const DoublyLinkedList& other) const;
 
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
     void copy_from(const DoublyLinkedList& other);
     void move_from(DoublyLinkedList& other) noexcept;
     void link_nodes(Node* prev, Node* next);
     Node* merge_sort(Node* head);
     std::pair<Node*, Node*> merge_sorted_lists(Node* first, Node* second);
 };
 
 } // namespace ds
 
 #endif // DOUBLY_LINKED_LIST_H