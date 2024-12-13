/**
 * @file CircularArray.cpp
 * @brief Implementation of Circular Array methods
 */

 #include "CircularArray.h"

 namespace ds {
 
 // CircularIterator implementation
 template<typename T, typename Allocator>
 template<bool IsConst>
 CircularArray<T, Allocator>::CircularIterator<IsConst>::CircularIterator() noexcept
     : buffer_(nullptr)
     , position_(0)
     , capacity_(0) {
 }
 
 template<typename T, typename Allocator>
 template<bool IsConst>
 CircularArray<T, Allocator>::CircularIterator<IsConst>::CircularIterator(
     pointer buffer, size_type position, size_type capacity)
     : buffer_(buffer)
     , position_(position)
     , capacity_(capacity) {
 }
 
 template<typename T, typename Allocator>
 template<bool IsConst>
 typename CircularArray<T, Allocator>::template CircularIterator<IsConst>::reference
 CircularArray<T, Allocator>::CircularIterator<IsConst>::operator*() const {
     return buffer_[position_];
 }
 
 template<typename T, typename Allocator>
 template<bool IsConst>
 typename CircularArray<T, Allocator>::template CircularIterator<IsConst>::pointer
 CircularArray<T, Allocator>::CircularIterator<IsConst>::operator->() const {
     return buffer_ + position_;
 }
 
 template<typename T, typename Allocator>
 template<bool IsConst>
 typename CircularArray<T, Allocator>::template CircularIterator<IsConst>&
 CircularArray<T, Allocator>::CircularIterator<IsConst>::operator++() {
     position_ = (position_ + 1) % capacity_;
     return *this;
 }
 
 // CircularArray implementation
 template<typename T, typename Allocator>
 CircularArray<T, Allocator>::CircularArray(size_type capacity, const Allocator& alloc)
     : alloc_(alloc)
     , buffer_(std::allocator_traits<Allocator>::allocate(alloc_, capacity))
     , head_(0)
     , tail_(0)
     , capacity_(capacity)
     , size_(0) {
 }
 
 template<typename T, typename Allocator>
 CircularArray<T, Allocator>::CircularArray(size_type count, const T& value, 
                                          const Allocator& alloc)
     : alloc_(alloc)
     , buffer_(std::allocator_traits<Allocator>::allocate(alloc_, count))
     , head_(0)
     , tail_(count)
     , capacity_(count)
     , size_(count) {
     
     for (size_type i = 0; i < count; ++i) {
         std::allocator_traits<Allocator>::construct(alloc_, buffer_ + i, value);
     }
 }
 
 template<typename T, typename Allocator>
 CircularArray<T, Allocator>::CircularArray(const CircularArray& other)
     : alloc_(std::allocator_traits<Allocator>::
         select_on_container_copy_construction(other.alloc_))
     , buffer_(std::allocator_traits<Allocator>::allocate(alloc_, other.capacity_))
     , head_(0)
     , tail_(other.size_)
     , capacity_(other.capacity_)
     , size_(other.size_) {
     
     copy_elements(other);
 }
 
 template<typename T, typename Allocator>
 CircularArray<T, Allocator>::CircularArray(CircularArray&& other) noexcept
     : alloc_(std::move(other.alloc_))
     , buffer_(other.buffer_)
     , head_(other.head_)
     , tail_(other.tail_)
     , capacity_(other.capacity_)
     , size_(other.size_) {
     
     other.buffer_ = nullptr;
     other.head_ = other.tail_ = other.capacity_ = other.size_ = 0;
 }
 
 template<typename T, typename Allocator>
 CircularArray<T, Allocator>::~CircularArray() {
     clear();
     if (buffer_) {
         std::allocator_traits<Allocator>::deallocate(alloc_, buffer_, capacity_);
     }
 }
 
 // Element access
 template<typename T, typename Allocator>
 typename CircularArray<T, Allocator>::reference
 CircularArray<T, Allocator>::at(size_type pos) {
     if (pos >= size_) {
         throw std::out_of_range("Index out of range");
     }
     return buffer_[normalize_index(head_ + pos)];
 }
 
 template<typename T, typename Allocator>
 typename CircularArray<T, Allocator>::const_reference
 CircularArray<T, Allocator>::at(size_type pos) const {
     if (pos >= size_) {
         throw std::out_of_range("Index out of range");
     }
     return buffer_[normalize_index(head_ + pos)];
 }
 
 // Modifiers
 template<typename T, typename Allocator>
 void CircularArray<T, Allocator>::push_front(const T& value) {
     if (full()) {
         throw std::runtime_error("CircularArray is full");
     }
     head_ = normalize_index(head_ - 1);
     std::allocator_traits<Allocator>::construct(alloc_, buffer_ + head_, value);
     ++size_;
 }
 
 template<typename T, typename Allocator>
 void CircularArray<T, Allocator>::push_back(const T& value) {
     if (full()) {
         throw std::runtime_error("CircularArray is full");
     }
     std::allocator_traits<Allocator>::construct(alloc_, buffer_ + tail_, value);
     tail_ = normalize_index(tail_ + 1);
     ++size_;
 }
 
 template<typename T, typename Allocator>
 void CircularArray<T, Allocator>::pop_front() {
     if (empty()) {
         throw std::runtime_error("CircularArray is empty");
     }
     std::allocator_traits<Allocator>::destroy(alloc_, buffer_ + head_);
     head_ = normalize_index(head_ + 1);
     --size_;
 }
 
 template<typename T, typename Allocator>
 void CircularArray<T, Allocator>::pop_back() {
     if (empty()) {
         throw std::runtime_error("CircularArray is empty");
     }
     tail_ = normalize_index(tail_ - 1);
     std::allocator_traits<Allocator>::destroy(alloc_, buffer_ + tail_);
     --size_;
 }
 
 template<typename T, typename Allocator>
 void CircularArray<T, Allocator>::rotate(difference_type n) {
     if (empty()) return;
     
     // Normalize rotation amount
     n = n % static_cast<difference_type>(size_);
     if (n < 0) n += size_;
     
     head_ = normalize_index(head_ + n);
     tail_ = normalize_index(tail_ + n);
 }
 
 // Helper functions
 template<typename T, typename Allocator>
 typename CircularArray<T, Allocator>::size_type
 CircularArray<T, Allocator>::normalize_index(difference_type index) const noexcept {
     if (index < 0) {
         index += capacity_;
     }
     return index % capacity_;
 }
 
 template<typename T, typename Allocator>
 void CircularArray<T, Allocator>::clear() noexcept {
     while (!empty()) {
         pop_front();
     }
 }
 
 template<typename T, typename Allocator>
 void CircularArray<T, Allocator>::destroy_range(pointer first, pointer last) {
     for (; first != last; ++first) {
         std::allocator_traits<Allocator>::destroy(alloc_, first);
     }
 }
 
 template<typename T, typename Allocator>
 void CircularArray<T, Allocator>::copy_elements(const CircularArray& other) {
     size_type count = other.size_;
     size_type src_pos = other.head_;
     
     for (size_type i = 0; i < count; ++i) {
         std::allocator_traits<Allocator>::construct(
             alloc_,
             buffer_ + i,
             other.buffer_[src_pos]
         );
         src_pos = (src_pos + 1) % other.capacity_;
     }
 }
 
 template<typename T, typename Allocator>
 void CircularArray<T, Allocator>::move_elements(CircularArray& other) {
     size_type count = other.size_;
     size_type src_pos = other.head_;
     
     for (size_type i = 0; i < count; ++i) {
         std::allocator_traits<Allocator>::construct(
             alloc_,
             buffer_ + i,
             std::move(other.buffer_[src_pos])
         );
         src_pos = (src_pos + 1) % other.capacity_;
     }
 }
 
 // Capacity methods
 template<typename T, typename Allocator>
 bool CircularArray<T, Allocator>::empty() const noexcept {
     return size_ == 0;
 }
 
 template<typename T, typename Allocator>
 bool CircularArray<T, Allocator>::full() const noexcept {
     return size_ == capacity_;
 }
 
 template<typename T, typename Allocator>
 typename CircularArray<T, Allocator>::size_type
 CircularArray<T, Allocator>::size() const noexcept {
     return size_;
 }
 
 template<typename T, typename Allocator>
 typename CircularArray<T, Allocator>::size_type
 CircularArray<T, Allocator>::capacity() const noexcept {
     return capacity_;
 }
 
 // Iterator methods
 template<typename T, typename Allocator>
 typename CircularArray<T, Allocator>::iterator
 CircularArray<T, Allocator>::begin() noexcept {
     return iterator(buffer_, head_, capacity_);
 }
 
 template<typename T, typename Allocator>
 typename CircularArray<T, Allocator>::const_iterator
 CircularArray<T, Allocator>::begin() const noexcept {
     return const_iterator(buffer_, head_, capacity_);
 }
 
 template<typename T, typename Allocator>
 typename CircularArray<T, Allocator>::const_iterator
 CircularArray<T, Allocator>::cbegin() const noexcept {
     return const_iterator(buffer_, head_, capacity_);
 }
 
 template<typename T, typename Allocator>
 typename CircularArray<T, Allocator>::iterator
 CircularArray<T, Allocator>::end() noexcept {
     return iterator(buffer_, tail_, capacity_);
 }
 
 template<typename T, typename Allocator>
 typename CircularArray<T, Allocator>::const_iterator
 CircularArray<T, Allocator>::end() const noexcept {
     return const_iterator(buffer_, tail_, capacity_);
 }
 
 template<typename T, typename Allocator>
 typename CircularArray<T, Allocator>::const_iterator
 CircularArray<T, Allocator>::cend() const noexcept {
     return const_iterator(buffer_, tail_, capacity_);
 }
 
 // Operators
 template<typename T, typename Allocator>
 CircularArray<T, Allocator>&
 CircularArray<T, Allocator>::operator=(const CircularArray& other) {
     if (this != &other) {
         // Clear existing elements
         clear();
         
         // Reallocate if necessary
         if (capacity_ != other.capacity_) {
             std::allocator_traits<Allocator>::deallocate(alloc_, buffer_, capacity_);
             capacity_ = other.capacity_;
             buffer_ = std::allocator_traits<Allocator>::allocate(alloc_, capacity_);
         }
         
         // Copy elements
         head_ = 0;
         tail_ = other.size_;
         size_ = other.size_;
         copy_elements(other);
         
         // Copy allocator if needed
         if (std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value) {
             alloc_ = other.alloc_;
         }
     }
     return *this;
 }
 
 template<typename T, typename Allocator>
 CircularArray<T, Allocator>&
 CircularArray<T, Allocator>::operator=(CircularArray&& other) noexcept {
     if (this != &other) {
         clear();
         std::allocator_traits<Allocator>::deallocate(alloc_, buffer_, capacity_);
         
         // Move members
         buffer_ = other.buffer_;
         head_ = other.head_;
         tail_ = other.tail_;
         capacity_ = other.capacity_;
         size_ = other.size_;
         
         // Move allocator if needed
         if (std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value) {
             alloc_ = std::move(other.alloc_);
         }
         
         // Reset other
         other.buffer_ = nullptr;
         other.head_ = other.tail_ = other.capacity_ = other.size_ = 0;
     }
     return *this;
 }
 
 template<typename T, typename Allocator>
 bool CircularArray<T, Allocator>::operator==(const CircularArray& other) const {
     if (size_ != other.size_) return false;
     
     for (size_type i = 0; i < size_; ++i) {
         if (at(i) != other.at(i)) return false;
     }
     return true;
 }
 
 template<typename T, typename Allocator>
 bool CircularArray<T, Allocator>::operator!=(const CircularArray& other) const {
     return !(*this == other);
 }
 
 } // namespace ds