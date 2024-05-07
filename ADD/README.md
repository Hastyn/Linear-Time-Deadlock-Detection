Railway scheduling

To execute the simulator:
Run the following commands:
1. make
2. ./obj/simulate \<line\> \<algorithm\> \<policy_improvement>  \<optimize\> \<iterations\>

Parameters:
line: 'HYP-1', 'HYP-2', 'HYP-3', 'HYP-4', 'HYP-5', 'HYP-6', 'HYP-7', 'HYP-8', 'Ajmer', 'Kanpur', 'Konkan';
Default:'HYP-1' 

algorithm: 'Rg', 'R0', 'Rc' ;
Default: 'Rg' 

policy_improvement: Boolean value indicating if policy improvement is to be done -> 'true', 'false' ; Default: 'false' 

optimize: Boolean value indicating if optimization should be performed during policy improvement by limiting the number of steps policy improvement is performed to 50 and width of the search to 5 -> 'true', 'false' ; 
Default: 'false' 

iterations: integer -> number of perturbations to average over ; 
Default: 10 

Example command:
./obj/simulate HYP-2 R0 true 
This will run the simulator with R0 deadlock detection algorithm with policy improvement

The parameters are ordered, please provide in same order as given in command.

