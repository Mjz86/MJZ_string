// ConsoleApplication3.cpp : This file contains the 'main' function. Program
// execution begins and ends there.
//
#include "mjzString.hpp"
#include "pch.h"
#include "start.h"
/* File Handling with C++ using ifstream & ofstream class object*/
/* To write the Content in File*/
/* Then to read the content of file*/
#include <iostream>

/* fstream header file for ifstream, ofstream,
 fstream classes */
#include <fstream>

class my_main : public main_class {
 public:
 int main(int argc, const char* const* const argv) override;
};

std::unique_ptr<main_class> main_class::run(int, const char* const* const) {
 return std::make_unique<my_main>();
}

#include "fstream"
int my_main::main(int argc, const char* const* const argv) {
 using namespace mjz_ard::have_mjz_ard_removed;

 ///////////////////////////////////////////////////////////////////////////////////////
 ////////////////////// create a array data structure/////////////////////////////////////
 return 0; //////////////////////////////////////////////////////////////////////////
}

#include <iostream>
#include <stdexcept>

template <typename T, size_t Size>
class MyArray {
public:
    // Default constructor
    MyArray() : elements{} {}

    // Element access
    T& operator[](size_t index) {
        if (index >= Size) {
            throw std::out_of_range("Index out of range");
        }
        return elements[index];
    }

    const T& operator[](size_t index) const {
        if (index >= Size) {
            throw std::out_of_range("Index out of range");
        }
        return elements[index];
    }

    T& at(size_t index) {
        if (index >= Size) {
            throw std::out_of_range("Index out of range");
        }
        return elements[index];
    }

    const T& at(size_t index) const {
        if (index >= Size) {
            throw std::out_of_range("Index out of range");
        }
        return elements[index];
    }

    T& front() {
        return elements[0];
    }

    const T& front() const {
        return elements[0];
    }

    T& back() {
        return elements[Size - 1];
    }

    const T& back() const {
        return elements[Size - 1];
    }

    T* data() noexcept {
        return elements;
    }

    const T* data() const noexcept {
        return elements;
    }

    // Capacity
    bool empty() const noexcept {
        return Size == 0;
    }

    size_t size() const noexcept {
        return Size;
    }

    size_t max_size() const noexcept {
        return Size;
    }

    // Modifiers
    void fill(const T& value) {
        for (size_t i = 0; i < Size; ++i) {
            elements[i] = value;
        }
    }

    void swap(MyArray& other) noexcept(std::is_nothrow_swappable_v<T>) {
        for (size_t i = 0; i < Size; ++i) {
            std::swap(elements[i], other.elements[i]);
        }
    }

private:
    T elements[Size];
};

