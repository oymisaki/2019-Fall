import pandas as pd
import numpy as np

from ReturnPredictor import ReturnPredictorFactory, linear_model
from performance import PerformanceEvaluator
from MVOptimizer import MVOptimizer
from MVManager import MVManager, MVCTEFManager


def main():

    # problem 1
    benchmark = pd.read_csv('./data/Benchmark Returns.csv')

    Perf = PerformanceEvaluator(benchmark.loc[(benchmark['Date'] <= 20181130) &
                                              (benchmark['Date'] >= 20030101),
                                              'Russell 1000 Bench Return'])
    print(Perf.evaluate())

    factor_data = pd.read_csv('./data/factor_data.csv',
                              skiprows=4,
                              skip_blank_lines=True)

    # change daily timestamp to monthly timestamp
    factor_data['DATE'] = pd.to_datetime(factor_data['DATE']).dt.to_period('M')
    factor_data['RETURN'] /= 100

    factor_data.index = pd.MultiIndex.from_frame(factor_data[['FS_ID',
                                                              'DATE']])

    # t+1 return
    factor_data['nextRET'] = factor_data['RETURN'].groupby(
        factor_data['FS_ID']).apply(lambda x: x.shift(-1))

    # problem 2
    ret_data = factor_data.set_index(['DATE', 'FS_ID'])['RETURN'].unstack()
    ret_data.sort_index(inplace=True)

    Dec2004 = ret_data.loc['2002-12':'2004-12']
    Dec2004 = Dec2004.loc[:, np.all(pd.notna(Dec2004), axis=0)]
    Avg = Dec2004.mean()
    Var = Dec2004.var()
    Cov = Dec2004.cov()
    print(Avg, '\n', Var, '\n', Cov)

    # assets pools for Dec-2004
    pool = Cov.columns

    # Problem 3
    Y = 'nextRET'
    # features = ['EP1', 'BP', 'CP', 'SP', 'REP', 'RBP', 'RCP', 'RSP', 'CTEF', 'PM']
    features = ['REP', 'RBP', 'RCP', 'RSP', 'CTEF']
    universe2004 = factor_data.loc[factor_data['DATE'] <= '2004-12', :]
    print(linear_model(universe2004[features], universe2004[Y]))

    # Problem 4
    # note that the window should be 24 months
    # but because the earliest date is 2002-12-31 and today is 2004-12-31
    # so I didn't specify the window begin date but only the end date
    # In other month beside 2004-12-31, remember to specify the data window

    # train 11 times
    model_list = []
    for date in pd.period_range(start='2004-01', end='2004-11', freq='M'):
        X = factor_data.loc[factor_data['DATE'] <= date, features]
        y = factor_data.loc[factor_data['DATE'] <= date, Y]
        model_list.append(linear_model(X, y))

    todayData = factor_data.loc[factor_data['DATE'] == '2004-12', :]

    # get the 11 times training average
    predicted_ret = np.zeros(len(todayData))
    for m in model_list:
        predicted_ret += m.predict(todayData[features].fillna(0))
    predicted_ret /= 11

    predicted_ret = pd.Series(predicted_ret, index=todayData['FS_ID'])

    # there are NA values, use available assets pool in Dec 2004 to select non-na values
    # and proceed to optimization
    predicted_ret = predicted_ret.loc[pool]
    print(predicted_ret)

    # Problem 4
    # now we have predicted returns and covariance matrix
    # opt = MVOptimizer(show_progress=False)
    # optimal_weights = opt.optimize(predicted_ret, Cov)
    # print(optimal_weights)

    # Problem 5
    benchmark = benchmark.loc[(benchmark['Date'] <= 20181201) &
                              (benchmark['Date'] >= 20050101),
                              'Russell 1000 Bench Return']
    benchmark.index = pd.period_range('2004-12', '2018-10', freq='M')

    # repeat the process from part 2-4
    manager = MVManager(factor_data, ret_data, 'LinearModel', features, Y)
    manager.manage('2004-12', '2018-10')

    manager.get_weights().to_csv("weight_LinearModel.csv")
    portfolio_return = manager.get_portfolio_return()
    Perf = PerformanceEvaluator(portfolio_return, benchmark)
    print(Perf.evaluate())

    # Problem 6
    manager = MVCTEFManager(factor_data, ret_data)
    manager.manage('2004-12', '2018-10')

    manager.get_weights().to_csv("weight_CTEF_LinearModel.csv")
    portfolio_return = manager.get_portfolio_return()
    Perf = PerformanceEvaluator(portfolio_return, benchmark)
    print(Perf.evaluate())

    # Problem 7
    # use SVR and RandomForest Regression to replace the Linear Regression

    for model in ['SVR', 'RandomForest']:

        manager = MVManager(factor_data, ret_data, model, features, Y)
        manager.manage('2004-12', '2018-10')

        manager.get_weights().to_csv("weight_{}_LinearModel.csv".format(model))
        portfolio_return = manager.get_portfolio_return()
        Perf = PerformanceEvaluator(portfolio_return, benchmark)
        print(Perf.evaluate())

    # Save return data
    ret_data.loc['2004-12':'2018-10', 'nextRET'].to_csv('actual_return.csv')


if __name__ == "__main__":
    main()
