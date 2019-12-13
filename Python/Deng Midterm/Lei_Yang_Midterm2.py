import quandl
quandl.ApiConfig.api_key = "ihb1tjz1KpaPsjyU6xMc"

import os
import warnings
from numpy.random import choice, normal
import pandas as pd
import numpy as np

from sklearn.linear_model import LogisticRegression
from sklearn.metrics import roc_auc_score
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split, cross_val_score, GridSearchCV
from sklearn.metrics import roc_auc_score
warnings.filterwarnings('ignore')

np.random.seed(seed=42)


def get_quandl_data(universe, begin, end):
    # get data from quandl
    data = quandl.get_table(
        'WIKI/PRICES',
        ticker=universe,
        qopts={'columns': ['ticker', 'date', 'adj_close', 'adj_volume']},
        date={
            'gte': begin,
            'lte': end
        },
        paginate=True)

    data.index = pd.MultiIndex.from_frame(data[["ticker", "date"]])

    return data


def handle_data(data):
    # fillna
    data.groupby(data["ticker"]).fillna(method="ffill", inplace=True)

    # generate explanatory variable

    # ma5
    ma5 = data["adj_close"].groupby(data["ticker"]).rolling(5).mean().shift(-4)
    ma5.index = pd.MultiIndex.from_frame(ma5.index.to_frame())

    #  macd
    def team_ema(team, span):
        feature_ema = team.rolling(window=span, min_periods=span).mean()[:span]
        rest = team[span:]
        return pd.concat([feature_ema, rest]).ewm(span=span,
                                                  adjust=False).mean()

    ema26 = data["adj_close"].groupby(data["ticker"]).apply(team_ema,
                                                            26).shift(-25)
    ema12 = data["adj_close"].groupby(data["ticker"]).apply(team_ema,
                                                            12).shift(-11)

    macd = ema12 - ema26

    # bollinger bands
    mid_bollinger = data["adj_close"].groupby(
        data["ticker"]).rolling(20).mean().shift(-19)
    std20 = data["adj_close"].groupby(
        data["ticker"]).rolling(20).std().shift(-19)

    mid_bollinger.index = pd.MultiIndex.from_frame(
        mid_bollinger.index.to_frame())
    std20.index = pd.MultiIndex.from_frame(std20.index.to_frame())

    up_bollinger = mid_bollinger + 2 * std20
    low_bollinger = mid_bollinger - 2 * std20

    # RSI
    data["last_close"] = data["adj_close"].groupby(data["ticker"]).shift(-1)
    U = data.apply(lambda row: row["adj_close"] - row["last_close"]
                   if row["adj_close"] > row["last_close"] else 0,
                   axis=1)

    D = data.apply(lambda row: abs(row["adj_close"] - row["last_close"])
                   if row["adj_close"] < row["last_close"] else 0,
                   axis=1)
    avgU = U.groupby(data["ticker"]).rolling(14).mean().shift(-13)
    avgD = D.groupby(data["ticker"]).rolling(14).mean().shift(-13)
    RS = avgU / avgD
    RSI = 100 - 100 / (1 + RS)
    RSI.index = pd.MultiIndex.from_frame(RSI.index.to_frame())

    data["ma5"] = ma5
    data["macd"] = macd
    data["ma20"] = mid_bollinger
    data["std20"] = std20
    data["up_bollinger"] = up_bollinger
    data["low_bollinger"] = low_bollinger
    data["RSI"] = RSI

    # normalization
    for name in [
            "adj_close", "adj_volume", "ma5", "macd", "ma20", "std20",
            "up_bollinger", "low_bollinger", "RSI"
    ]:
        data[name] = data[name].groupby(
            data["ticker"]).apply(lambda x: (x - x.min()) / (x.max() - x.min()))

    # generate Y variable
    data["adj_closet1"] = data["adj_close"].groupby(data["ticker"]).shift(1)
    data["Y"] = data.apply(lambda row: 1
                           if row["adj_close"] < row["adj_closet1"] else 0,
                           axis=1)

    # drop na
    data.dropna(inplace=True)

    return data


def split_data(data, features, target, q=0.6):

    # split data into training set and test set

    data = data.dropna()
    data["date"] = pd.to_datetime(data["date"])
    splitdate = data["date"].groupby(data["ticker"]).quantile(q)

    split = pd.merge(splitdate,
                     data[["ticker", "date"]],
                     how="right",
                     left_index=True,
                     right_index=True,
                     suffixes=("_split", ""))

    train_split = (split["date_split"] >= split["date"])
    X_train = data.loc[train_split, features]
    Y_train = data.loc[train_split, target]

    test_split = np.logical_not(train_split)
    X_test = data.loc[test_split, features]
    Y_test = data.loc[test_split, target]

    return X_train, Y_train, X_test, Y_test


