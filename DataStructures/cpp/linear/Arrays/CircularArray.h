/**
 * @file CircularArray.h
 * @brief Circular Array Implementation
 * @date 2025-02-16
 * 
 * This file implements a circular array (ring buffer) that provides efficient
 * constant-time insertions and deletions at both ends of the sequence.
 * 
 * Time Complexities:
 * - Access (operator[]): O(1)
 * - Insert at front/back: O(1)
 * - Delete from front/back: O(1)
 * - Search: O(n)
 * - Rotation: O(1)
 * 
 * Space Complexity:
 * - O(n) where n is the capacity of the array
 * 
 * Common Use Cases:
 * - Circular buffers for streaming data
 * - Implementation of queues and deques
 * - Real-time data processing
 * - Audio/Video buffering
 */

 #ifndef CIRCULAR_ARRAY_H
 #define CIRCULAR_ARRAY_H
 
 #include <cstddef>
 #include <stdexcept>
 #include <memory>
 #include <iterator>
 #include <type_traits>
 
 namespace ds {
 
 template<typename T, typename Allocator = std::allocator<T>>
 class CircularArray {
 public:
     // Type definitions for STL compatibility
     using value_type = T;
     using allocator_type = Allocator;
     using size_type = std::size_t;
     using difference_type = std::ptrdiff_t;
     using reference = value_type&;
     using const_reference = const value_type&;
     using pointer = typename std::allocator_traits<Allocator>::pointer;
     using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
 
     // Custom iterator class for circular traversal
     template<bool IsConst>
     class CircularIterator {
     public:
         using iterator_category = std::random_access_iterator_tag;
         using value_type = std::conditional_t<IsConst, const T, T>;
         using difference_type = std::ptrdiff_t;
         using pointer = std::conditional_t<IsConst, const T*, T*>;
         using reference = std::conditional_t<IsConst, const T&, T&>;
 
         // Iterator constructors and operators
         CircularIterator() noexcept;
         reference operator*() const;
         pointer operator->() const;
         CircularIterator& operator++();
         CircularIterator operator++(int);
         CircularIterator& operator--();
         CircularIterator operator--(int);
         CircularIterator& operator+=(difference_type n);
         CircularIterator operator+(difference_type n) const;
         CircularIterator& operator-=(difference_type n);
         CircularIterator operator-(difference_type n) const;
         difference_type operator-(const CircularIterator& other) const;
         bool operator==(const CircularIterator& other) const;
         bool operator!=(const CircularIterator& other) const;
         bool operator<(const CircularIterator& other) const;
         bool operator>(const CircularIterator& other) const;
         bool operator<=(const CircularIterator& other) const;
         bool operator>=(const CircularIterator& other) const;
 
     private:
         friend class CircularArray;
         pointer buffer_;
         size_type position_;
         size_type capacity_;
         
         CircularIterator(pointer buffer, size_type position, size_type capacity);
     };
 
     using iterator = CircularIterator<false>;
     using const_iterator = CircularIterator<true>;
     using reverse_iterator = std::reverse_iterator<iterator>;
     using const_reverse_iterator = std::reverse_iterator<const_iterator>;
 
     /**
      * @brief Default constructor
      * @param capacity Initial capacity of the circular array
      * @complexity Time: O(1), Space: O(n)
      */
     explicit CircularArray(size_type capacity = 16, const Allocator& alloc = Allocator());
 
     /**
      * @brief Constructor with initial values
      * @param count Number of elements
      * @param value Value to fill the array with
      * @complexity Time: O(n), Space: O(n)
      */
     CircularArray(size_type count, const T& value, const Allocator& alloc = Allocator());
 
     /**
      * @brief Copy constructor
      * @param other Array to copy from
      * @complexity Time: O(n), Space: O(n)
      */
     CircularArray(const CircularArray& other);
 
     /**
      * @brief Move constructor
      * @param other Array to move from
      * @complexity Time: O(1), Space: O(1)
      */
     CircularArray(CircularArray&& other) noexcept;
 
     /**
      * @brief Destructor
      * @complexity Time: O(n), Space: O(1)
      */
     ~CircularArray();
 
     // Element access
     reference at(size_type pos);
     const_reference at(size_type pos) const;
     reference operator[](size_type pos);
     const_reference operator[](size_type pos) const;
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
     bool full() const noexcept;
     size_type size() const noexcept;
     size_type capacity() const noexcept;
 
     // Modifiers
     void push_front(const T& value);
     void push_front(T&& value);
     void push_back(const T& value);
     void push_back(T&& value);
     void pop_front();
     void pop_back();
     void clear() noexcept;
     
     /**
      * @brief Rotates the array by the specified number of positions
      * @param n Number of positions to rotate (positive for right, negative for left)
      * @complexity Time: O(1), Space: O(1)
      */
     void rotate(difference_type n);
 
     // Operators
     CircularArray& operator=(const CircularArray& other);
     CircularArray& operator=(CircularArray&& other) noexcept;
     bool operator==(const CircularArray& other) const;
     bool operator!=(const CircularArray& other) const;
 
 private:
     Allocator alloc_;             // Allocator instance
     pointer buffer_;              // Pointer to the data buffer
     size_type head_;             // Index of the first element
     size_type tail_;             // Index one past the last element
     size_type capacity_;         // Total capacity of the buffer
     size_type size_;            // Current number of elements
 
     // Helper functions
     size_type normalize_index(difference_type index) const noexcept;
     void destroy_range(pointer first, pointer last);
     void copy_elements(const CircularArray& other);
     void move_elements(CircularArray& other);
 };
 
 } // namespace ds
 
 #endif // CIRCULAR_ARRAY_H