import pandas as pd
import numpy as np
from dateutil.relativedelta import relativedelta

from ReturnPredictor import ReturnPredictorFactory
from performance import PerformanceEvaluator
from MVOptimizer import MVOptimizer


class MVManager:
    
    def __init__(self, factor_data, ret_data, return_model, features, Y):
        self.ret_data = ret_data
        self.factor_data = factor_data
        self.return_model = return_model
        self.features = features
        self.Y = Y


    def manage(self, start, end):
        factor_data, ret_data = self.factor_data, self.ret_data
        Y, features = self.Y, self.features

        weight_list = []
        pool_list = []
        portfolio_ret = []
        predictor_list = []

        for end_date in pd.date_range(start=start, end=end, freq='M'):
            print(end_date)
            end_date = end_date.to_period('M')
            start_date = end_date - 24

            # estimate covariance
            cov_data = ret_data.loc[start_date:end_date]
            cov_data = cov_data.loc[:, np.all(pd.notna(cov_data), axis=0)]
            Cov = cov_data.cov()

            # assets pool
            pool = Cov.columns
            pool_list.append(list(pool))

            train_end = end_date - 1
            train_data = factor_data.loc[factor_data['DATE'] >= start_date, :]
            X = train_data.loc[train_data['DATE'] <= train_end, features]
            y = train_data.loc[train_data['DATE'] <= train_end, Y]
            ret_predictor = ReturnPredictorFactory(self.return_model).produce(X, y)

            # store the previous predictor 
            if len(predictor_list) < 11:
                predictor_list.append(ret_predictor)
            else:
                predictor_list = predictor_list[1:] + [ret_predictor]

            todayData = factor_data.loc[factor_data['DATE'] == end_date, :]
            predicted_ret = np.zeros(len(todayData))

            for m in predictor_list:
                predicted_ret += m.predict(todayData[features].fillna(0))

            predicted_ret = pd.Series(predicted_ret, index=todayData['Symbol']) / 11
            predicted_ret = predicted_ret.loc[pool]

            opt = MVOptimizer(maxiter=20, show_progress=False)
            optimal_weights = opt.optimize(predicted_ret, Cov)
            weight_list.append(np.mat(optimal_weights))

            todayReturn = todayData['RETURN'].unstack()
            todayReturn = todayReturn.loc[pool]
            portfolio_ret.append(todayReturn.T.dot(optimal_weights).iloc[0, 0])

        portfolio_ret = [i-0.5 for i in portfolio_ret]
        Perf = PerformanceEvaluator(pd.Series(portfolio_ret))
        print(Perf.evaluate())
