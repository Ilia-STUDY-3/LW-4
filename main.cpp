#include <iostream>
#include "String.hpp"

struct UpperCaseTransformer : public String<char>::Transformer {
    char operator()(const char& ch) const override {
        return std::toupper(static_cast<unsigned char>(ch));
    }
};

struct ShiftTransformer : public String<char>::Transformer {
    char operator()(const char& ch) const override {
        return ch + 1;
    }
};

int main() {
    try {
        String<char> s1("hello");
        String<char> s2(" world");
        String<char> s3 = s1 + s2;
        std::cout << "Concat: " << s3 << '\n';

        String<char> s4 = s3 * 2;
        std::cout << "Repeat: " << s4 << '\n';

        std::cout << "Substring (0, 5): " << s4.substr(0, 5) << '\n';

        UpperCaseTransformer upper;
        String<char> s5 = s4.transform_copy(upper);
        std::cout << "Transform (copy to upper): " << s5 << '\n';

        ShiftTransformer shift;
        s5.apply(shift);
        std::cout << "Apply (shift): " << s5 << '\n';

        std::cout << "Enter a word: ";
        String<char> input;
        std::cin >> input;
        std::cout << "You entered: " << input << '\n';

        // Проверка на исключение
        try {
            String<char> invalid(nullptr); // должно выбросить исключение
        } catch (const StringException& e) {
            std::cout << "[Caught StringException]: " << e.what() << '\n';
        }

        // Проверка substr с выходом за предел
        try {
            auto bad = s4.substr(999, 10);
        } catch (const std::out_of_range& e) {
            std::cout << "[Caught out_of_range]: " << e.what() << '\n';
        }

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
    }

    return 0;
}
