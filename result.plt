set terminal png
set output "result.png"
plot 'result_hill_climb_stochastic.txt' u 1:2 w lines, 'result_hill_climb.txt' u 1:2 w lines, 'result_tabu_search.txt' u 1:2 w lines, 
