get_add(){
    echo "Running simulations for $1"
    echo "Results for $1 algorithm" >> ../results_add.txt
    ./obj/simulate Ajmer $1 >> ../results_add.txt
    echo "Ajmer done"
    ./obj/simulate Kanpur $1 >> ../results_add.txt
    echo "Kanpur done"
    ./obj/simulate Konkan $1 >> ../results_add.txt
    echo "Konkan done"
    ./obj/simulate HYP-1 $1 >> ../results_add.txt
    echo "HYP-1 done"
    ./obj/simulate HYP-2 $1 >> ../results_add.txt
    echo "HYP-2 done"
    ./obj/simulate HYP-3 $1 >> ../results_add.txt
    echo "HYP-3 done"
    ./obj/simulate HYP-4 $1 >> ../results_add.txt
    echo "HYP-4 done"
    ./obj/simulate HYP-5 $1 >> ../results_add.txt
    echo "HYP-5 done"
    ./obj/simulate HYP-6 $1 >> ../results_add.txt
    echo "HYP-6 done"
    ./obj/simulate HYP-7 $1 >> ../results_add.txt
    echo "HYP-7 done"
    ./obj/simulate HYP-8 $1 >> ../results_add.txt
    echo "HYP-8 done"
}

echo "ADD values in minutes with their standard error averaged over 10 perturbed versions of the reference timetables." > results_add.txt
cd ADD
make
get_add "Rc"
get_add "Rg"
get_add "R0"
make clean
cd ..
echo "All done"

