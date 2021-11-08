#include<iostream>
#include<functional>
#include<fstream>
#include<sstream>
#include<vector>
#include<random>
#include<algorithm>
#include<numeric>

using namespace std;

using my_multiset = vector<int>;

using my_triplets = vector<vector<int>>;

int number_of_triplets = 0;

random_device rd;
mt19937 rand_gen(rd());

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

    int target = sum_of_numbers / number_of_triplets;
    cout << "Your target is: " << target << "\n";

    return target;
}

//auto test_for_np_complete(my_multiset multiset) {
//    int target = test_for_target(multiset);
//    for (int i : multiset) {
//        if (i >= target / 4 && i <= target / 2) {
//            return true;
//        } else {
//            cout << "\nThe 3-partition problem isn't strongly NP-complete";
//            return false;
//        }
//    }
//}

my_triplets sort_into_triplets(my_multiset multiset){
    test_for_triplets(multiset);
//    test_for_np_complete(multiset);
    int target = test_for_target(multiset);

    my_triplets result;

    for(int i = 0; i <= number_of_triplets; i++){
        auto subset = [&](my_multiset multiset) -> vector<int> {
            vector<int> current_triplet;
            do{
                //First number of a triplet
                for(int j = 0; j <= multiset.size();){
                    int a = multiset[j];
                    current_triplet.push_back(a);
                    multiset.erase(multiset.begin() + j);

                    int l = 0;
                    //Second number of a triplet
                    for(int k = 0; k <= multiset.size();){
                        int b = multiset[k];
                        multiset.erase(multiset.begin() + k);

                        //Third number of a triplet
                        if(a+b+multiset[l]!=target){
                            l++;

                            //If no third number fits first two, change second number
                            if(l>=multiset.size()){
                                multiset.insert(multiset.begin()+k, b);
                                k++;
                                l=0+k;
                            } else {
                                multiset.insert(multiset.begin()+k, b);
                            }

                        } else {
                            current_triplet.push_back(b);
                            current_triplet.push_back(multiset[l]);
                            multiset.erase(multiset.begin()+l);
                            cout << " [";
                            for(int o : current_triplet) {
                                cout << " " << o << " ";
                            }
                            cout << "]";
                            break;
                        }
                    }
                }
            }while(current_triplet.size() != 3);

            return current_triplet;
        };

        result.push_back(subset(multiset));
    }

    return result;
}

void show_my_triplets(my_triplets triplets){
    cout << "{";
    for(auto a : triplets){
        cout << "[";
        for(int b : a){
            cout << " " << b << " ";
        }
        cout << "]";
    }
    cout << "}";
}
void show_my_multiset(my_multiset  multiset){
    cout << "Given numbers: [ ";
    for(int n : multiset) {
        cout << n << " ";
    }
    cout << "]\n";
}


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

my_triplets random_seperate_into_triplets(my_multiset multiset){
    my_triplets random_result;
    do {
        vector<int> trippy;
        for (int i = 0; i < 3; i++) {
            uniform_int_distribution<int> random(0, multiset.size()-1);
            int index = random(rand_gen);
            trippy.push_back(multiset[index]);
            multiset.erase(multiset.begin() + index);
        }
        random_result.push_back(trippy);
    }while(!multiset.empty());

    return random_result;
}

double goal_function(my_triplets triplets){
    double final_score = triplets.size();
    int final_target;
    vector<int> targets;

    for(auto a : triplets){
        int sum = 0;
        for(int b : a){
            sum += b;
            targets.push_back(sum);
        }
    }

    for(auto target : targets){
        double current_score = triplets.size();
        for(auto a : triplets){
            if(accumulate(a.begin(), a.end(), 0) == target){
                current_score -= 1;
            }
        }
        if(current_score < final_score){
            final_score = current_score;
            final_target = target;
        }
    }

    cout << "->Current target: " << final_target << " ->Score: " << final_score <<"\n";
    return final_score;
}

my_triplets next_solution(my_triplets triplets){
    int i = 1;
    for(auto &triplet : triplets) {
        sort(triplet.begin(), triplet.end());
        do {
            cout << i << ") ";
            show_my_triplets(triplets);
            goal_function(triplets);
            i++;
        } while (next_permutation(triplet.begin(), triplet.end()));
    }
    return triplets;
}


int main(int argc, char** argv) {

    string directory = "C:\\Users\\Lenovo\\Desktop\\MHE\\"; //"Z:\\public_html\\ROK 3\\Metaheurystyka\\Lab 2\\";
    my_multiset numbers = acquier_numbers(directory + argv[1]);

    show_my_multiset(numbers);
    int target = test_for_target(numbers);
    next_solution(random_seperate_into_triplets(numbers));
//    show_my_triplets(random_seperate_into_triplets(numbers));
//    goal_function(random_seperate_into_triplets(numbers));

}
