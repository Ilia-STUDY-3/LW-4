#ifndef STRING_HPP
#define STRING_HPP

#include <cstddef>  // size_t
#include <stdexcept> // std::out_of_range
#include <utility>  // std::move

template<typename T>
class String {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    void allocate(std::size_t cap) {
        data_ = (cap > 0) ? new T[cap] : nullptr;
        capacity_ = cap;
    }

public:
    // Конструктор по умолчанию — пустая строка
    String()
        : data_(nullptr), size_(0), capacity_(0) {}

    // Копирующий конструктор
    String(const String& other)
        : size_(other.size_), capacity_(other.size_) {
        if (size_ > 0) {
            data_ = new T[capacity_];
            for (std::size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        } else {
            data_ = nullptr;
        }
    }

    // Перемещающий конструктор
    String(String&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    // Конструктор: повторы символа
    String(std::size_t count, const T& ch)
        : size_(count), capacity_(count) {
        if (count > 0) {
            data_ = new T[count];
            for (std::size_t i = 0; i < count; ++i) {
                data_[i] = ch;
            }
        } else {
            data_ = nullptr;
        }
    }

    // Конструктор из C-строки
    String(const T* cstr) : size_(0), capacity_(0), data_(nullptr) {
        if (cstr) {
            // Подсчитываем длину до T{}
            while (cstr[size_] != T{}) {
                ++size_;
            }
            if (size_ > 0) {
                capacity_ = size_;
                data_ = new T[capacity_];
                for (std::size_t i = 0; i < size_; ++i) {
                    data_[i] = cstr[i];
                }
            }
        }
    }

    // Конструктор из диапазона указателей
    String(const T* begin, const T* end) : size_(0), capacity_(0), data_(nullptr) {
        if (begin > end) {
            throw std::invalid_argument("begin must not be greater than end");
        }
        size_ = end - begin;
        if (size_ > 0) {
            capacity_ = size_;
            data_ = new T[capacity_];
            for (std::size_t i = 0; i < size_; ++i) {
                data_[i] = begin[i];
            }
        }
    }

    // Копирующее присваивание
    String& operator=(const String& other) {
        if (this != &other) { // Проверка на самоприсваивание
            // Освобождаем текущую память
            delete[] data_;
            // Копируем размеры
            size_ = other.size_;
            capacity_ = other.size_;
            // Выделяем новую память и копируем данные
            if (size_ > 0) {
                data_ = new T[capacity_];
                for (std::size_t i = 0; i < size_; ++i) {
                    data_[i] = other.data_[i];
                }
            } else {
                data_ = nullptr;
            }
        }
        return *this;
    }

    // Деструктор
    ~String() {
        delete[] data_;
    }

    // Размер строки
    std::size_t size() const {
        return size_;
    }

    // Проверка на пустоту
    bool empty() const {
        return size_ == 0;
    }
};

#endif // STRING_HPP