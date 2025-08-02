#ifndef STRING_HPP
#define STRING_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

template<typename T>
class String {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

public:
    String() : data_(nullptr), size_(0), capacity_(0) {}

    String(const String& other) : size_(other.size_), capacity_(other.size_) {
        if (size_ > 0) {
            data_ = new T[capacity_];
            for (std::size_t i = 0; i < size_; ++i)
                data_[i] = other.data_[i];
        } else {
            data_ = nullptr;
        }
    }

    String(String&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    String(std::size_t count, const T& ch)
        : size_(count), capacity_(count) {
        if (count > 0) {
            data_ = new T[count];
            for (std::size_t i = 0; i < count; ++i)
                data_[i] = ch;
        } else {
            data_ = nullptr;
        }
    }

    String(const T* cstr) : size_(0), capacity_(0), data_(nullptr) {
        if (cstr) {
            while (cstr[size_] != T{}) ++size_;
            if (size_ > 0) {
                capacity_ = size_;
                data_ = new T[capacity_];
                for (std::size_t i = 0; i < size_; ++i)
                    data_[i] = cstr[i];
            }
        }
    }

    String(const T* begin, const T* end) : size_(0), capacity_(0), data_(nullptr) {
        if (begin > end)
            throw std::invalid_argument("begin must not be greater than end");
        size_ = end - begin;
        if (size_ > 0) {
            capacity_ = size_;
            data_ = new T[capacity_];
            for (std::size_t i = 0; i < size_; ++i)
                data_[i] = begin[i];
        }
    }

    template <typename U>
    String(const String<U>& other)
        : size_(other.size()), capacity_(other.size()) {
        if (size_ > 0) {
            data_ = new T[capacity_];
            for (std::size_t i = 0; i < size_; ++i)
                data_[i] = static_cast<T>(other[i]);
        } else {
            data_ = nullptr;
        }
    }

    String& operator=(const String& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.size_;
            if (size_ > 0) {
                data_ = new T[capacity_];
                for (std::size_t i = 0; i < size_; ++i)
                    data_[i] = other.data_[i];
            } else {
                data_ = nullptr;
            }
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

    ~String() {
        delete[] data_;
    }

    std::size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    void clear() {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    T& operator[](std::size_t index) {
        return data_[index];
    }

    const T& operator[](std::size_t index) const {
        return data_[index];
    }

    String substr(std::size_t pos, std::size_t len) const {
        if (pos > size_) {
            throw std::out_of_range("substr: position out of range");
        }
        std::size_t actual_len = (pos + len > size_) ? (size_ - pos) : len;
        return String(data_ + pos, data_ + pos + actual_len);
    }

    void push_back(const T& ch) {
        if (size_ + 1 > capacity_) {
            std::size_t new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
            T* new_data = new T[new_cap];
            for (std::size_t i = 0; i < size_; ++i)
                new_data[i] = data_[i];
            delete[] data_;
            data_ = new_data;
            capacity_ = new_cap;
        }
        data_[size_++] = ch;
    }

    String operator+(const String& other) const {
        String result(size_ + other.size_, T{});
        for (std::size_t i = 0; i < size_; ++i)
            result[i] = data_[i];
        for (std::size_t i = 0; i < other.size_; ++i)
            result[i + size_] = other.data_[i];
        return result;
    }

    String operator+(const T& ch) const {
        String result(size_ + 1, T{});
        for (std::size_t i = 0; i < size_; ++i)
            result[i] = data_[i];
        result[size_] = ch;
        return result;
    }

    friend String operator+(const T& ch, const String& str) {
        String result(str.size_ + 1, T{});
        result[0] = ch;
        for (std::size_t i = 0; i < str.size_; ++i)
            result[i + 1] = str.data_[i];
        return result;
    }

    String operator*(int count) const {
        if (count <= 0) return String();
        String result(size_ * count, T{});
        for (int i = 0; i < count; ++i)
            for (std::size_t j = 0; j < size_; ++j)
                result[i * size_ + j] = data_[j];
        return result;
    }

    friend String operator*(int count, const String& str) {
        return str * count;
    }

    // == : равны по размеру и по символам
bool operator==(const String& other) const {
    if (size_ != other.size_) return false;
    for (std::size_t i = 0; i < size_; ++i) {
        if (data_[i] != other.data_[i])
            return false;
    }
    return true;
}

// != : через == 
bool operator!=(const String& other) const {
    return !(*this == other);
}

// < : лексикографическое сравнение
bool operator<(const String& other) const {
    std::size_t min_size = (size_ < other.size_) ? size_ : other.size_;
    for (std::size_t i = 0; i < min_size; ++i) {
        if (data_[i] < other.data_[i]) return true;
        if (data_[i] > other.data_[i]) return false;
    }
    return size_ < other.size_;
}

// <= : меньше или равно
bool operator<=(const String& other) const {
    return *this < other || *this == other;
}

// > : строго больше
bool operator>(const String& other) const {
    return !(*this <= other);
}

// >= : больше или равно
bool operator>=(const String& other) const {
    return !(*this < other);
}
};

#endif // STRING_HPP
