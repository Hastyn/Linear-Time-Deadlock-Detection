This code base accompanies the paper "Linear-Time Optimal Deadlock Detection for Efficient Scheduling in Multi-Track Railway Networks" by Hastyn Doshi , Ayush Tripathi , Keshav Agarwal , Harshad Khadilkar , and Shivaram Kalyanakrishnan, which is accepted to IJCAI 2024.

The following scripts can be used to generate the results presented in the paper:

1. ADD.sh 
This will generate the ADD values for Rg, Rc and R0 algorithms corresponding to Table 2 in the paper
The results are stored in a file results_add.txt

2. policy_improvement.sh
This will generate the ADD values with policy improvement corresponding to Table 3 in the paper 
The results are stored in a file results_pi.txt

Data
The data folder consists of all the schedules used as data with 10 perturbations for each.

ADD
The ADD folder contains the source code. Please read the README.md in the folder for further instructions on how to make and run. 