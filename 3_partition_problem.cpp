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

//my_triplets sort_into_triplets(my_multiset multiset){
//    test_for_triplets(multiset);
//    test_for_np_complete(multiset);
//    int target = test_for_target(multiset);
//
//    my_triplets result;
//
//    for(int i = 0; i <= number_of_triplets; i++){
//        vector<int> subset = [&](my_multiset multiset) -> vector<int> {
//            vector<int> current_triplet;
//            do{
//                //First number of a triplet
//                for(int j = 0; j <= multiset.size();){
//                    int a = multiset[j];
//                    current_triplet.push_back(a);
//                    multiset.erase(multiset.begin() + j);
//
//                    int l = 0;
//                    //Second number of a triplet
//                    for(int k = 0; k <= multiset.size();){
//                        int b = multiset[k];
//                        multiset.erase(multiset.begin() + k);
//
//                        //Third number of a triplet
//                        if(a+b+multiset[l]!=target){
//                            l++;
//
//                            //If no third number fits first two, change second number
//                            if(l>=multiset.size()){
//                                k++;
//                                l=0+k;
//                                multiset.insert(multiset.begin()+k-1, b);
//                            } else {
//                                multiset.insert(multiset.begin()+k, b);
//                            }
//
//                        } else {
//                            current_triplet.push_back(b);
//                            current_triplet.push_back(multiset[l]);
//                            multiset.erase(multiset.begin()+l);
//                            break;
//                        }
//                    }
//                }
//            }while(current_triplet.size() != 3);
//
//            return current_triplet;
//        };
//
//        result.push_back(subset);
//    }
//
//    return result;
//}

my_multiset acquier_numbers(string file_name){
    vector<int> result;
    ifstream txt_file;
    txt_file.open(file_name);
    int number;

    if(!txt_file){
        cout << "File not found.";
    }

    while(txt_file >> number){
        result.push_back(number);
    }

    txt_file.close();

    return result;
}

int main(int argc, char** argv) {

    string directory = "C:\\Users\\Lenovo\\Desktop\\MHA\\";
    my_multiset numbers = acquier_numbers(directory + argv[1]);

    cout << "Given numbers: [ ";
    for(int n : numbers) {
        cout << n << " ";
    }
    cout << "]";
}
