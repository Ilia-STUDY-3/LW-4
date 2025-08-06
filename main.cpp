#include <iostream>
#include "String.hpp"

int main() {
    using std::cout;
    using std::cin;
    using std::endl;

    // Конструкторы
    String<char> s1("HelloWorld");
    String<char> s2(5, '*');
    String<char> s3(s1);
    String<char> s4(std::move(s2));
    const char* abc = "ABCDE";
    String<char> s5(abc + 1, abc + 4); // "BCD"

    cout << "s1: " << s1 << endl;
    cout << "s3 (copy of s1): " << s3 << endl;
    cout << "s4 (moved from s2): " << s4 << endl;
    cout << "s5 (range ABCDE+1 to +4): " << s5 << endl;

    // substr
    String<char> sub = s1.substr(5, 5); // "World"
    cout << "substr(5,5): " << sub << endl;

    // Конкатенация
    String<char> a("Hello"), b(" C++");
    cout << "a + b: " << (a + b) << endl;
    cout << "a + '!': " << (a + '!') << endl;
    cout << "'>' + a: " << ('>' + a) << endl;

    // push_back
    String<char> p("Test");
    p.push_back('!');
    cout << "After push_back: " << p << endl;

    // Повторение строки
    String<char> r("ab");
    cout << "r * 3: " << (r * 3) << endl;
    cout << "2 * r: " << (2 * r) << endl;
    cout << "r * 0: size = " << (r * 0).size() << ", empty? " << ((r * 0).empty() ? "yes" : "no") << endl;

    // Сравнение
    String<char> x("apple"), y("banana");
    cout << "x == y: " << (x == y ? "true" : "false") << endl;
    cout << "x != y: " << (x != y ? "true" : "false") << endl;
    cout << "x < y: "  << (x < y ? "true" : "false") << endl;
    cout << "x <= y: " << (x <= y ? "true" : "false") << endl;
    cout << "x > y: "  << (x > y ? "true" : "false") << endl;
    cout << "x >= y: " << (x >= y ? "true" : "false") << endl;

    // Трансформация (статик)
    struct ToUpper {
        char operator()(char ch) const {
            return (ch >= 'a' && ch <= 'z') ? (ch - 'a' + 'A') : ch;
        }
    };

    String<char> word("hello");
    word.transform_inplace(ToUpper{});
    cout << "After transform_inplace (ToUpper): " << word << endl;

    String<char> original("hello");
    String<char> upper = original.transform_copy(ToUpper{});
    cout << "transform_copy (ToUpper): " << upper << ", original: " << original << endl;

    // I/O
    String<char> input;
    cout << "Enter a word: ";
    cin >> input;
    cout << "You entered: " << input << endl;

    return 0;
}
