get_pi(){
    echo "Running simulations for $1"
    echo "Results for $1 algorithm" >> ../results_pi.txt
    ./obj/simulate Ajmer $1 true >> ../results_pi.txt
    echo "Ajmer done"
    ./obj/simulate Kanpur $1 true >> ../results_pi.txt
    echo "Kanpur done"
    ./obj/simulate Konkan $1 true >> ../results_pi.txt
    echo "Konkan done"
    ./obj/simulate HYP-1 $1 true >> ../results_pi.txt
    echo "HYP-1 done"
    ./obj/simulate HYP-2 $1 true >> ../results_pi.txt
    echo "HYP-2 done"
    ./obj/simulate HYP-3 $1 true >> ../results_pi.txt
    echo "HYP-3 done"
    ./obj/simulate HYP-4 $1 true true >> ../results_pi.txt
    echo "HYP-4 done"
    ./obj/simulate HYP-5 $1 true true >> ../results_pi.txt
    echo "HYP-5 done"
    ./obj/simulate HYP-6 $1 true >> ../results_pi.txt
    echo "HYP-6 done"
    ./obj/simulate HYP-7 $1 true true >> ../results_pi.txt
    echo "HYP-7 done"
    ./obj/simulate HYP-8 $1 true true >> ../results_pi.txt
    echo "HYP-8 done"
}

echo "ADD values in minutes with their standard error averaged over 10 perturbed versions of the reference timetables." > results_pi.txt
cd ADD
make
get_pi "Rc"
get_pi "Rg"
get_pi "R0"
make clean
cd ..
echo "All done"

