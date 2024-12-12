/**
 * @file DynamicArray.cpp
 * @brief Implementation of Dynamic Array methods
 */

 #include "DynamicArray.h"

 namespace ds {
 
 template<typename T, typename Allocator>
 DynamicArray<T, Allocator>::DynamicArray(const Allocator& alloc)
     : alloc_(alloc)
     , data_(nullptr)
     , size_(0)
     , capacity_(0) {
 }
 
 template<typename T, typename Allocator>
 DynamicArray<T, Allocator>::DynamicArray(size_type count, const T& value, const Allocator& alloc)
     : alloc_(alloc)
     , data_(nullptr)
     , size_(count)
     , capacity_(count) {
     
     if (count > 0) {
         // Allocate memory
         data_ = std::allocator_traits<Allocator>::allocate(alloc_, count);
         
         // Construct elements
         for (size_type i = 0; i < count; ++i) {
             std::allocator_traits<Allocator>::construct(alloc_, data_ + i, value);
         }
     }
 }
 
 template<typename T, typename Allocator>
 DynamicArray<T, Allocator>::DynamicArray(std::initializer_list<T> init, const Allocator& alloc)
     : alloc_(alloc)
     , data_(nullptr)
     , size_(init.size())
     , capacity_(init.size()) {
     
     if (size_ > 0) {
         // Allocate memory
         data_ = std::allocator_traits<Allocator>::allocate(alloc_, size_);
         
         // Copy elements from initializer list
         std::uninitialized_copy(init.begin(), init.end(), data_);
     }
 }
 
 template<typename T, typename Allocator>
 DynamicArray<T, Allocator>::DynamicArray(const DynamicArray& other)
     : alloc_(std::allocator_traits<Allocator>::select_on_container_copy_construction(other.alloc_))
     , data_(nullptr)
     , size_(other.size_)
     , capacity_(other.size_) {
     
     if (size_ > 0) {
         // Allocate memory
         data_ = std::allocator_traits<Allocator>::allocate(alloc_, size_);
         
         // Copy elements
         try {
             std::uninitialized_copy(other.data_, other.data_ + size_, data_);
         } catch (...) {
             std::allocator_traits<Allocator>::deallocate(alloc_, data_, size_);
             throw;
         }
     }
 }
 
 template<typename T, typename Allocator>
 DynamicArray<T, Allocator>::DynamicArray(DynamicArray&& other) noexcept
     : alloc_(std::move(other.alloc_))
     , data_(other.data_)
     , size_(other.size_)
     , capacity_(other.capacity_) {
     
     // Reset other
     other.data_ = nullptr;
     other.size_ = 0;
     other.capacity_ = 0;
 }
 
 template<typename T, typename Allocator>
 DynamicArray<T, Allocator>::~DynamicArray() {
     clear();
     if (data_) {
         std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
     }
 }
 
 // Element access
 template<typename T, typename Allocator>
 typename DynamicArray<T, Allocator>::reference 
 DynamicArray<T, Allocator>::at(size_type pos) {
     if (pos >= size_) {
         throw std::out_of_range("Index out of range");
     }
     return data_[pos];
 }
 
 template<typename T, typename Allocator>
 typename DynamicArray<T, Allocator>::const_reference 
 DynamicArray<T, Allocator>::at(size_type pos) const {
     if (pos >= size_) {
         throw std::out_of_range("Index out of range");
     }
     return data_[pos];
 }
 
 template<typename T, typename Allocator>
 typename DynamicArray<T, Allocator>::reference 
 DynamicArray<T, Allocator>::operator[](size_type pos) {
     return data_[pos];
 }
 
 template<typename T, typename Allocator>
 typename DynamicArray<T, Allocator>::const_reference 
 DynamicArray<T, Allocator>::operator[](size_type pos) const {
     return data_[pos];
 }
 
 // Modifiers
 template<typename T, typename Allocator>
 void DynamicArray<T, Allocator>::push_back(const T& value) {
     if (size_ == capacity_) {
         reallocate(calculate_growth(size_ + 1));
     }
     std::allocator_traits<Allocator>::construct(alloc_, data_ + size_, value);
     ++size_;
 }
 
 template<typename T, typename Allocator>
 void DynamicArray<T, Allocator>::push_back(T&& value) {
     if (size_ == capacity_) {
         reallocate(calculate_growth(size_ + 1));
     }
     std::allocator_traits<Allocator>::construct(alloc_, data_ + size_, std::move(value));
     ++size_;
 }
 
 template<typename T, typename Allocator>
 void DynamicArray<T, Allocator>::pop_back() {
     if (size_ > 0) {
         --size_;
         std::allocator_traits<Allocator>::destroy(alloc_, data_ + size_);
     }
 }
 
 // Private helper methods
 template<typename T, typename Allocator>
 void DynamicArray<T, Allocator>::reallocate(size_type new_capacity) {
     // Allocate new buffer
     pointer new_data = std::allocator_traits<Allocator>::allocate(alloc_, new_capacity);
     
     // Move existing elements
     for (size_type i = 0; i < size_; ++i) {
         std::allocator_traits<Allocator>::construct(alloc_, new_data + i, 
             std::move_if_noexcept(data_[i]));
     }
     
     // Destroy old elements
     destroy_range(data_, data_ + size_);
     
     // Deallocate old buffer
     if (data_) {
         std::allocator_traits<Allocator>::deallocate(alloc_, data_, capacity_);
     }
     
     // Update pointers and capacity
     data_ = new_data;
     capacity_ = new_capacity;
 }
 
 template<typename T, typename Allocator>
 typename DynamicArray<T, Allocator>::size_type 
 DynamicArray<T, Allocator>::calculate_growth(size_type new_size) const {
     const size_type new_capacity = static_cast<size_type>(capacity_ * GROWTH_FACTOR);
     return std::max(new_capacity, std::max(INITIAL_CAPACITY, new_size));
 }
 
 template<typename T, typename Allocator>
 void DynamicArray<T, Allocator>::destroy_range(pointer first, pointer last) {
     for (; first != last; ++first) {
         std::allocator_traits<Allocator>::destroy(alloc_, first);
     }
 }
 
 // Clear
 template<typename T, typename Allocator>
 void DynamicArray<T, Allocator>::clear() noexcept {
     destroy_range(data_, data_ + size_);
     size_ = 0;
 }
 
 // Capacity methods
 template<typename T, typename Allocator>
 void DynamicArray<T, Allocator>::reserve(size_type new_cap) {
     if (new_cap > capacity_) {
         reallocate(new_cap);
     }
 }
 
 template<typename T, typename Allocator>
 void DynamicArray<T, Allocator>::shrink_to_fit() {
     if (size_ < capacity_) {
         reallocate(size_);
     }
 }
 
 // Iterator methods
 template<typename T, typename Allocator>
 typename DynamicArray<T, Allocator>::iterator 
 DynamicArray<T, Allocator>::begin() noexcept {
     return data_;
 }
 
 template<typename T, typename Allocator>
 typename DynamicArray<T, Allocator>::const_iterator 
 DynamicArray<T, Allocator>::begin() const noexcept {
     return data_;
 }
 
 template<typename T, typename Allocator>
 typename DynamicArray<T, Allocator>::iterator 
 DynamicArray<T, Allocator>::end() noexcept {
     return data_ + size_;
 }
 
 template<typename T, typename Allocator>
 typename DynamicArray<T, Allocator>::const_iterator 
 DynamicArray<T, Allocator>::end() const noexcept {
     return data_ + size_;
 }
 
 } // namespace ds