def tune_log_reg(X_train, Y_train, X_test, Y_test):

    # tune parameters in logistics and return the best auc and parameters

    Cs = np.logspace(-5, 5, 11)

    logistic_results, logistic_coeffs = pd.DataFrame(), pd.DataFrame()
    for C in Cs:
        coeffs = []
        log_reg = LogisticRegression(C=C, solver="lbfgs")

        log_reg.fit(X=X_train, y=Y_train)

        Y_pred = log_reg.predict_proba(X_test)[:, 1]
        coeffs.append(log_reg.coef_.squeeze())
        logistic_results = (logistic_results.append(
            Y_test.to_frame('actuals').assign(predicted=Y_pred, C=C)))

        logistic_coeffs[C] = np.mean(coeffs, axis=0)

    auc_by_C = logistic_results.groupby('C').apply(lambda x: roc_auc_score(
        y_true=x.actuals.astype(int), y_score=x.predicted))

    best_auc = auc_by_C.max()
    best_C = auc_by_C.idxmax()

    return best_auc, best_C


def run_log_reg(C, X_train, Y_train):

    # use the best paramters and training dataset to fit a logistics model

    coeffs = []
    log_reg = LogisticRegression(C=C, solver="lbfgs")

    log_reg.fit(X=X_train, y=Y_train)
    coeffs.append(log_reg.coef_.squeeze())

    return log_reg, coeffs


def evaluate_model(model, X_test, Y_test):

    # use the fitted model and the testing dataset to evaluate the results

    Y_pred = model.predict_proba(X_test)[:, 1]

    results = pd.DataFrame()
    results = (results.append(
        Y_test.to_frame('actuals').assign(predicted=Y_pred)))

    auc = roc_auc_score(y_true=results.actuals.astype(int),
                        y_score=results.predicted)

    return results, auc


# ---------------- random forest --------------- #

def tune_random_forest(X_train, Y_train):
    
    rf_clf = RandomForestClassifier(
        n_estimators=200,  # will change from 10 to 100 in version 0.22 
        oob_score=True,
        n_jobs=-1,
        random_state=42)

    # Grid Search for the best paramters of random forest

    param_grid = {
        'n_estimators': [200, 400],
        'max_depth': [10, 15, 20],
        'min_samples_leaf': [50, 100]
    }

    gridsearch_clf = GridSearchCV(estimator=rf_clf,
                                  param_grid=param_grid,
                                  scoring='roc_auc',
                                  n_jobs=-1,
                                  refit=True,
                                  return_train_score=True)

    gridsearch_clf.fit(X=X_train, y=Y_train)

    return gridsearch_clf.best_params_, gridsearch_clf.best_score_


def run_random_forest(params, X_train, Y_train):

    # run the random forests classifier on the training datasets
    # using the giben best parameters

    clf = RandomForestClassifier(**params)
    clf.fit(X_train, Y_train)

    return clf


def run_on_small_universe():

    # run logistics and random forests on the small universe

    universe = [
        'AAPL', 'ADI', 'CNP', 'DLTR', 'FLS', 'HAS', 'PBCT', 'RHI', 'TGT', 'WBA'
    ]
    begin, end = '2000-01-01', '2019-01-10'

    # get data from quandl
    data = get_quandl_data(universe, begin, end)
    data = handle_data(data)

    # specify target and features
    target = "Y"
    features = [
        "adj_close", "adj_volume", "ma5", "macd", "ma20", "std20",
        "up_bollinger", "low_bollinger", "RSI"
    ]

    # split datasets into trainingset and testing set
    X_train, Y_train, X_test, Y_test = split_data(data, features, target)

    print("tuning logistics")
    # find the best hyperparamters of logistics model
    _, log_best_C = tune_log_reg(X_train, Y_train, X_test, Y_test)

    # train and evaluate
    log_reg, _ = run_log_reg(log_best_C, X_train, Y_train)
    log_results, log_auc = evaluate_model(log_reg, X_test, Y_test)

    print("tuning random forest")
    # find the best hyperparameters of random forest
    rf_best_params, _ = tune_random_forest(X_train, Y_train)

    # train and evaluate
    best_clf = run_random_forest(rf_best_params, X_train, Y_train)
    rf_results, rf_auc = evaluate_model(best_clf, X_test, Y_test)

    logi = {"result": log_results, "auc": log_auc, "best_params": log_best_C}

    rf = {"result": rf_results, "auc": rf_auc, "best_params": rf_best_params}

    return logi, rf


