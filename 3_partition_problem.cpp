#include<iostream>
#include<functional>
#include<fstream>
#include<sstream>
#include<vector>
#include<random>
#include<algorithm>
#include<numeric>
#include<chrono>
#include<set>
#include<list>
#include<map>


using namespace std;
using my_multiset = vector<int>;
using my_triplets = vector<vector<int>>;

//random_device rd;
//mt19937 rand_gen(rd());
std::mt19937 rand_gen(time(nullptr));

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

    int sum_of_numbers = 0;
    for(int n : multiset){
        sum_of_numbers += n;
    }

    int target = sum_of_numbers / number_of_triplets;
    cout << "Your target is: " << target << "\n";

    return target;
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

my_triplets next_solution(my_multiset multiset){
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
    double full_score = 0;
    vector<int> targets;

    for(auto triplet : triplets){
        int triplet_target = accumulate(triplet.begin(), triplet.end(), 0);
        full_score += triplet_target;
        targets.push_back(triplet_target);
    }

    int final_target = 0;
    double final_score = full_score;

    for(auto target : targets){
        double current_score = full_score;
        for(auto triplet : triplets){
            if(accumulate(triplet.begin(), triplet.end(), 0) == target){
                current_score -= target;
            }
        }
        if(current_score < final_score){
            final_score = current_score;
            final_target = target;
        }
    }

//    cout << "->Current target: " << final_target << " ->Score: " << final_score <<"\n";
    return final_score;
}

vector<my_triplets> all_permutations(my_triplets triplets){
    vector<my_triplets> all_permutations;
    for(auto &triplet : triplets) {
        sort(triplet.begin(), triplet.end());
        do {
            all_permutations.push_back(triplets);
        } while (next_permutation(triplet.begin(), triplet.end()));
    }
    return all_permutations;
}

my_triplets brute_force(vector<int> numbers,
                        function<void(int c, double dt)> on_statistics = [](int c, double dt){},
                        function<void(int i, double current_goal_val, double goal_val)>
                        on_iteration = [](int i, double current_goal_val, double goal_val) {}){
    auto start = chrono::steady_clock::now();

    vector<my_triplets> checked_solutions;
    my_triplets best_solution;
    double score_check;
    double best_score = accumulate(numbers.begin(), numbers.end(), 0);
    int stat_goal_function_calls = 0;

    int iteration_limit = 1;
    for(int i=numbers.size(); i>=1; i--){
        iteration_limit *= i;
    }

    int iteration = 1;

    do {
        my_triplets current_triplets = next_solution(numbers);
        if(!(find(checked_solutions.begin(), checked_solutions.end(), current_triplets) != checked_solutions.end())) {
            cout << stat_goal_function_calls+1 << ") ";
            iteration++;
            stat_goal_function_calls++;
            show_my_triplets(current_triplets);
            score_check = goal_function(current_triplets);
            cout << " =>Score: " << score_check << "\n";
            checked_solutions.push_back(current_triplets);

            if(score_check < best_score){
                best_score = score_check;
                best_solution = current_triplets;
            }

            for(auto permutation : all_permutations(current_triplets)){
                iteration++;
                checked_solutions.push_back(permutation);
            }

            on_iteration(stat_goal_function_calls, score_check, best_score);
        }
    } while(score_check != 0 && iteration < iteration_limit);
    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;
    on_statistics(stat_goal_function_calls, duration.count());

    return best_solution;
}

my_triplets generate_random_neighbour(my_triplets current_triplets){
    uniform_int_distribution<int> distr_set(0, current_triplets.size()-2);
    uniform_int_distribution<int> distr_triplet(0, 2);
    int index_set = distr_set(rand_gen);
    int index_triplet_a = distr_triplet(rand_gen);
    int index_triplet_b = distr_triplet(rand_gen);
    swap(current_triplets[index_set][index_triplet_a], current_triplets[index_set+1][index_triplet_b]);
    return current_triplets;
}

