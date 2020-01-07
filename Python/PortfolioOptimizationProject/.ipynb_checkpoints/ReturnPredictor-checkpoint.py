# If you have any other more return prediction model
# implement them here
# create a new class and
# inheret from ReturnPredictor class

import pandas as pd
import numpy as np
from sklearn.svm import SVR
from sklearn.model_selection import GridSearchCV
from sklearn.linear_model import LinearRegression
from sklearn.ensemble import RandomForestRegressor


def linear_model(X, y):
    na_filter_x = np.all(pd.notna(X), axis=1)
    na_filter_y = pd.notna(y)

    na_filter = np.logical_and(na_filter_x, na_filter_y)
    X, y = X.loc[na_filter], y.loc[na_filter]

    reg = LinearRegression().fit(X, y)
    return reg


def svr_model(X, y):
    na_filter_x = np.all(pd.notna(X), axis=1)
    na_filter_y = pd.notna(y)

    na_filter = np.logical_and(na_filter_x, na_filter_y)
    X, y = X.loc[na_filter], y.loc[na_filter]

    # model = SVR(kernel='rbf', gamma='auto')
    # param_grid = {'C':[0.1, 1, 10]}
    # reg = GridSearchCV(model, param_grid, cv=3)
    # reg.fit(X, y)
    # C = reg.best_params_['C']

    model = SVR(kernel='rbf', gamma='auto', C=0.1)
    model.fit(X, y)
    return model


def randomforest_model(X, y):
    na_filter_x = np.all(pd.notna(X), axis=1)
    na_filter_y = pd.notna(y)

    na_filter = np.logical_and(na_filter_x, na_filter_y)
    X, y = X.loc[na_filter], y.loc[na_filter]

    model = RandomForestRegressor(n_estimators=100, max_depth=2)
    model.fit(X, y)
    return model


class ReturnPredictorFactory:
    def __init__(self, predictorType):
        self.type = predictorType
        
    def produce(self, X, y):
        if self.type == 'LinearModel':
            return LinearPredictor(X, y)
        elif self.type == 'SVR':
            return SVRPredictor(X, y)
        elif self.type == 'RandomForest':
            return RFPredictor(X, y)


class ReturnPredictor:
    def __init__(self, X, y):
        pass

    def predict(self, todayData):
        pass


class LinearPredictor(ReturnPredictor):
    def __init__(self, X, y):
        self.model = linear_model(X, y)

    def predict(self, todayData):
        return self.model.predict(todayData)


class SVRPredictor(ReturnPredictor):
    def __init__(self, X, y):
        self.model = svr_model(X, y)

    def predict(self, todayData):
        return self.model.predict(todayData)


class RFPredictor(ReturnPredictor):
    def __init__(self, X, y):
        self.model = randomforest_model(X, y)

    def predict(self, todayData):
        return self.model.predict(todayData)