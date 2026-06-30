#include <iostream>
#include <vector>

int add (int a, int b) {
    return a + b;
}

int sumVector(const std::vector<int>& nums) {
    int sum = 0;
    for (int value : nums) {
        sum += value;
    }
    return sum;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    int result = add(3, 4);
    std::cout << "3 + 4 = " << result << std::endl;
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::cout << "Vector sum: " << sumVector(numbers) << std::endl;

    return 0;
}