vector<my_triplets> generate_all_neighbours(my_triplets current_triplets){
    vector<my_triplets> all_neighbours;
    int number_of_triplets = current_triplets.size();
    int number_of_swaps = (number_of_triplets-1)*3;

    for(int i = 0; i < number_of_triplets; i++){
        int second_triplet_index = 0;
            for(int j = 0; j < 3; j++){
                for(int k = 0; k < number_of_swaps; k++){
                    int cop = k;
                    int second_element_index = cop%3;
                    my_triplets buffer = current_triplets;
                    if(second_triplet_index != i) {
                        swap(buffer[i][j], buffer[second_triplet_index][second_element_index]);
                        if (!(find(all_neighbours.begin(), all_neighbours.end(), buffer) != all_neighbours.end())) {
                            all_neighbours.push_back(buffer);
                        }
                    }
                    else{
                        k--;
                        second_triplet_index++;
                    }
                }
            }
        }
    return all_neighbours;
}

my_triplets hill_climb_stochastic(vector<int> numbers, int N,
                                  function<void(int c, double dt)> on_statistics = [](int c, double dt) {},
                                  function<void(int i, double current_goal_val, double goal_val)>
                                  on_iteration = [](int i, double current_goal_val, double goal_val) {}){

    auto start = chrono::steady_clock::now();

    my_triplets best_solution = next_solution(numbers);
    double best_score = accumulate(numbers.begin(), numbers.end(), 0);

    double score_check;

    my_triplets current_triplets = best_solution;


    for(int i=0; i < N; i++){
        current_triplets = generate_random_neighbour(best_solution);
        cout << i+1 << ") ";
        show_my_triplets(current_triplets);
        score_check = goal_function(current_triplets);
        cout << " =>Score: " << score_check << "\n";

        if(score_check < best_score){
            best_solution = current_triplets;
            best_score = score_check;
        }

        on_iteration(i, score_check, best_score);
    }

    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;
    on_statistics(N, duration.count());

    return best_solution;
}

my_triplets hill_climb(vector<int> numbers, int N,
                       function<void(int c, double dt)> on_statistics = [](int c, double dt) {},
                       function<void(int i, double current_goal_val, double goal_val)>
                       on_iteration = [](int i, double current_goal_val, double goal_val) {}){

    auto start = chrono::steady_clock::now();

    my_triplets best_solution = next_solution(numbers);
    double best_score = goal_function(best_solution);

    double score_check;

    for(int i=0; i < N; i++){
        auto all_neighbours = generate_all_neighbours(best_solution);
        for(auto neighbour : all_neighbours){
            score_check = goal_function(neighbour);
            if(score_check<best_score){
                best_solution = neighbour;
                best_score = score_check;
            }
        }
        cout << i+1 << ") ";
        show_my_triplets(best_solution);
        cout << " =>Score: " << best_score <<"\n";

        on_iteration(i, score_check, best_score);
    }

    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;
    on_statistics(N, duration.count());

    return best_solution;
}

