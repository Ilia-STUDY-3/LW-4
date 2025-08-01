#include <iostream>
#include "String.hpp"

using std::cout;
using std::endl;

struct ToUpper : String<char>::Transformer<char> {
    char operator()(const char& ch) const override {
        if (ch >= 'a' && ch <= 'z') return ch - ('a' - 'A');
        return ch;
    }
};

struct Starify {
    char operator()(const char&) const {
        return '*';
    }
};

int main() {
    String<char> s1;
    cout << "s1 (default): size = " << s1.size() << ", empty? " << (s1.empty() ? "yes" : "no") << endl;

    String<char> s2("Hello");
    cout << "s2: ";
    for (size_t i = 0; i < s2.size(); ++i) cout << s2[i];
    cout << endl;

    String<char> s3(5, '*');
    cout << "s3: ";
    for (size_t i = 0; i < s3.size(); ++i) cout << s3[i];
    cout << endl;

    String<char> s4(s2);
    cout << "s4 (copy of s2): ";
    for (size_t i = 0; i < s4.size(); ++i) cout << s4[i];
    cout << endl;

    String<char> s5 = String<char>("Moved");
    cout << "s5 (moved): ";
    for (size_t i = 0; i < s5.size(); ++i) cout << s5[i];
    cout << endl;

    const char* str1 = "ABCDE";
    String<char> s6(str1 + 1, str1 + 4);
    cout << "s6 (range): ";
    for (size_t i = 0; i < s6.size(); ++i) cout << s6[i];
    cout << endl;

    String<char> s7;
    s7 = s3;
    cout << "s7 (copy assign from s3): ";
    for (size_t i = 0; i < s7.size(); ++i) cout << s7[i];
    cout << endl;

    String<char> s8;
    s8 = String<char>("MoveAssign");
    cout << "s8 (move assign): ";
    for (size_t i = 0; i < s8.size(); ++i) cout << s8[i];
    cout << endl;

    String<char> s9("substring");
    auto sub = s9.substr(3, 6);
    cout << "s9.substr(3, 6): ";
    for (size_t i = 0; i < sub.size(); ++i) cout << sub[i];
    cout << endl;

    s9.clear();
    cout << "s9 cleared: size = " << s9.size() << ", empty? " << (s9.empty() ? "yes" : "no") << endl;

    const char* abc = "ABCDE";
    String<char> s6b(abc + 1, abc + 4);
    cout << "s6b (from char*): ";
    for (size_t i = 0; i < s6b.size(); ++i) cout << s6b[i] << ' ';
    cout << endl;

    try {
        String<char> bad = s2.substr(100, 10);
    } catch (const std::out_of_range& e) {
        cout << "Caught out_of_range: " << e.what() << endl;
    }

    try {
        const char* str2 = "ABCD";
        String<char> badRange(str2 + 3, str2 + 1);
    } catch (const std::invalid_argument& e) {
        cout << "Caught invalid_argument: " << e.what() << endl;
    }

    // Конкатенации и повторение
    String<char> c1("Hi");
    String<char> c2(" there");
    String<char> joined = c1 + c2 + '!';
    cout << "Concatenation: ";
    for (size_t i = 0; i < joined.size(); ++i) cout << joined[i];
    cout << endl;

    String<char> repeated = c1 * 3;
    cout << "Repeated (Hi*3): ";
    for (size_t i = 0; i < repeated.size(); ++i) cout << repeated[i];
    cout << endl;

    // Сравнения
    String<char> a("abc"), b("abd");
    cout << "abc == abc? " << (a == String<char>("abc") ? "yes" : "no") << endl;
    cout << "abc < abd? " << (a < b ? "yes" : "no") << endl;

    // Трансформации
    String<char> t1("hello world");
    ToUpper upper;
    t1.apply(upper);
    cout << "ToUpper: ";
    for (size_t i = 0; i < t1.size(); ++i) cout << t1[i];
    cout << endl;

    Starify star;
    auto t2 = t1.transform(star);
    cout << "Starify: ";
    for (size_t i = 0; i < t2.size(); ++i) cout << t2[i];
    cout << endl;

    return 0;
}