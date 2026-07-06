#include <iostream>

int add(int a , int b){
    return a + b;
}

int multiply(int a , int b){
    return a * b;
}

int main() {
    int sum = add(3,4);
    int product = multiply(5,6);

    std::cout << "3 + 4 = " << sum << std::endl;
    std::cout << "5 * 6 = " << product << std::endl;

    return 0;
}