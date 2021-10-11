#include <iostream>
#include <functional>
#include <fstream>
#include<sstrea>
#include<vector>

using namespace std;

using my_multiset = vector<double>;

using my_triplets = vector<vector<double>>;

bool test_for_triplets(my_multiset multiset){
    if(multiset.size()%3 != 0){
        cout << "Multiset can't be divided into triplets.";
        return false;
    }
    return true;
}

int test_for_target(my_multiset multiset){
    test_for_triplets(multiset);
    int number_of_triplets = multiset.size()/3;


    auto sum_of_numbers = [=]() -> int {
        int sum = 0;
        for(int i : multiset){
            sum += i;
        }
        return sum;
    };

    int target = sum_of_numbers / number_of_triplets;

    return target;
}


int main(int argc, char** argv) {
}
