#include "String.hpp"
#include <iostream>

int main() {
    try {
        // Тест 1: Конструктор по умолчанию
        String<char> empty;
        std::cout << "Empty string size: " << empty.size() << ", empty: " 
                  << (empty.empty() ? "yes" : "no") << "\n";

        // Тест 2: Копирующий конструктор
        String<char> original(5, 'A'); // "AAAAA"
        String<char> copy = original;
        std::cout << "Original size: " << original.size() << ", empty: " 
                  << (original.empty() ? "yes" : "no") << "\n";
        std::cout << "Copy size: " << copy.size() << ", empty: " 
                  << (copy.empty() ? "yes" : "no") << "\n";

        // Тест 3: Перемещающий конструктор
        String<char> temp(3, 'B'); // "BBB"
        String<char> moved = std::move(temp);
        std::cout << "Moved size: " << moved.size() << ", empty: " 
                  << (moved.empty() ? "yes" : "no") << "\n";
        std::cout << "Temp after move size: " << temp.size() << ", empty: " 
                  << (temp.empty() ? "yes" : "no") << "\n";

        // Тест 4: Конструктор с повторением символа
        String<char> stars(10, '*');
        std::cout << "Stars size: " << stars.size() << ", empty: " 
                  << (stars.empty() ? "yes" : "no") << "\n";

        // Тест 5: Конструктор из C-строки
        const char* cstr = "Hello";
        String<char> from_cstr(cstr);
        std::cout << "C-string size: " << from_cstr.size() << ", empty: " 
                  << (from_cstr.empty() ? "yes" : "no") << "\n";

        // Тест 6: Конструктор из пустой C-строки
        const char* empty_cstr = "";
        String<char> from_empty_cstr(empty_cstr);
        std::cout << "Empty C-string size: " << from_empty_cstr.size() << ", empty: " 
                  << (from_empty_cstr.empty() ? "yes" : "no") << "\n";

        // Тест 7: Конструктор из диапазона указателей
        const char arr[] = {'H', 'e', 'l', 'l', 'o'};
        String<char> from_range(arr, arr + 5);
        std::cout << "Range size: " << from_range.size() << ", empty: " 
                  << (from_range.empty() ? "yes" : "no") << "\n";

        // Тест 8: Копирующее присваивание
        String<char> source("World");
        String<char> dest;
        dest = source;
        std::cout << "Dest size after copy: " << dest.size() << ", empty: " 
                  << (dest.empty() ? "yes" : "no") << "\n";
        std::cout << "Source size after copy: " << source.size() << ", empty: " 
                  << (source.empty() ? "yes" : "no") << "\n";

        // Тест 9: Копирующее присваивание пустой строки
        String<char> empty_source;
        String<char> dest2("Test");
        dest2 = empty_source;
        std::cout << "Dest2 size after empty copy: " << dest2.size() << ", empty: " 
                  << (dest2.empty() ? "yes" : "no") << "\n";

        // Тест 10: Самоприсваивание
        String<char> self("Self");
        self = self;
        std::cout << "Self size after self-assign: " << self.size() << ", empty: " 
                  << (self.empty() ? "yes" : "no") << "\n";

        // Тест 11: Проверка исключения для некорректного диапазона
        String<char> invalid_range(arr + 5, arr);
        std::cout << "This line should not be reached\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught invalid_argument: " << e.what() << "\n";
    }

    return 0;
}