def run_on_large_universe(best_logi_params, best_rf_params):

    # running the algos on the large universe

    # specify target and features 
    target = "Y"
    features = [
        "adj_close", "adj_volume", "ma5", "macd", "ma20", "std20",
        "up_bollinger", "low_bollinger", "RSI"
    ]

    universe = {}

    # read and store data
    for _, _, files in os.walk("./stock_dfs"):
        for file in files:
            try:
                ticker = file.split(r".")[0]
                ticker_data = pd.read_csv("./stock_dfs/" + file)
                ticker_data = ticker_data[["date", "close", "volume"]]
                ticker_data.columns = ["date", "adj_close", "adj_volume"]
                ticker_data["ticker"] = ticker
                ticker_data.sort_values(by="date",
                                        ascending=False,
                                        inplace=True)
                ticker_data.index = pd.MultiIndex.from_frame(
                    ticker_data[["ticker", "date"]])

                universe[ticker] = handle_data(ticker_data)
            except:
                print(ticker)

    universe_logi_results = {}
    universe_logi_metrics = {}

    # run logistics on all the large universe
    for ticker in universe.keys():
        if universe[ticker].empty:
            continue
        
        print(universe[ticker].head(2))
        X_train, Y_train, X_test, Y_test = split_data(universe[ticker], features, target)

        log_reg, _ = run_log_reg(best_logi_params, X_train, Y_train)
        log_results, log_auc = evaluate_model(log_reg, X_test, Y_test)
        universe_logi_results[ticker] = log_results
        universe_logi_metrics[ticker] = log_auc

    universe_rf_results = {}
    universe_rf_metrics = {}

    # run random forests on all the large universe
    for ticker in universe.keys():
        if universe[ticker].empty:
            continue

        X_train, Y_train, X_test, Y_test = split_data(universe[ticker], features, target)

        best_clf = run_random_forest(best_rf_params, X_train, Y_train)
        rf_results, rf_auc = evaluate_model(best_clf, X_test, Y_test)
        universe_rf_results[ticker] = rf_results
        universe_rf_metrics[ticker] = rf_auc

    return {
        "results": universe_logi_results,
        "auc": universe_logi_metrics
    }, {
        "results": universe_rf_results,
        "auc": universe_rf_metrics
    }


def main():

    # run algos on small universe

    print("running on small universe")
    logi_results, rf_results = run_on_small_universe()
    
    # store results
    logi_results["result"].to_csv("./out/sm_universe_logistics.csv")
    rf_results["result"].to_csv("./out/sm_universe_random_forests.csv")

    sm_universe_metrics = pd.DataFrame(
        {
            "random forest": rf_results["auc"],
            "logistics": logi_results["auc"]
        },
        index=["auc"])

    sm_universe_metrics.to_csv("./out/sm_universe_metrics.csv")

    print("running on large universe")
    # run on larger universe
    universe_logi_results, universe_rf_results = run_on_large_universe(
        logi_results["best_params"], rf_results["best_params"])

    # sort the best 20 logistics results
    logi_best_20 = sorted(list(universe_logi_results["auc"].keys()),
                          key=lambda x: universe_logi_results["auc"][x],
                          reverse=True)[:20]

    # store results
    for best in logi_best_20:
        universe_logi_results["results"][best].to_csv(
            "./out/lg_universe_logistics_{}.csv".format(best))

    lg_universe_logistics_metrics = pd.DataFrame(
        {
            "auc": [universe_logi_results["auc"][x] for x in logi_best_20],
        },
        index=logi_best_20)

    lg_universe_logistics_metrics.to_csv(
        "./out/lg_universe_logistics_metrics.csv")

    # sort the best 20 random forests results
    rf_best_20 = sorted(list(universe_rf_results["auc"].keys()),
                        key=lambda x: universe_rf_results["auc"][x],
                        reverse=True)[:20]

    # store results
    for best in rf_best_20:
        universe_rf_results["results"][best].to_csv(
            "./out/lg_universe_random_forests_{}.csv".format(best))

    lg_universe_random_forest_metrics = pd.DataFrame(
        {
            "auc": [universe_rf_results["auc"][x] for x in rf_best_20],
        },
        index=rf_best_20)

    lg_universe_random_forest_metrics.to_csv(
        "./out/lg_universe_random_forest_metrics.csv")


if __name__ == "__main__":

    main()