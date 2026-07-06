#include <iostream>

int main() {
    int score = 85;

    if (score>= 90) {
        std::cout << "Grade A" << std::endl;
     } else if (score>= 80){
        std::cout << "Grade B" << std::endl;
     } else if (score>= 70){
        std::cout << "Grade C" << std::endl;
     } else {
        std::cout<< "Grade needs improving" << std::endl;
     }
     return 0;
}