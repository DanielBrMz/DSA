/**
 * @file DynamicArray.h
 * @brief Dynamic Array (Vector) Implementation
 * @author Your Name
 * @date 2025-02-16
 * 
 * This file implements a dynamic array (similar to std::vector) that automatically
 * resizes as elements are added or removed. The implementation uses a growth factor
 * of 1.5 for efficient amortized operations.
 * 
 * Time Complexities:
 * - Access (operator[]): O(1)
 * - Insert at end (push_back): O(1) amortized
 * - Insert at arbitrary position: O(n)
 * - Delete at end (pop_back): O(1)
 * - Delete at arbitrary position: O(n)
 * - Search: O(n)
 * 
 * Space Complexity:
 * - O(n) where n is the number of elements
 * - Extra space up to 50% of capacity due to growth factor
 */

 #ifndef DYNAMIC_ARRAY_H
 #define DYNAMIC_ARRAY_H
 
 #include <cstddef>      // for size_t
 #include <stdexcept>    // for std::out_of_range
 #include <initializer_list>
 #include <algorithm>    // for std::copy
 #include <memory>       // for std::allocator
 
 namespace ds {
 
 template<typename T, typename Allocator = std::allocator<T>>
 class DynamicArray {
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
     using iterator = pointer;
     using const_iterator = const_pointer;
 
     /**
      * @brief Default constructor
      * @complexity Time: O(1), Space: O(1)
      */
     explicit DynamicArray(const Allocator& alloc = Allocator());
 
     /**
      * @brief Constructor with initial size
      * @param count Initial size of the array
      * @param value Value to fill the array with
      * @complexity Time: O(n), Space: O(n)
      */
     explicit DynamicArray(size_type count, const T& value = T(), 
                          const Allocator& alloc = Allocator());
 
     /**
      * @brief Constructor from initializer list
      * @param init Initializer list
      * @complexity Time: O(n), Space: O(n)
      */
     DynamicArray(std::initializer_list<T> init, 
                  const Allocator& alloc = Allocator());
 
     /**
      * @brief Copy constructor
      * @param other Array to copy from
      * @complexity Time: O(n), Space: O(n)
      */
     DynamicArray(const DynamicArray& other);
 
     /**
      * @brief Move constructor
      * @param other Array to move from
      * @complexity Time: O(1), Space: O(1)
      */
     DynamicArray(DynamicArray&& other) noexcept;
 
     /**
      * @brief Destructor
      * @complexity Time: O(n), Space: O(1)
      */
     ~DynamicArray();
 
     // Element access
     reference at(size_type pos);
     const_reference at(size_type pos) const;
     reference operator[](size_type pos);
     const_reference operator[](size_type pos) const;
     reference front();
     const_reference front() const;
     reference back();
     const_reference back() const;
     T* data() noexcept;
     const T* data() const noexcept;
 
     // Iterators
     iterator begin() noexcept;
     const_iterator begin() const noexcept;
     iterator end() noexcept;
     const_iterator end() const noexcept;
 
     // Capacity
     bool empty() const noexcept;
     size_type size() const noexcept;
     size_type capacity() const noexcept;
     void reserve(size_type new_cap);
     void shrink_to_fit();
 
     // Modifiers
     void clear() noexcept;
     iterator insert(const_iterator pos, const T& value);
     iterator insert(const_iterator pos, T&& value);
     iterator erase(const_iterator pos);
     void push_back(const T& value);
     void push_back(T&& value);
     void pop_back();
     void resize(size_type count);
     void resize(size_type count, const value_type& value);
 
     // Operators
     DynamicArray& operator=(const DynamicArray& other);
     DynamicArray& operator=(DynamicArray&& other) noexcept;
     bool operator==(const DynamicArray& other) const;
     bool operator!=(const DynamicArray& other) const;
 
 private:
     static constexpr float GROWTH_FACTOR = 1.5f;
     static constexpr size_type INITIAL_CAPACITY = 16;
 
     Allocator alloc_;           // Allocator instance
     pointer data_;             // Pointer to the data
     size_type size_;          // Current number of elements
     size_type capacity_;      // Current capacity
 
     // Helper functions
     void reallocate(size_type new_capacity);
     size_type calculate_growth(size_type new_size) const;
     void destroy_range(pointer first, pointer last);
 };
 
 } // namespace ds
 
 #endif // DYNAMIC_ARRAY_H