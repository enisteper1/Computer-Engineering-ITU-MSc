- init:
    run: rm -f main

- build:
    run: g++ -std=c++11 -Wall -Werror main.cpp -o main  # timeout: 10

- clear_1:
    run: rm -f generated_outputs/case1/*.txt
    visible: false

- case_run1:
    run: ./main case_1
    exit: 0

- clear_2:
    run: rm -f generated_outputs/case2/*.txt
    visible: false

- case_run2:
    run: ./main case_2
    exit: 0

- clear_3:
    run: rm -f generated_outputs/case3/*.txt
    visible: false

- case_run3:
    run: ./main case_3
    exit: 0

- clear_4:
    run: rm -f generated_outputs/case4/*.txt
    visible: false

- case_run4:
    run: ./main case_4
    exit: 0