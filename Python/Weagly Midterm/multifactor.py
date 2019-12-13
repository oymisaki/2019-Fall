#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Oct 31 17:45:23 2019

@author: mingjili
"""
import numpy as np
import pandas as pd
import csv
import xlrd
from sklearn.linear_model import LinearRegression

#Read the data
with open("F-F_Research_Data_5_Factors_2x3.csv") as csvfile:
    reader = csv.reader(csvfile)
    rows = [row for row in reader]
ff5 = pd.DataFrame(rows[4:679], columns = rows[3]).astype(float)
ff5.set_index([''], inplace = True)
ff5_ann = pd.DataFrame(rows[682:], columns = rows[3]).astype(float)


with open("25_Portfolios_5x5.csv") as csvfile:
    reader = csv.reader(csvfile)
    rows = [row for row in reader]
port = pd.DataFrame(rows[16:], columns = rows[15])


with open("F-F_Momentum_Factor.csv") as csvfile:
    reader = csv.reader(csvfile)
    rows = [row for row in reader]
mtm = pd.DataFrame(rows[14:1127], columns = ['Date', 'UMD']).astype(float)
mtm.set_index(['Date'], inplace = True)

hml_dev = pd.read_csv("HML-DEV.csv").astype(float)
hml_dev.set_index(['Date'], inplace = True)

#table 1
ff5_col = list(ff5.columns[:-1])
reg_df1 = pd.DataFrame(index = ff5_col, columns = ff5_col+['Intercept'])
for col in ff5_col:
    x = np.array(ff5.loc[:, (ff5.columns != col)&(ff5.columns != 'RF')])
    x = np.vstack([x.T, np.ones(len(x))]).T
    y = np.array(ff5.loc[:, col])
    res = np.linalg.lstsq(x, y)[0]
    reg_df1.loc[col, reg_df1.columns != col] = res

#table 2
ff6_col = ff5_col+['UMD']
reg_df2 = pd.DataFrame(index = ff6_col, columns = ff6_col+['Intercept'])
ff6 = ff5.join(mtm, how='inner')
for col in ff6_col:
    x = np.array(ff6.loc[:, (ff6.columns != col)&(ff6.columns != 'RF')])
    x = np.vstack([x.T, np.ones(len(x))]).T
    y = np.array(ff6.loc[:, col])
    res = np.linalg.lstsq(x, y)[0]
    reg_df2.loc[col, reg_df2.columns != col] = res
    
#table 3
ff5_new = ff5.join(hml_dev, how='inner')



