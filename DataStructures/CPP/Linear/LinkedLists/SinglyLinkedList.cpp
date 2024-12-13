/**
 * @file SinglyLinkedList.cpp
 * @brief Implementation of Singly Linked List methods
 */

 #include "SinglyLinkedList.h"

 namespace ds {
 
 // Constructors and Destructor
 template<typename T, typename Allocator>
 SinglyLinkedList<T, Allocator>::SinglyLinkedList() noexcept
     : node_alloc_()
     , head_(nullptr)
     , tail_(nullptr)
     , size_(0) {
 }
 
 template<typename T, typename Allocator>
 SinglyLinkedList<T, Allocator>::SinglyLinkedList(const Allocator& alloc) noexcept
     : node_alloc_(alloc)
     , head_(nullptr)
     , tail_(nullptr)
     , size_(0) {
 }
 
 template<typename T, typename Allocator>
 SinglyLinkedList<T, Allocator>::SinglyLinkedList(size_type count, const T& value,
                                                 const Allocator& alloc)
     : node_alloc_(alloc)
     , head_(nullptr)
     , tail_(nullptr)
     , size_(0) {
     
     for (size_type i = 0; i < count; ++i) {
         push_back(value);
     }
 }
 
 template<typename T, typename Allocator>
 SinglyLinkedList<T, Allocator>::SinglyLinkedList(std::initializer_list<T> init,
                                                 const Allocator& alloc)
     : node_alloc_(alloc)
     , head_(nullptr)
     , tail_(nullptr)
     , size_(0) {
     
     for (const auto& value : init) {
         push_back(value);
     }
 }
 
 template<typename T, typename Allocator>
 SinglyLinkedList<T, Allocator>::SinglyLinkedList(const SinglyLinkedList& other)
     : node_alloc_(NodeAllocTraits::select_on_container_copy_construction(other.node_alloc_))
     , head_(nullptr)
     , tail_(nullptr)
     , size_(0) {
     
     copy_from(other);
 }
 
 template<typename T, typename Allocator>
 SinglyLinkedList<T, Allocator>::SinglyLinkedList(SinglyLinkedList&& other) noexcept
     : node_alloc_(std::move(other.node_alloc_))
     , head_(other.head_)
     , tail_(other.tail_)
     , size_(other.size_) {
     
     other.head_ = nullptr;
     other.tail_ = nullptr;
     other.size_ = 0;
 }
 
 template<typename T, typename Allocator>
 SinglyLinkedList<T, Allocator>::~SinglyLinkedList() {
     clear();
 }
 
 // Element access
 template<typename T, typename Allocator>
 typename SinglyLinkedList<T, Allocator>::reference
 SinglyLinkedList<T, Allocator>::front() {
     if (empty()) {
         throw std::out_of_range("List is empty");
     }
     return head_->data;
 }
 
 template<typename T, typename Allocator>
 typename SinglyLinkedList<T, Allocator>::const_reference
 SinglyLinkedList<T, Allocator>::front() const {
     if (empty()) {
         throw std::out_of_range("List is empty");
     }
     return head_->data;
 }
 
 // Modifiers
 template<typename T, typename Allocator>
 void SinglyLinkedList<T, Allocator>::push_front(const T& value) {
     Node* new_node = create_node(value);
     new_node->next = head_;
     head_ = new_node;
     if (!tail_) {
         tail_ = new_node;
     }
     ++size_;
 }
 
 template<typename T, typename Allocator>
 void SinglyLinkedList<T, Allocator>::push_front(T&& value) {
     Node* new_node = create_node(std::move(value));
     new_node->next = head_;
     head_ = new_node;
     if (!tail_) {
         tail_ = new_node;
     }
     ++size_;
 }
 
 template<typename T, typename Allocator>
 void SinglyLinkedList<T, Allocator>::pop_front() {
     if (empty()) {
         throw std::out_of_range("List is empty");
     }
     
     Node* old_head = head_;
     head_ = head_->next;
     destroy_node(old_head);
     --size_;
     
     if (size_ == 0) {
         tail_ = nullptr;
     }
 }
 
 template<typename T, typename Allocator>
 void SinglyLinkedList<T, Allocator>::push_back(const T& value) {
     Node* new_node = create_node(value);
     if (tail_) {
         tail_->next = new_node;
     } else {
         head_ = new_node;
     }
     tail_ = new_node;
     ++size_;
 }
 
 template<typename T, typename Allocator>
 void SinglyLinkedList<T, Allocator>::push_back(T&& value) {
     Node* new_node = create_node(std::move(value));
     if (tail_) {
         tail_->next = new_node;
     } else {
         head_ = new_node;
     }
     tail_ = new_node;
     ++size_;
 }
 
 // Operations
 template<typename T, typename Allocator>
 void SinglyLinkedList<T, Allocator>::clear() noexcept {
     while (head_) {
         Node* temp = head_;
         head_ = head_->next;
         destroy_node(temp);
     }
     tail_ = nullptr;
     size_ = 0;
 }
 
 template<typename T, typename Allocator>
 void SinglyLinkedList<T, Allocator>::reverse() noexcept {
     if (size_ <= 1) return;
     
     Node* prev = nullptr;
     Node* current = head_;
     tail_ = head_;  // Current head will become tail
     
     while (current != nullptr) {
         Node* next_temp = current->next;
         current->next = prev;
         prev = current;
         current = next_temp;
     }
     
     head_ = prev;
 }
 
 template<typename T, typename Allocator>
 void SinglyLinkedList<T, Allocator>::remove(const T& value) {
     Node* current = head_;
     Node* prev = nullptr;
     
     while (current != nullptr) {
         if (current->data == value) {
             if (prev == nullptr) {
                 // Removing head
                 head_ = current->next;
                 if (tail_ == current) {
                     tail_ = nullptr;
                 }
             } else {
                 prev->next = current->next;
                 if (tail_ == current) {
                     tail_ = prev;
                 }
             }
             Node* to_delete = current;
             current = current->next;
             destroy_node(to_delete);
             --size_;
         } else {
             prev = current;
             current = current->next;
         }
     }
 }
 
 template<typename T, typename Allocator>
 template<class UnaryPredicate>
 void SinglyLinkedList<T, Allocator>::remove_if(UnaryPredicate p) {
     Node* current = head_;
     Node* prev = nullptr;
     
     while (current != nullptr) {
         if (p(current->data)) {
             if (prev == nullptr) {
                 head_ = current->next;
                 if (tail_ == current) {
                     tail_ = nullptr;
                 }
             } else {
                 prev->next = current->next;
                 if (tail_ == current) {
                     tail_ = prev;
                 }
             }
             Node* to_delete = current;
             current = current->next;
             destroy_node(to_delete);
             --size_;
         } else {
             prev = current;
             current = current->next;
         }
     }
 }
 
 template<typename T, typename Allocator>
 void SinglyLinkedList<T, Allocator>::unique() {
     if (size_ <= 1) return;
     
     Node* current = head_;
     while (current && current->next) {
         if (current->data == current->next->data) {
             Node* duplicate = current->next;
             current->next = duplicate->next;
             if (tail_ == duplicate) {
                 tail_ = current;
             }
             destroy_node(duplicate);
             --size_;
         } else {
             current = current->next;
         }
     }
 }
 
 template<typename T, typename Allocator>
 void SinglyLinkedList<T, Allocator>::sort() {
     if (size_ <= 1) return;
     
     // Using merge sort algorithm
     head_ = merge_sort(head_);
     
     // Update tail
     Node* current = head_;
     while (current->next) {
         current = current->next;
     }
     tail_ = current;
 }
 
 template<typename T, typename Allocator>
 void SinglyLinkedList<T, Allocator>::merge(SinglyLinkedList& other) {
     if (this == &other) return;
     
     if (empty()) {
         head_ = other.head_;
         tail_ = other.tail_;
         size_ = other.size_;
     } else if (!other.empty()) {
         tail_->next = other.head_;
         tail_ = other.tail_;
         size_ += other.size_;
     }
     
     other.head_ = nullptr;
     other.tail_ = nullptr;
     other.size_ = 0;
 }
 
 // Helper methods
 template<typename T, typename Allocator>
 typename SinglyLinkedList<T, Allocator>::Node*
 SinglyLinkedList<T, Allocator>::create_node(const T& value) {
     Node* node = NodeAllocTraits::allocate(node_alloc_, 1);
     try {
         NodeAllocTraits::construct(node_alloc_, node, value);
     } catch (...) {
         NodeAllocTraits::deallocate(node_alloc_, node, 1);
         throw;
     }
     return node;
 }
 
 template<typename T, typename Allocator>
 typename SinglyLinkedList<T, Allocator>::Node*
 SinglyLinkedList<T, Allocator>::create_node(T&& value) {
     Node* node = NodeAllocTraits::allocate(node_alloc_, 1);
     try {
         NodeAllocTraits::construct(node_alloc_, node, std::move(value));
     } catch (...) {
         NodeAllocTraits::deallocate(node_alloc_, node, 1);
         throw;
     }
     return node;
 }
 
 template<typename T, typename Allocator>
 void SinglyLinkedList<T, Allocator>::destroy_node(Node* node) {
     NodeAllocTraits::destroy(node_alloc_, node);
     NodeAllocTraits::deallocate(node_alloc_, node, 1);
 }
 
 template<typename T, typename Allocator>
 void SinglyLinkedList<T, Allocator>::copy_from(const SinglyLinkedList& other) {
     Node* current = other.head_;
     while (current != nullptr) {
         push_back(current->data);
         current = current->next;
     }
 }
 
 // Operators
 template<typename T, typename Allocator>
 SinglyLinkedList<T, Allocator>&
 SinglyLinkedList<T, Allocator>::operator=(const SinglyLinkedList& other) {
     if (this != &other) {
         SinglyLinkedList temp(other);
         std::swap(head_, temp.head_);
         std::swap(tail_, temp.tail_);
         std::swap(size_, temp.size_);
         if (NodeAllocTraits::propagate_on_container_copy_assignment::value) {
             node_alloc_ = other.node_alloc_;
         }
     }
     return *this;
 }
 
 template<typename T, typename Allocator>
 SinglyLinkedList<T, Allocator>&
 SinglyLinkedList<T, Allocator>::operator=(SinglyLinkedList&& other) noexcept {
     if (this != &other) {
         clear();
         head_ = other.head_;
         tail_ = other.tail_;
         size_ = other.size_;
         if (NodeAllocTraits::propagate_on_container_move_assignment::value) {
             node_alloc_ = std::move(other.node_alloc_);
         }
         other.head_ = nullptr;
         other.tail_ = nullptr;
         other.size_ = 0;
     }
     return *this;
 }
 
 template<typename T, typename Allocator>
 bool SinglyLinkedList<T, Allocator>::operator==(const SinglyLinkedList& other) const {
     if (size_ != other.size_) return false;
     
     Node* current1 = head_;
     Node* current2 = other.head_;
     while (current1 != nullptr) {
         if (current1->data != current2->data) return false;
         current1 = current1->next;
         current2 = current2->next;
     }
     return true;
 }
 
 template<typename T, typename Allocator>
 bool SinglyLinkedList<T, Allocator>::operator!=(const SinglyLinkedList& other) const {
     return !(*this == other);
 }
 
 } // namespace ds