template <typename T, typename U = T>
T exchange(T& obj, U&& new_value) {
    T old_value = std::move(obj);
    obj = std::forward<U>(new_value);
    return old_value;
}
template <typename T, typename Allocator = std::allocator<T>>
class MyVector {
public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    MyVector() : MyVector(Allocator{}) {}
    explicit MyVector(const Allocator& a) : alloc_{a}, size_{0}, capacity_{0}, data_{nullptr} {}
    explicit MyVector(size_type n, const T& val, const Allocator& a = Allocator{}) : alloc_{a}, size_{n}, capacity_{n}, data_{alloc_.allocate(n)} { std::uninitialized_fill_n(data_, n, val); }
    explicit MyVector(size_type n, const Allocator& a = Allocator{}) : MyVector(n, T{}, a) {}
    template <typename InputIt> MyVector(InputIt first, InputIt last, const Allocator& a = Allocator{}) : MyVector(std::distance(first, last), a) { std::copy(first, last, begin()); }
    MyVector(const MyVector& other) : alloc_{other.alloc_}, size_{other.size_}, capacity_{other.capacity_} { data_ = alloc_.allocate(capacity_); std::uninitialized_copy(other.begin(), other.end(), begin()); }
    MyVector(const MyVector& other, const Allocator& a) : alloc_{a}, size_{other.size_}, capacity_{other.capacity_} { data_ = alloc_.allocate(capacity_); std::uninitialized_copy(other.begin(), other.end(), begin()); }
    MyVector(MyVector&& other) noexcept : alloc_{std::move(other.alloc_)}, size_{std::exchange(other.size_, 0)}, capacity_{std::exchange(other.capacity_, 0)}, data_{std::exchange(other.data_, nullptr)} {}
    MyVector(MyVector&& other, const Allocator& a) : alloc_{a}, size_{std::exchange(other.size_, 0)}, capacity_{std::exchange(other.capacity_, 0)}, data_{std::exchange(other.data_, nullptr)} {}
    MyVector(std::initializer_list<T> il, const Allocator& a = Allocator{}) : MyVector(il.begin(), il.end(), a) {}
    ~MyVector() { clear(); deallocate(); }
    MyVector& operator=(const MyVector& other) { if (this != &other) { clear(); deallocate(); alloc_ = other.alloc_; size_ = other.size_; capacity_ = other.capacity_; data_ = alloc_.allocate(capacity_); std::uninitialized_copy(other.begin(), other.end(), begin()); } return *this; }
    MyVector& operator=(MyVector&& other) noexcept { if (this != &other) { clear(); deallocate(); alloc_ = std::move(other.alloc_); size_ = std::exchange(other.size_, 0); capacity_ = std::exchange(other.capacity_, 0); data_ = std::exchange(other.data_, nullptr); } return *this; }
    MyVector& operator=(std::initializer_list<T> il) { *this = MyVector(il); return *this; }
    reference operator[](size_type pos) { return data_[pos]; }
    const_reference operator[](size_type pos) const { return data_[pos]; }
    reference at(size_type pos) { check_range(pos); return data_[pos]; }
    const_reference at(size_type pos) const { check_range(pos); return data_[pos]; }
    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *(end() - 1); }
    const_reference back() const { return *(end() - 1); }
    T* data() noexcept { return data_; }
    const T* data() const noexcept { return data_; }
    iterator begin() noexcept { return data_; }
    const_iterator begin() const noexcept { return data_; }
    iterator end() noexcept { return data_ + size_; }
    const_iterator end() const noexcept { return data_ + size_; }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_iterator crend() const noexcept { return rend(); }
    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }
    size_type max_size() const noexcept { return std::allocator_traits<Allocator>::max_size(alloc_); }
    void reserve(size_type n) { if (n > capacity_) reallocate(n); }
    size_type capacity() const noexcept { return capacity_; }
    void shrink_to_fit() { if (size_ < capacity_) reallocate(size_); }
    void clear() noexcept { destroy(begin(), end()); size_ = 0; }
    iterator insert(const_iterator pos, const T& val) { return insert(pos, 1, val); }
    iterator insert(const_iterator pos, T&& val) { emplace(pos, std::move(val)); return pos; }
    iterator insert(const_iterator pos, size_type count, const T& val) { auto offset = pos - cbegin(); reallocate(size_ + count); std::move_backward(begin() + offset, end() - count, end()); std::uninitialized_fill_n(begin() + offset, count, val); size_ += count; return begin() + offset; }
    template <typename InputIt> iterator insert(const_iterator pos, InputIt first, InputIt last) { auto offset = pos - cbegin(); auto count = std::distance(first, last); reallocate(size_ + count); std::move_backward(begin() + offset, end() - count, end()); std::uninitialized_copy(first, last, begin() + offset); size_ += count; return begin() + offset; }
    iterator insert(const_iterator pos, std::initializer_list<T> ilist) { return insert(pos, ilist.begin(), ilist.end()); }
    template <typename... Args> iterator emplace(const_iterator pos, Args&&... args) { auto offset = pos - cbegin(); reallocate(size_ + 1); std::move_backward(begin() + offset, end() - 1, end()); allocator_traits<Allocator>::construct(alloc_, data_ + offset, std::forward<Args>(args)...); ++size_; return begin() + offset; }
    iterator erase(const_iterator pos) { return erase(pos, pos + 1); }
    iterator erase(const_iterator first, const_iterator last) { auto offset = first - cbegin(); auto count = last - first; destroy(begin() + offset, end()); std::move(begin() + offset + count, end(), begin() + offset); size_ -= count; return begin() + offset; }
    template <typename... Args>
    reference emplace_back(Args&&... args) {
        if (size_ == capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        alloc_.construct(data_ + size_, std::forward<Args>(args)...);
        ++size_;
        return back();
    }

    void pop_back() {
        if (size_ > 0) {
            --size_;
            alloc_.destroy(data_ + size_);
        }
    }

private:
    allocator_type alloc_;
    size_type size_;
    size_type capacity_;
    pointer data_;

    // Helper functions
    void deallocate() {
        if (data_) {
            for (size_type i = 0; i < size_; ++i) {
                alloc_.destroy(data_ + i);
            }
            alloc_.deallocate(data_, capacity_);
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;
        }
    }

    void reallocate(size_type newCapacity) {
        pointer newData = alloc_.allocate(newCapacity);
        std::uninitialized_move_n(data_, size_, newData);
        deallocate();
        data_ = newData;
        capacity_ = newCapacity;
    }
};

