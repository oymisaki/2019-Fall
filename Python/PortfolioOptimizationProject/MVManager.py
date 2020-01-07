import pandas as pd
import numpy as np

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

        # manage results
        self._portfolio_return = None
        self._weights = None

    def manage(self, start, end):
        factor_data, ret_data = self.factor_data, self.ret_data
        Y, features = self.Y, self.features
        whole = factor_data['FS_ID'].unique()

        weights, portfolio_ret, predictors = [], [], []
        # predictors: List[]. historical trained predictor

        for date in pd.period_range(start=start, end=end, freq='M'):
            print(date)

            # estimate covariance
            cov_data = ret_data.loc[date - 24:date]
            cov_data = cov_data.loc[:, np.all(pd.notna(cov_data), axis=0)]
            Cov = cov_data.cov()

            # assets pool
            pool = Cov.columns

            # train
            train_data = factor_data.loc[factor_data['DATE'] >= date - 24, :]
            X = train_data.loc[train_data['DATE'] <= date - 1, features]
            y = train_data.loc[train_data['DATE'] <= date - 1, Y]
            ret_predictor = ReturnPredictorFactory(self.return_model).produce(
                X, y)

            # we need the trained model of previous 11 months
            # store the previous predictor
            if len(predictors) < 11:
                predictors.append(ret_predictor)
            else:
                predictors = predictors[1:] + [ret_predictor]

            today_data = factor_data.loc[factor_data['DATE'] == date, :]

            # use the historical predictors to get robust results
            predicted_ret = np.zeros(len(today_data))
            for p in predictors:
                predicted_ret += p.predict(today_data[features].fillna(0))
            predicted_ret /= len(predictors)

            predicted_ret = pd.Series(predicted_ret, index=today_data['FS_ID'])
            predicted_ret = predicted_ret.loc[pool]

            opt = MVOptimizer(maxiter=20, show_progress=False)
            optimal_weights = opt.optimize(predicted_ret, Cov)
            weights.append(
                pd.Series(np.array(optimal_weights)[:, 0],
                          index=pool).reindex(whole))

            today_return = today_data['nextRET'].unstack().fillna(0.5)
            today_return = today_return.loc[pool]
            portfolio_ret.append(
                today_return.T.dot(optimal_weights).iloc[0, 0])

        self._portfolio_return = pd.Series(
            portfolio_ret,
            index=pd.period_range(start=start, end=end, freq='M')) - 0.5

        self._weights = pd.DataFrame(weights,
                                     index=pd.period_range(start=start,
                                                           end=end,
                                                           freq='M'))

    def get_weights(self):
        return self._weights

    def get_portfolio_return(self):
        return self._portfolio_return


class MVCTEFManager:
    def __init__(self, factor_data, ret_data):
        self.ret_data = ret_data
        self.factor_data = factor_data

        # manage results
        self._portfolio_return = None
        self._weights = None

    def manage(self, start, end):
        factor_data, ret_data = self.factor_data, self.ret_data
        whole = factor_data['FS_ID'].unique()

        weights, portfolio_ret = [], []

        for date in pd.period_range(start=start, end=end, freq='M'):
            print(date)

            # estimate covariance
            cov_data = ret_data.loc[date - 24:date]
            cov_data = cov_data.loc[:, np.all(pd.notna(cov_data), axis=0)]
            Cov = cov_data.cov()

            # assets pool
            pool = Cov.columns

            today_data = factor_data.loc[factor_data['DATE'] == date, :]

            predicted_ret = today_data['CTEF']
            predicted_ret.reset_index(level='DATE', drop=True, inplace=True)
            predicted_ret = predicted_ret.loc[pool]

            opt = MVOptimizer(maxiter=20, show_progress=False)
            optimal_weights = opt.optimize(predicted_ret, Cov)
            weights.append(
                pd.Series(np.array(optimal_weights)[:, 0],
                          index=pool).reindex(whole))

            today_return = today_data['nextRET'].unstack().fillna(0.5)
            today_return = today_return.loc[pool]
            portfolio_ret.append(
                today_return.T.dot(optimal_weights).iloc[0, 0])

        self._portfolio_return = pd.Series(
            portfolio_ret,
            index=pd.period_range(start=start, end=end, freq='M')) - 0.5

        self._weights = pd.DataFrame(weights,
                                     index=pd.period_range(start=start,
                                                           end=end,
                                                           freq='M'))

    def get_weights(self):
        return self._weights

    def get_portfolio_return(self):
        return self._portfolio_return