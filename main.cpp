#include <iostream>
#include <cstring>

void vulnerableFunction() {
    char buffer[8];
    int adjacentVar = 1234;

    std::cout << "Before overflow: adjacentVar = " << adjacentVar << std::endl;

    std::strcpy(buffer, "This string is too long!");

    std::cout << "After overflow: adjacentVar = " << adjacentVar << std::endl;
}

int main() {
    vulnerableFunction();
    return 0;
}
