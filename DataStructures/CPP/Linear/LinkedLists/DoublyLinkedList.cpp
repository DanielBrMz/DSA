/**
 * @file DoublyLinkedList.cpp
 * @brief Implementation of Doubly Linked List methods
 */

 #include "DoublyLinkedList.h"

 namespace ds {
 
 // Constructors and Destructor
 template<typename T, typename Allocator>
 DoublyLinkedList<T, Allocator>::DoublyLinkedList() noexcept
     : node_alloc_()
     , head_(nullptr)
     , tail_(nullptr)
     , size_(0) {
 }
 
 template<typename T, typename Allocator>
 DoublyLinkedList<T, Allocator>::DoublyLinkedList(const Allocator& alloc) noexcept
     : node_alloc_(alloc)
     , head_(nullptr)
     , tail_(nullptr)
     , size_(0) {
 }
 
 template<typename T, typename Allocator>
 DoublyLinkedList<T, Allocator>::DoublyLinkedList(size_type count, const T& value,
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
 DoublyLinkedList<T, Allocator>::DoublyLinkedList(std::initializer_list<T> init,
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
 DoublyLinkedList<T, Allocator>::DoublyLinkedList(const DoublyLinkedList& other)
     : node_alloc_(NodeAllocTraits::select_on_container_copy_construction(other.node_alloc_))
     , head_(nullptr)
     , tail_(nullptr)
     , size_(0) {
     
     copy_from(other);
 }
 
 template<typename T, typename Allocator>
 DoublyLinkedList<T, Allocator>::DoublyLinkedList(DoublyLinkedList&& other) noexcept
     : node_alloc_(std::move(other.node_alloc_))
     , head_(other.head_)
     , tail_(other.tail_)
     , size_(other.size_) {
     
     other.head_ = nullptr;
     other.tail_ = nullptr;
     other.size_ = 0;
 }
 
 template<typename T, typename Allocator>
 DoublyLinkedList<T, Allocator>::~DoublyLinkedList() {
     clear();
 }
 
 // Element access
 template<typename T, typename Allocator>
 typename DoublyLinkedList<T, Allocator>::reference
 DoublyLinkedList<T, Allocator>::front() {
     if (empty()) {
         throw std::out_of_range("List is empty");
     }
     return head_->data;
 }
 
 template<typename T, typename Allocator>
 typename DoublyLinkedList<T, Allocator>::const_reference
 DoublyLinkedList<T, Allocator>::front() const {
     if (empty()) {
         throw std::out_of_range("List is empty");
     }
     return head_->data;
 }
 
 template<typename T, typename Allocator>
 typename DoublyLinkedList<T, Allocator>::reference
 DoublyLinkedList<T, Allocator>::back() {
     if (empty()) {
         throw std::out_of_range("List is empty");
     }
     return tail_->data;
 }
 
 template<typename T, typename Allocator>
 typename DoublyLinkedList<T, Allocator>::const_reference
 DoublyLinkedList<T, Allocator>::back() const {
     if (empty()) {
         throw std::out_of_range("List is empty");
     }
     return tail_->data;
 }
 
 // Capacity
 template<typename T, typename Allocator>
 bool DoublyLinkedList<T, Allocator>::empty() const noexcept {
     return size_ == 0;
 }
 
 template<typename T, typename Allocator>
 typename DoublyLinkedList<T, Allocator>::size_type
 DoublyLinkedList<T, Allocator>::size() const noexcept {
     return size_;
 }
 
 // Modifiers
 template<typename T, typename Allocator>
 void DoublyLinkedList<T, Allocator>::push_front(const T& value) {
     Node* new_node = create_node(value);
     if (empty()) {
         head_ = tail_ = new_node;
     } else {
         new_node->next = head_;
         head_->prev = new_node;
         head_ = new_node;
     }
     ++size_;
 }
 
 template<typename T, typename Allocator>
 void DoublyLinkedList<T, Allocator>::push_front(T&& value) {
     Node* new_node = create_node(std::move(value));
     if (empty()) {
         head_ = tail_ = new_node;
     } else {
         new_node->next = head_;
         head_->prev = new_node;
         head_ = new_node;
     }
     ++size_;
 }
 
 template<typename T, typename Allocator>
 void DoublyLinkedList<T, Allocator>::push_back(const T& value) {
     Node* new_node = create_node(value);
     if (empty()) {
         head_ = tail_ = new_node;
     } else {
         new_node->prev = tail_;
         tail_->next = new_node;
         tail_ = new_node;
     }
     ++size_;
 }
 
 template<typename T, typename Allocator>
 void DoublyLinkedList<T, Allocator>::push_back(T&& value) {
     Node* new_node = create_node(std::move(value));
     if (empty()) {
         head_ = tail_ = new_node;
     } else {
         new_node->prev = tail_;
         tail_->next = new_node;
         tail_ = new_node;
     }
     ++size_;
 }
 
 template<typename T, typename Allocator>
 void DoublyLinkedList<T, Allocator>::pop_front() {
     if (empty()) {
         throw std::out_of_range("List is empty");
     }
     
     Node* old_head = head_;
     head_ = head_->next;
     
     if (head_) {
         head_->prev = nullptr;
     } else {
         tail_ = nullptr;
     }
     
     destroy_node(old_head);
     --size_;
 }
 
 template<typename T, typename Allocator>
 void DoublyLinkedList<T, Allocator>::pop_back() {
     if (empty()) {
         throw std::out_of_range("List is empty");
     }
     
     Node* old_tail = tail_;
     tail_ = tail_->prev;
     
     if (tail_) {
         tail_->next = nullptr;
     } else {
         head_ = nullptr;
     }
     
     destroy_node(old_tail);
     --size_;
 }
 
 template<typename T, typename Allocator>
 typename DoublyLinkedList<T, Allocator>::iterator
 DoublyLinkedList<T, Allocator>::insert(const_iterator pos, const T& value) {
     if (pos == cend()) {
         push_back(value);
         return iterator(tail_);
     }
     
     Node* new_node = create_node(value);
     Node* current = const_cast<Node*>(pos.current_);
     
     new_node->next = current;
     new_node->prev = current->prev;
     
     if (current->prev) {
         current->prev->next = new_node;
     } else {
         head_ = new_node;
     }
     
     current->prev = new_node;
     ++size_;
     
     return iterator(new_node);
 }
 
 template<typename T, typename Allocator>
 typename DoublyLinkedList<T, Allocator>::iterator
 DoublyLinkedList<T, Allocator>::insert(const_iterator pos, T&& value) {
     if (pos == cend()) {
         push_back(std::move(value));
         return iterator(tail_);
     }
     
     Node* new_node = create_node(std::move(value));
     Node* current = const_cast<Node*>(pos.current_);
     
     new_node->next = current;
     new_node->prev = current->prev;
     
     if (current->prev) {
         current->prev->next = new_node;
     } else {
         head_ = new_node;
     }
     
     current->prev = new_node;
     ++size_;
     
     return iterator(new_node);
 }
 
 template<typename T, typename Allocator>
 typename DoublyLinkedList<T, Allocator>::iterator
 DoublyLinkedList<T, Allocator>::erase(const_iterator pos) {
     if (empty() || pos == cend()) {
         throw std::out_of_range("Invalid iterator");
     }
     
     Node* current = const_cast<Node*>(pos.current_);
     Node* next_node = current->next;
     
     if (current->prev) {
         current->prev->next = next_node;
     } else {
         head_ = next_node;
     }
     
     if (next_node) {
         next_node->prev = current->prev;
     } else {
         tail_ = current->prev;
     }
     
     destroy_node(current);
     --size_;
     
     return iterator(next_node);
 }
 
 template<typename T, typename Allocator>
 void DoublyLinkedList<T, Allocator>::clear() noexcept {
     while (head_) {
         Node* temp = head_;
         head_ = head_->next;
         destroy_node(temp);
     }
     tail_ = nullptr;
     size_ = 0;
 }
 
 // Operations
 template<typename T, typename Allocator>
 void DoublyLinkedList<T, Allocator>::splice(const_iterator pos,
                                            DoublyLinkedList& other,
                                            const_iterator first,
                                            const_iterator last) {
     if (first == last) return;
     if (&other == this) return;
     
     size_type count = 0;
     for (auto it = first; it != last; ++it) {
         ++count;
     }
     
     Node* before_first = const_cast<Node*>(first.current_->prev);
     Node* before_last = const_cast<Node*>(last.current_->prev);
     Node* insert_pos = const_cast<Node*>(pos.current_);
     
     if (before_first) {
         before_first->next = last.current_;
     } else {
         other.head_ = last.current_;
     }
     
     if (last.current_) {
         last.current_->prev = before_first;
     }
     
     Node* first_node = const_cast<Node*>(first.current_);
     if (insert_pos) {
         Node* before_pos = insert_pos->prev;
         if (before_pos) {
             before_pos->next = first_node;
         } else {
             head_ = first_node;
         }
         first_node->prev = before_pos;
         before_last->next = insert_pos;
         insert_pos->prev = before_last;
     } else {
         if (tail_) {
             tail_->next = first_node;
             first_node->prev = tail_;
         } else {
             head_ = first_node;
             first_node->prev = nullptr;
         }
         tail_ = before_last;
         tail_->next = nullptr;
     }
     
     size_ += count;
     other.size_ -= count;
 }
 
 template<typename T, typename Allocator>
 void DoublyLinkedList<T, Allocator>::remove(const T& value) {
     Node* current = head_;
     while (current) {
         Node* next = current->next;
         if (current->data == value) {
             if (current->prev) {
                 current->prev->next = current->next;
             } else {
                 head_ = current->next;
             }
             if (current->next) {
                 current->next->prev = current->prev;
             } else {
                 tail_ = current->prev;
             }
             destroy_node(current);
             --size_;
         }
         current = next;
     }
 }
 
 template<typename T, typename Allocator>
 template<class UnaryPredicate>
 void DoublyLinkedList<T, Allocator>::remove_if(UnaryPredicate p) {
     Node* current = head_;
     while (current) {
         Node* next = current->next;
         if (p(current->data)) {
             if (current->prev) {
                 current->prev->next = current->next;
             } else {
                 head_ = current->next;
             }
             if (current->next) {
                 current->next->prev = current->prev;
             } else {
                 tail_ = current->prev;
             }
             destroy_node(current);
             --size_;
         }
         current = next;
     }
 }
 
 template<typename T, typename Allocator>
 void DoublyLinkedList<T, Allocator>::reverse() noexcept {
     if (size_ <= 1) return;
     
     Node* current = head_;
     Node* temp = nullptr;
     
     while (current) {
         temp = current->prev;
         current->prev = current->next;
         current->next = temp;
         current = current->prev;
     }
     
     temp = head_;
     head_ = tail_;
     tail_ = temp;
 }
 
 template<typename T, typename Allocator>
 void DoublyLinkedList<T, Allocator>::unique() {
     if (size_ <= 1) return;
     
     Node* current = head_;
     while (current && current->next) {
         if (current->data == current->next->data) {
             Node* duplicate = current->next;
             current->next = duplicate->next;
             if (duplicate->next) {
                 duplicate->next->prev = current;
             } else {
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
 void DoublyLinkedList<T, Allocator>::sort() {
     if (size_ <= 1) return;
     
     head_ = merge_sort(head_);
     
     Node* current = head_;
     while (current->next) {
         current->next->prev = current;
         current = current->next;
     }
     tail_ = current;
 }
 
// Iterator methods
template<typename T, typename Allocator>
typename DoublyLinkedList<T, Allocator>::iterator
DoublyLinkedList<T, Allocator>::begin() noexcept {
    return iterator(head_);
}

template<typename T, typename Allocator>
typename DoublyLinkedList<T, Allocator>::const_iterator
DoublyLinkedList<T, Allocator>::begin() const noexcept {
    return const_iterator(head_);
}

template<typename T, typename Allocator>
typename DoublyLinkedList<T, Allocator>::const_iterator
DoublyLinkedList<T, Allocator>::cbegin() const noexcept {
    return const_iterator(head_);
}

template<typename T, typename Allocator>
typename DoublyLinkedList<T, Allocator>::iterator
DoublyLinkedList<T, Allocator>::end() noexcept {
    return iterator(nullptr);
}

template<typename T, typename Allocator>
typename DoublyLinkedList<T, Allocator>::const_iterator
DoublyLinkedList<T, Allocator>::end() const noexcept {
    return const_iterator(nullptr);
}

template<typename T, typename Allocator>
typename DoublyLinkedList<T, Allocator>::const_iterator
DoublyLinkedList<T, Allocator>::cend() const noexcept {
    return const_iterator(nullptr);
}

template<typename T, typename Allocator>
typename DoublyLinkedList<T, Allocator>::reverse_iterator
DoublyLinkedList<T, Allocator>::rbegin() noexcept {
    return reverse_iterator(end());
}

template<typename T, typename Allocator>
typename DoublyLinkedList<T, Allocator>::const_reverse_iterator
DoublyLinkedList<T, Allocator>::rbegin() const noexcept {
    return const_reverse_iterator(end());
}

template<typename T, typename Allocator>
typename DoublyLinkedList<T, Allocator>::reverse_iterator
DoublyLinkedList<T, Allocator>::rend() noexcept {
    return reverse_iterator(begin());
}

template<typename T, typename Allocator>
typename DoublyLinkedList<T, Allocator>::const_reverse_iterator
DoublyLinkedList<T, Allocator>::rend() const noexcept {
    return const_reverse_iterator(begin());
}

// Helper methods
template<typename T, typename Allocator>
typename DoublyLinkedList<T, Allocator>::Node*
DoublyLinkedList<T, Allocator>::create_node(const T& value) {
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
typename DoublyLinkedList<T, Allocator>::Node*
DoublyLinkedList<T, Allocator>::create_node(T&& value) {
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
void DoublyLinkedList<T, Allocator>::destroy_node(Node* node) {
    NodeAllocTraits::destroy(node_alloc_, node);
    NodeAllocTraits::deallocate(node_alloc_, node, 1);
}

template<typename T, typename Allocator>
void DoublyLinkedList<T, Allocator>::copy_from(const DoublyLinkedList& other) {
    Node* current = other.head_;
    while (current) {
        push_back(current->data);
        current = current->next;
    }
}

template<typename T, typename Allocator>
typename DoublyLinkedList<T, Allocator>::Node*
DoublyLinkedList<T, Allocator>::merge_sort(Node* head) {
    if (!head || !head->next) return head;
    
    // Find middle using slow and fast pointers
    Node* slow = head;
    Node* fast = head->next;
    
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    Node* right = slow->next;
    slow->next = nullptr;
    
    // Recursively sort both halves
    Node* left_sorted = merge_sort(head);
    Node* right_sorted = merge_sort(right);
    
    // Merge the sorted halves
    return merge_sorted_lists(left_sorted, right_sorted).first;
}

template<typename T, typename Allocator>
std::pair<typename DoublyLinkedList<T, Allocator>::Node*,
          typename DoublyLinkedList<T, Allocator>::Node*>
DoublyLinkedList<T, Allocator>::merge_sorted_lists(Node* first, Node* second) {
    if (!first) return {second, second};
    if (!second) return {first, first};
    
    Node* result = nullptr;
    Node* tail = nullptr;
    
    while (first && second) {
        Node* next = nullptr;
        if (first->data <= second->data) {
            next = first;
            first = first->next;
        } else {
            next = second;
            second = second->next;
        }
        
        if (!result) {
            result = next;
        } else {
            tail->next = next;
        }
        
        next->prev = tail;
        tail = next;
    }
    
    // Append remaining nodes
    if (first) {
        tail->next = first;
        first->prev = tail;
        while (tail->next) {
            tail = tail->next;
        }
    }
    if (second) {
        tail->next = second;
        second->prev = tail;
        while (tail->next) {
            tail = tail->next;
        }
    }
    
    return {result, tail};
}

// Operators
template<typename T, typename Allocator>
DoublyLinkedList<T, Allocator>&
DoublyLinkedList<T, Allocator>::operator=(const DoublyLinkedList& other) {
    if (this != &other) {
        DoublyLinkedList temp(other);
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
DoublyLinkedList<T, Allocator>&
DoublyLinkedList<T, Allocator>::operator=(DoublyLinkedList&& other) noexcept {
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
bool DoublyLinkedList<T, Allocator>::operator==(const DoublyLinkedList& other) const {
    if (size_ != other.size_) return false;
    
    Node* current1 = head_;
    Node* current2 = other.head_;
    while (current1) {
        if (current1->data != current2->data) return false;
        current1 = current1->next;
        current2 = current2->next;
    }
    return true;
}

template<typename T, typename Allocator>
bool DoublyLinkedList<T, Allocator>::operator!=(const DoublyLinkedList& other) const {
    return !(*this == other);
}

} // namespace ds