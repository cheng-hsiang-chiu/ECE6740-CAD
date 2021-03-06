[![Ubuntu](https://github.com/cheng-hsiang-chiu/ECE6740-CAD/workflows/Ubuntu/badge.svg)](https://github.com/cheng-hsiang-chiu/ECE6740-CAD/actions?query=workflow%3AUbuntu)


# BDDs re-ordering


## What is this project about?
This is a final project of ECE6740-CAD. The purpose of this project is to find a variable ordering that results in a minimal number of node sizes of the BDDs. Simulated annealing is implemented. Users could specify the desired paramenters(TEMPERATURE, FRONZEN_TEMPERATURE, DEGRADE, MAX_ITERATION_PER_TEMPERATURE).


## Repository structure
- 3rd-party : cudd package
- src : source files
- test : test examples
- bench : benchmarks
- bin : executable file


## Compile steps
```bash
make distclean
make
```


## Run
```bash
./bin/BDD /path/to/testcase.blif initial_temperature frozen_temperature iterations_per_temperature degrade
```
For example, 
```bash
./bin/BDD ./test/test-L14.blif 1000 1 10 0.9
```


## Experimental results
Please refer to the <a href="./slide.pdf">slide</a> for more information.
