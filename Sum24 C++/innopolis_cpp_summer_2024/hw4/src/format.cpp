#include <iostream>
#include <cstdarg>

void myFunction(const char* str, ...) {
    std::va_list args;
    va_start(args, str);

    while (str != nullptr) {
        std::cout << str << " ";

        str = va_arg(args, const char*);
    }

    va_end(args);
}

int main() {
    myFunction("{1}+{1} = {0}", 2, "one", nullptr);
    return 0;
}