my_triplets tabu_search(vector<int> numbers, int N, int tabu_size,
                        function<void(int c, double dt)> on_statistics = [](int c, double dt) {},
                        function<void(int i, double current_goal_val, double goal_val)>
                        on_iteration = [](int i, double current_goal_val, double goal_val) {}){

    auto start = chrono::steady_clock::now();

    my_triplets best_solution = next_solution(numbers);
    double best_score = goal_function(best_solution);

    set<vector<vector<int>>> tabu_set;
    list<vector<vector<int>>> tabu_list;

    double score_check;

    auto is_in_tabu = [&](auto e) {
        return tabu_set.count(e);
    };

    auto add_to_tabu = [&](auto e) {
        tabu_set.insert(e);
        tabu_list.push_back(e);
    };

    auto shrink_taboo = [&]() {
        if (tabu_set.size() > tabu_size) {
            tabu_set.erase(tabu_list.front());
            tabu_list.pop_front();
        }
    };

    auto current_triplets = best_solution;

    for (int i = 0; i < N; i++) {

        auto current_neighbours = generate_all_neighbours(best_solution);

        current_neighbours.erase(remove_if(current_neighbours.begin(), current_neighbours.end(), [&](auto e) { return is_in_tabu(e); }), current_neighbours.end());

        if (current_neighbours.size() == 0) break;

        current_triplets = *min_element(current_neighbours.begin(), current_neighbours.end(), [&](auto a, auto b) {
            return goal_function(a) > goal_function(b);
        });

        add_to_tabu(current_triplets);

        score_check = goal_function(current_triplets);

        if (goal_function(current_triplets) < goal_function(best_solution)) {
            best_solution = current_triplets;
            best_score = score_check;
        }

        cout << i+1 << ") ";
        show_my_triplets(current_triplets);
        cout << " =>Score: " << score_check <<"\n";

        on_iteration(i, score_check, best_score);

        shrink_taboo();
    }

    auto finish = chrono::steady_clock::now();
    chrono::duration<double> duration = finish - start;

    on_statistics(N, duration.count());

    return best_solution;
    }


map<string, string> args_to_map(vector<string> arguments){
    map<string, string> ret;
    string argname ="";
    for(auto param : arguments){
        if((param.size() > 2) && (param.substr(0, 2) == "--")){
            argname = param.substr(2);
        } else {
            ret[argname] = param;
        }
    }
    return ret;
}

vector<int> generate_random_problem(int number_of_triplets){
    vector<int> problem_set;

    uniform_int_distribution<int> random(1,30);

    for(int i=0; i<number_of_triplets*3;i++){
        problem_set.push_back(random(rand_gen));
    }

    int sum = accumulate(problem_set.begin(), problem_set.end(), 0);
    int control = sum%number_of_triplets;

    while(control!=0){
        problem_set[random(rand_gen)] -= 1;
        sum = accumulate(problem_set.begin(), problem_set.end(), 0);
        control = sum%number_of_triplets;
    }



    return problem_set;
}

int main(int argc, char** argv) {

    map<string, string> parameters = {
            {"size", "3"},
            {"iterations", "1000"},
            {"method", "brute_force"},
            {"print_result", "false"},
            {"tabu_size", "50"}
    };

    for(auto [k, v] : args_to_map(vector<string>(argv, argv + argc))){
        parameters[k] = v;
    }

    my_multiset problem = generate_random_problem(stoi(parameters["size"]));

    show_my_multiset(problem);
    cout << endl;

    int iterations = stoi(parameters["iterations"]);

    my_triplets best_solution;

    auto on_finish =
            [](int c, double dt) {
                cout << "# count: " << c << "; dt:  " << dt << endl;
            };

    auto on_step = [&](int i, double current_goal_val, double goal_v) {
        cout << i << " " << current_goal_val << " " << goal_v << endl;
    };


    if (parameters["method"] == "brute_force") {
        best_solution = brute_force(
                problem,
                on_finish,
                on_step);

    } else if (parameters["method"] == "hill_climb_stochastic") {
        best_solution = hill_climb_stochastic(
                problem,
                iterations,
                on_finish,
                on_step);

    } else if (parameters["method"] == "hill_climb") {
        best_solution = hill_climb(
                problem,
                iterations,
                on_finish,
                on_step);

    } else if (parameters["method"] == "tabu_search") {
        best_solution = tabu_search(
                problem,
                iterations,
                stoi(parameters["tabu_size"]),
                on_finish,
                on_step);
    }

    if (parameters["print_result"] == "true") {
        cout << "# best " << parameters["method"] << " solution:\n";
        show_my_multiset(problem);
        cout << " => ";
        show_my_triplets(best_solution);
        cout << endl;
    }
    cout << "# best " << parameters["method"] << " result: " <<
         goal_function(best_solution) << endl;

    return 0;
}
