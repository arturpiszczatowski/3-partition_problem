#include<iostream>
#include<functional>
#include<fstream>
#include<sstream>
#include<vector>

using namespace std;

using my_multiset = vector<int>;

using my_triplets = vector<vector<int>>;

int number_of_triplets = 0;

bool test_for_triplets(my_multiset multiset){
    if(multiset.size()%3 != 0){
        cout << "Multiset can't be divided into triplets.";
        return false;
    }
    return true;
}

int test_for_target(my_multiset multiset){
    test_for_triplets(multiset);
    number_of_triplets = multiset.size()/3;

    int sum_of_numbers = 0;
    for(int n : multiset){
        sum_of_numbers += n;
    }

//    auto sum_of_numbers = [=]() {
//        int sum = 0;
//        for(int n : multiset){
//            sum += n;
//        }
//        return sum;
//    };

    int target = sum_of_numbers / number_of_triplets;

    return target;
}

auto test_for_np_complete(my_multiset multiset){
    int target = test_for_target(multiset);
    for(int i : multiset){
        if(i>=target/4 && i<=target/2){
            return true;
        }
        else{
            throw invalid_argument("The 3-partition problem isn't strongly NP-complete");
        }
    }
}

my_triplets sort_into_triplets(my_multiset multiset){
    test_for_triplets(multiset);
    test_for_np_complete(multiset);
    int target = test_for_target(multiset);
    my_triplets result;

    for(int i=0; i <= number_of_triplets; i++){
        auto subset = [&](my_multiset multiset) {
            vector<int> subset;
            for(int n : multiset){

            }
            return subset;
        }
        result.push_back(subset);
    }
}

int main(int argc, char** argv) {
    cout << "hello";
}
