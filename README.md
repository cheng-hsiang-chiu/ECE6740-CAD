# BDDs re-ordering


## What is this project about?
This is a final project of ECE6740-CAD. The purpose of this project is to find a variable ordering that results in a minimal number of node sizes of the BDDs.  




## Compile steps
Step 1)
Make sure the cudd_location file containes one single line with the full path to the cudd package 2.5.0

Step 2)
Type make distclean

Step 3) 
Type make


## Run
- Simulated annealing is implemented. Users could specify the desired paramenters in blif2bdd/main.c and recompile.
```bash

./BDDs >>> Including packages built on CUDD 2.5; converting BLIF (AIG) into BDDs w.r.t to input variable order defined in BLIF

```

## Experimental results
Please refer to the <a href="https://github.com/cheng-hsiang-chiu/ECE6740-CAD/slide.pdf">slide</a> for more information.
