#pragma once
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <utility>
#include <cstring>

// Пользовательское исключение
class StringException : public std::runtime_error {
public:
    explicit StringException(const std::string& msg) : std::runtime_error(msg) {}
};

template<typename T>
class String {
private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void allocate(size_t new_cap) {
        T* new_data = new T[new_cap + 1]{};
        if (data_) {
            std::copy(data_, data_ + size_, new_data);
            delete[] data_;
        }
        data_ = new_data;
        capacity_ = new_cap;
        data_[size_] = T{};
    }

    void copy_from(const String& other) {
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = new T[capacity_ + 1];
        std::copy(other.data_, other.data_ + size_, data_);
        data_[size_] = T{};
    }

public:
    // Конструкторы
    String() : data_(new T[1]{T{}}), size_(0), capacity_(0) {}

    String(size_t n, const T& ch) : size_(n), capacity_(n) {
        data_ = new T[n + 1];
        std::fill(data_, data_ + n, ch);
        data_[n] = T{};
    }

    String(const T* cstr) {
        if (!cstr)
            throw StringException("nullptr passed to constructor");
        size_ = 0;
        while (cstr[size_] != T{})
            ++size_;
        capacity_ = size_;
        data_ = new T[size_ + 1];
        std::copy(cstr, cstr + size_, data_);
        data_[size_] = T{};
    }

    String(const T* begin, const T* end) {
        if (!begin || !end || end < begin)
            throw std::invalid_argument("Invalid range in constructor");
        size_ = static_cast<size_t>(end - begin);
        capacity_ = size_;
        data_ = new T[size_ + 1];
        std::copy(begin, end, data_);
        data_[size_] = T{};
    }

    template<typename U>
    explicit String(const String<U>& other)
        : size_(other.size()), capacity_(other.size()) {
        data_ = new T[capacity_ + 1];
        std::transform(other.begin(), other.end(), data_, [](const U& ch) {
            return static_cast<T>(ch);
        });
        data_[size_] = T{};
    }

    // Копирование и перемещение
    String(const String& other) { copy_from(other); }

    String(String&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = other.capacity_ = 0;
    }

    ~String() { delete[] data_; }

    String& operator=(String other) {
        swap(*this, other);
        return *this;
    }

    friend void swap(String& a, String& b) noexcept {
        using std::swap;
        swap(a.data_, b.data_);
        swap(a.size_, b.size_);
        swap(a.capacity_, b.capacity_);
    }

    // Методы
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    void clear() {
        delete[] data_;
        data_ = new T[1]{T{}};
        size_ = 0;
        capacity_ = 0;
    }

    const T& operator[](size_t i) const {
        if (i >= size_) throw std::out_of_range("Index out of range");
        return data_[i];
    }

    T& operator[](size_t i) {
        if (i >= size_) throw std::out_of_range("Index out of range");
        return data_[i];
    }

    String substr(size_t pos, size_t len) const {
        if (pos > size_) throw std::out_of_range("Substring position out of range");
        size_t actual_len = std::min(len, size_ - pos);
        return String(data_ + pos, data_ + pos + actual_len);
    }

    void push_back(const T& ch) {
        if (size_ == capacity_) {
            size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
            reserve(new_cap);
        }
        data_[size_++] = ch;
        data_[size_] = T{};
    }

    void reserve(size_t new_cap) {
        if (new_cap > capacity_)
            allocate(new_cap);
    }

    // Итераторы
    T* begin() { return data_; }
    T* end() { return data_ + size_; }
    const T* begin() const { return data_; }
    const T* end() const { return data_ + size_; }

    // Конкатенация
    friend String operator+(const String& lhs, const String& rhs) {
        String res(lhs.size_ + rhs.size_, T{});
        std::copy(lhs.data_, lhs.data_ + lhs.size_, res.data_);
        std::copy(rhs.data_, rhs.data_ + rhs.size_, res.data_ + lhs.size_);
        res.data_[res.size_] = T{};
        return res;
    }

    friend String operator+(const String& lhs, const T& ch) {
        String res(lhs);
        res.push_back(ch);
        return res;
    }

    friend String operator+(const T& ch, const String& rhs) {
        String res(1, ch);
        return res + rhs;
    }

    // Повторение
    friend String operator*(const String& str, size_t times) {
        String res;
        res.reserve(str.size_ * times);
        for (size_t i = 0; i < times; ++i)
            for (size_t j = 0; j < str.size_; ++j)
                res.push_back(str.data_[j]);
        return res;
    }

    friend String operator*(size_t times, const String& str) {
        return str * times;
    }

    // Сравнение
    friend bool operator==(const String& a, const String& b) {
        return a.size_ == b.size_ && std::equal(a.data_, a.data_ + a.size_, b.data_);
    }

    friend bool operator!=(const String& a, const String& b) {
        return !(a == b);
    }

    friend bool operator<(const String& a, const String& b) {
        return std::lexicographical_compare(a.data_, a.data_ + a.size_,
                                            b.data_, b.data_ + b.size_);
    }

    friend bool operator<=(const String& a, const String& b) {
        return !(b < a);
    }

    friend bool operator>(const String& a, const String& b) {
        return b < a;
    }

    friend bool operator>=(const String& a, const String& b) {
        return !(a < b);
    }

    // Трансформации (статические)
    template<typename Trans>
    void transform_inplace(const Trans& t) {
        for (size_t i = 0; i < size_; ++i)
            data_[i] = t(data_[i]);
        data_[size_] = T{};
    }

    template<typename Trans>
    String transform_copy(const Trans& t) const {
        String res(size_, T{});
        for (size_t i = 0; i < size_; ++i)
            res.data_[i] = t(data_[i]);
        return res;
    }

    // Трансформации (динамические)
    struct Transformer {
        virtual T operator()(const T&) const = 0;
        virtual ~Transformer() = default;
    };

    void apply(const Transformer& t) {
        for (size_t i = 0; i < size_; ++i)
            data_[i] = t(data_[i]);
        data_[size_] = T{};
    }
};

// I/O только для char
template<typename T>
std::ostream& operator<<(std::ostream& os, const String<T>& str) {
    for (size_t i = 0; i < str.size(); ++i)
        os << str[i];
    return os;
}

template<>
std::ostream& operator<<(std::ostream& os, const String<char>& str) {
    return os << str.begin();
}

std::istream& operator>>(std::istream& is, String<char>& str) {
    str.clear();
    char ch;
    while (is.get(ch)) {
        if (std::isspace(static_cast<unsigned char>(ch))) break;
        str.push_back(ch);
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const String<char>& str) {
    for (size_t i = 0; i < str.size(); ++i)
        os << str[i];
    return os;
}

