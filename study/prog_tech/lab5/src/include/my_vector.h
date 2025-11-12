#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <initializer_list>



template<typename T>
class my_vector {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    void grow_to(std::size_t new_cap) {
        if (new_cap <= capacity_) return;
        T* new_data = static_cast<T*>(operator new[](new_cap * sizeof(T)));
        for (std::size_t i = 0; i < size_; ++i) {
            new (new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        operator delete[](data_);
        data_ = new_data;
        capacity_ = new_cap;
    }

public:
    my_vector() : data_(nullptr), size_(0), capacity_(0) {}

    explicit my_vector(std::size_t n) : data_(nullptr), size_(0), capacity_(0) {
        if (n > 0) {
            data_ = static_cast<T*>(operator new[](n * sizeof(T)));
            for (std::size_t i = 0; i < n; ++i) new (data_ + i) T();
            size_ = n;
            capacity_ = n;
        }
    }

    my_vector(std::initializer_list<T> il) : data_(nullptr), size_(0), capacity_(0) {
        reserve(il.size());
        for (const T& v : il) push_back(v);
    }

    ~my_vector() {
        clear();
        operator delete[](data_);
    }

    my_vector(const my_vector& other) : data_(nullptr), size_(0), capacity_(0) {
        if (other.size_ > 0) {
            reserve(other.size_);
            for (std::size_t i = 0; i < other.size_; ++i) new (data_ + i) T(other.data_[i]);
            size_ = other.size_;
        }
    }

    my_vector& operator=(const my_vector& other) {
        if (this == &other) return *this;
        clear();
        reserve(other.size_);
        for (std::size_t i = 0; i < other.size_; ++i) new (data_ + i) T(other.data_[i]);
        size_ = other.size_;
        return *this;
    }

    my_vector(my_vector&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    my_vector& operator=(my_vector&& other) noexcept {
        if (this == &other) return *this;
        clear();
        operator delete[](data_);
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        return *this;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) reserve(capacity_ ? capacity_ * 2 : 1);
        new (data_ + size_) T(value);
        ++size_;
    }

    void push_back(T&& value) {
        if (size_ == capacity_) reserve(capacity_ ? capacity_ * 2 : 1);
        new (data_ + size_) T(std::move(value));
        ++size_;
    }

    void pop_back() {
        if (size_ == 0) throw std::out_of_range("pop_back from empty my_vector");
        --size_;
        data_[size_].~T();
    }

    T& operator[](std::size_t idx) { return data_[idx]; }
    const T& operator[](std::size_t idx) const { return data_[idx]; }

    T& at(std::size_t idx) {
        if (idx >= size_) throw std::out_of_range("my_vector::at");
        return data_[idx];
    }
    const T& at(std::size_t idx) const {
        if (idx >= size_) throw std::out_of_range("my_vector::at");
        return data_[idx];
    }

    std::size_t size() const noexcept { return size_; }
    std::size_t capacity() const noexcept { return capacity_; }
    bool empty() const noexcept { return size_ == 0; }

    void reserve(std::size_t new_cap) {
        if (new_cap <= capacity_) return;
        T* new_data = static_cast<T*>(operator new[](new_cap * sizeof(T)));
        for (std::size_t i = 0; i < size_; ++i) {
            new (new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        operator delete[](data_);
        data_ = new_data;
        capacity_ = new_cap;
    }

    void clear() noexcept {
        for (std::size_t i = 0; i < size_; ++i) data_[i].~T();
        size_ = 0;
    }


    void erase_at(std::size_t idx) {
        if (idx >= size_) throw std::out_of_range("erase_at: index out of range");

        for (std::size_t i = idx; i < size_ - 1; ++i) {
            data_[i] = std::move(data_[i + 1]);
        }
        --size_;
        data_[size_].~T();
    }


    std::size_t erase_index(std::size_t idx) {
        erase_at(idx);
        return idx;
    }


    T* begin() noexcept { return data_; }
    T* end() noexcept { return data_ + size_; }
    const T* begin() const noexcept { return data_; }
    const T* end() const noexcept { return data_ + size_; }
    const T* cbegin() const noexcept { return data_; }
    const T* cend() const noexcept { return data_ + size_; }
};

#endif // MY_VECTOR_H