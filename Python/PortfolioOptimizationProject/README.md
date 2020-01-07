# README

## Dependencies

+ `python == 3.6.8` 
+ `pandas == 0.25.3`
+ `numpy == 1.17.4`
+ `quandl == 3.4.8`
+ `sklearn == 0.0`
+ `cvxopt == 1.2.3`

## Compilation

+ Create `data` subdirectory and put the datasets `Benchmark Returns.csv`, `rus1000_stocks_factors.csv` under this directory
+ Run `Python3 raw_data.py` under project subdirectory
+ Run `Python3 main.py` under project subdirectory

## Code Functionality

+ `main.py` The main process from problem 1 to problem 7
+ `raw_data.py` Perform data pre-processing as the raw data have bad entries
+ `performance.py` Performance calculation
+ `MVOptimizer.py` Mean variance nonlinear convex optimizer with linear objective
+ `MVManager.py` Rolling assets management and performance evaluation
+ `ReturnPredictor.py` Return Prediction Models

