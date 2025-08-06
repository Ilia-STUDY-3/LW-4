#ifndef STRING_HPP
#define STRING_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <iostream>
#include <string>

template<typename T>
class String {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    void copy_from(const T* src, std::size_t count) {
        size_ = capacity_ = count;
        data_ = (count > 0) ? new T[capacity_] : nullptr;
        for (std::size_t i = 0; i < count; ++i) {
            data_[i] = src[i];
        }
    }

public:
    String() : data_(nullptr), size_(0), capacity_(0) {}

    String(const String& other) { copy_from(other.data_, other.size_); }

    String(String&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    String(std::size_t count, const T& ch) : size_(count), capacity_(count) {
        data_ = (count > 0) ? new T[count] : nullptr;
        for (std::size_t i = 0; i < count; ++i) {
            data_[i] = ch;
        }
    }

    String(const T* cstr) : size_(0), capacity_(0), data_(nullptr) {
        if (cstr) {
            while (cstr[size_] != T{}) ++size_;
            copy_from(cstr, size_);
        }
    }

    String(const T* begin, const T* end) {
        if (begin > end) throw std::invalid_argument("begin must not be greater than end");
        size_ = end - begin;
        copy_from(begin, size_);
    }

    template<typename U>
    explicit String(const String<U>& other) {
        size_ = capacity_ = other.size();
        data_ = (size_ > 0) ? new T[size_] : nullptr;
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = static_cast<T>(other[i]);
        }
    }

    ~String() { delete[] data_; }

    String& operator=(const String& other) {
        if (this != &other) {
            delete[] data_;
            copy_from(other.data_, other.size_);
        }
        return *this;
    }

    String& operator=(String&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    std::size_t size() const { return size_; }

    bool empty() const { return size_ == 0; }

    void clear() {
        delete[] data_;
        data_ = nullptr;
        size_ = capacity_ = 0;
    }

    T& operator[](std::size_t index) {
        if (index >= size_) throw std::out_of_range("Index out of bounds");
        return data_[index];
    }

    const T& operator[](std::size_t index) const {
        if (index >= size_) throw std::out_of_range("Index out of bounds");
        return data_[index];
    }

    String substr(std::size_t pos, std::size_t len) const {
        if (pos > size_) throw std::out_of_range("substr: position out of range");
        if (pos + len > size_) len = size_ - pos;
        return String(data_ + pos, data_ + pos + len);
    }

    String operator+(const String& other) const {
        String result;
        result.size_ = result.capacity_ = size_ + other.size_;
        result.data_ = new T[result.size_];
        for (std::size_t i = 0; i < size_; ++i) result.data_[i] = data_[i];
        for (std::size_t i = 0; i < other.size_; ++i) result.data_[size_ + i] = other.data_[i];
        return result;
    }

    String operator+(const T& ch) const {
        String result;
        result.size_ = result.capacity_ = size_ + 1;
        result.data_ = new T[result.size_];
        for (std::size_t i = 0; i < size_; ++i) result.data_[i] = data_[i];
        result.data_[size_] = ch;
        return result;
    }

    friend String operator+(const T& ch, const String& str) {
        String result;
        result.size_ = result.capacity_ = str.size_ + 1;
        result.data_ = new T[result.size_];
        result.data_[0] = ch;
        for (std::size_t i = 0; i < str.size_; ++i) result.data_[i + 1] = str.data_[i];
        return result;
    }

    void push_back(const T& ch) {
        if (size_ == capacity_) {
            capacity_ = capacity_ * 2 + 1;
            T* new_data = new T[capacity_];
            for (std::size_t i = 0; i < size_; ++i) new_data[i] = data_[i];
            delete[] data_;
            data_ = new_data;
        }
        data_[size_++] = ch;
    }

    String operator*(int count) const {
        if (count <= 0) return String();
        String result;
        result.size_ = result.capacity_ = size_ * count;
        result.data_ = new T[result.size_];
        for (int i = 0; i < count; ++i)
            for (std::size_t j = 0; j < size_; ++j)
                result.data_[i * size_ + j] = data_[j];
        return result;
    }

    friend String operator*(int count, const String& str) {
        return str * count;
    }

    bool operator==(const String& other) const {
        if (size_ != other.size_) return false;
        for (std::size_t i = 0; i < size_; ++i)
            if (!(data_[i] == other.data_[i])) return false;
        return true;
    }

    bool operator!=(const String& other) const {
        return !(*this == other);
    }

    bool operator<(const String& other) const {
        std::size_t min_len = (size_ < other.size_) ? size_ : other.size_;
        for (std::size_t i = 0; i < min_len; ++i) {
            if (data_[i] < other.data_[i]) return true;
            if (data_[i] > other.data_[i]) return false;
        }
        return size_ < other.size_;
    }

    bool operator<=(const String& other) const { return *this < other || *this == other; }
    bool operator>(const String& other) const { return !(*this <= other); }
    bool operator>=(const String& other) const { return !(*this < other); }

    template <typename Trans>
    void transform_inplace(const Trans& t) {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = t(data_[i]);
        }
    }

    template <typename Trans>
    String transform_copy(const Trans& t) const {
        String result(*this);
        result.transform_inplace(t);
        return result;
    }
};

// I/O только для String<char>
std::ostream& operator<<(std::ostream& os, const String<char>& str) {
    for (std::size_t i = 0; i < str.size(); ++i) {
        os << str[i];
    }
    return os;
}

std::istream& operator>>(std::istream& is, String<char>& str) {
    std::string temp;
    is >> temp;
    str = String<char>(temp.c_str());
    return is;
}

#endif // STRING_HPP
