#include <iostream>
#include <vector>

int main(){
    std::vector<int> nums = {2,7,11,15};

    std::cout << "First num: " << nums[0] << "\n";
    std::cout << "Second num: " << nums[1] << "\n";

    for(int i = 0; i < nums.size(); i++){
        std::cout << "Index" << i <<": " << nums[i] << "\n";
    }

    return 0;
}