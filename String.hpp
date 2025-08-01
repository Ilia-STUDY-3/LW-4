#ifndef STRING_HPP
#define STRING_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <iostream>

// === Класс String ===
template<typename T>
class String {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    void copy_from(const T* src, std::size_t n) {
        data_ = new T[n];
        for (std::size_t i = 0; i < n; ++i) data_[i] = src[i];
        size_ = capacity_ = n;
    }

public:
    String() : data_(nullptr), size_(0), capacity_(0) {}

    String(const String& other) { copy_from(other.data_, other.size_); }

    String(String&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr; other.size_ = other.capacity_ = 0;
    }

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
            data_ = other.data_; size_ = other.size_; capacity_ = other.capacity_;
            other.data_ = nullptr; other.size_ = other.capacity_ = 0;
        }
        return *this;
    }

    String(std::size_t count, const T& ch) {
        data_ = new T[count];
        for (std::size_t i = 0; i < count; ++i) data_[i] = ch;
        size_ = capacity_ = count;
    }

    String(const T* cstr) : size_(0), capacity_(0), data_(nullptr) {
        while (cstr && cstr[size_] != T{}) ++size_;
        if (size_ > 0) copy_from(cstr, size_);
    }

    String(const T* begin, const T* end) {
        if (begin > end) throw std::invalid_argument("begin > end");
        std::size_t n = end - begin;
        copy_from(begin, n);
    }

    template<typename U>
    explicit String(const String<U>& other) {
        data_ = new T[other.size()];
        for (std::size_t i = 0; i < other.size(); ++i)
            data_[i] = static_cast<T>(other[i]);
        size_ = capacity_ = other.size();
    }

    ~String() { delete[] data_; }

    std::size_t size() const { return size_; }

    bool empty() const { return size_ == 0; }

    void clear() {
        delete[] data_;
        data_ = nullptr; size_ = capacity_ = 0;
    }

    T& operator[](std::size_t index) {
        if (index >= size_) throw std::out_of_range("[] out of range");
        return data_[index];
    }

    const T& operator[](std::size_t index) const {
        if (index >= size_) throw std::out_of_range("[] out of range");
        return data_[index];
    }

    String substr(std::size_t pos, std::size_t len) const {
        if (pos > size_) throw std::out_of_range("substr: position out of range");
        if (pos + len > size_) len = size_ - pos;
        return String(data_ + pos, data_ + pos + len);
    }

    String operator+(const String& other) const {
        String result;
        result.size_ = size_ + other.size_;
        result.capacity_ = result.size_;
        result.data_ = new T[result.size_];
        for (std::size_t i = 0; i < size_; ++i) result.data_[i] = data_[i];
        for (std::size_t i = 0; i < other.size_; ++i) result.data_[size_ + i] = other.data_[i];
        return result;
    }

    String operator+(const T& ch) const {
        String result;
        result.size_ = size_ + 1;
        result.capacity_ = result.size_;
        result.data_ = new T[result.size_];
        for (std::size_t i = 0; i < size_; ++i) result.data_[i] = data_[i];
        result.data_[size_] = ch;
        return result;
    }

    friend String operator+(const T& ch, const String& str) {
        String result;
        result.size_ = str.size_ + 1;
        result.capacity_ = result.size_;
        result.data_ = new T[result.size_];
        result.data_[0] = ch;
        for (std::size_t i = 0; i < str.size_; ++i) result.data_[i + 1] = str.data_[i];
        return result;
    }

    String operator*(int count) const {
        if (count < 0) throw std::invalid_argument("negative repeat count");
        String result;
        result.size_ = size_ * count;
        result.capacity_ = result.size_;
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

    bool operator>(const String& other) const { return other < *this; }
    bool operator<=(const String& other) const { return !(other < *this); }
    bool operator>=(const String& other) const { return !(*this < other); }

    // === Этап 7 ===
    template <typename U>
    struct Transformer {
        virtual U operator()(const U& ch) const = 0;
        virtual ~Transformer() = default;
    };

    void apply(const Transformer<T>& transformer) {
        for (std::size_t i = 0; i < size_; ++i)
            data_[i] = transformer(data_[i]);
    }

    template <typename Trans>
    void modify(const Trans& transformer) {
        for (std::size_t i = 0; i < size_; ++i)
            data_[i] = transformer(data_[i]);
    }

    template <typename Trans>
    String transform(const Trans& transformer) const {
        String result(*this);
        result.modify(transformer);
        return result;
    }

    const T* data() const { return data_; }
};

#endif // STRING_HPP
