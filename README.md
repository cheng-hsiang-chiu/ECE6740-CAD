[![Ubuntu](https://github.com/cheng-hsiang-chiu/ECE6740-CAD/workflows/Ubuntu/badge.svg)](https://github.com/cheng-hsiang-chiu/ECE6740-CAD/actions?query=workflow%3AUbuntu)


# BDDs re-ordering


## What is this project about?
This is a final project of ECE6740-CAD. The purpose of this project is to find a variable ordering that results in a minimal number of node sizes of the BDDs. Simulated annealing is implemented. Users could specify the desired paramenters(TEMPERATURE, FRONZEN_TEMPERATURE, DEGRADE, MAX_ITERATION_PER_TEMPERATURE) in src/main.c and recompile. 


## Repository structure
- 3rd-party : cudd package
- src : source files
- test : testing examples
- bench : benchmarks
- bin : executable file


## Compile steps
Tested with G++, Ubuntu 20.04

```bash
make distclean
make
```


## Run
```bash
./bin/BDD ./test/test-L14.blif

```

## Experimental results
Please refer to the <a href="./slide.pdf">slide</a> for more information.
