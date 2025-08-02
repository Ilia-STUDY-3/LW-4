#include <iostream>
#include "String.hpp"

int main() {
    using std::cout;
    using std::endl;

    // substr
    String<char> s1("HelloWorld");
    String<char> sub = s1.substr(5, 5); // "World"
    cout << "substr: ";
    for (size_t i = 0; i < sub.size(); ++i)
        cout << sub[i];
    cout << endl;

    // + String
    String<char> a("Hello");
    String<char> b(" C++");
    String<char> c = a + b;
    cout << "Concatenation (string + string): ";
    for (size_t i = 0; i < c.size(); ++i)
        cout << c[i];
    cout << endl;

    // + символ
    String<char> d = a + '!';
    cout << "Concatenation (string + char): ";
    for (size_t i = 0; i < d.size(); ++i)
        cout << d[i];
    cout << endl;

    String<char> e = '>' + a;
    cout << "Concatenation (char + string): ";
    for (size_t i = 0; i < e.size(); ++i)
        cout << e[i];
    cout << endl;

    // push_back
    String<char> f("Test");
    f.push_back('!');
    cout << "After push_back: ";
    for (size_t i = 0; i < f.size(); ++i)
        cout << f[i];
    cout << endl;

    // умножение строки
    String<char> g("ab");
    String<char> repeated = g * 3;
    cout << "Repetition (string * 3): ";
    for (size_t i = 0; i < repeated.size(); ++i)
        cout << repeated[i];
    cout << endl;

    String<char> repeated2 = 2 * g;
    cout << "Repetition (2 * string): ";
    for (size_t i = 0; i < repeated2.size(); ++i)
        cout << repeated2[i];
    cout << endl;

    // Проверка повторения с нулём
    String<char> zeroRep = g * 0;
    cout << "Repetition (string * 0): size = " << zeroRep.size() << ", empty? " << (zeroRep.empty() ? "yes" : "no") << endl;



        String<char> x("apple");
    String<char> y("banana");

    cout << "x == y: " << (x == y ? "true" : "false") << endl;
    cout << "x != y: " << (x != y ? "true" : "false") << endl;
    cout << "x < y: "  << (x < y  ? "true" : "false") << endl;
    cout << "x <= y: " << (x <= y ? "true" : "false") << endl;
    cout << "x > y: "  << (x > y  ? "true" : "false") << endl;
    cout << "x >= y: " << (x >= y ? "true" : "false") << endl;


    return 0;
}
