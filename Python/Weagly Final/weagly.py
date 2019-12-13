import pandas as pd
import numpy as np
from sklearn.linear_model import LinearRegression
from sklearn.linear_model import LassoCV, Lasso
from sklearn.ensemble import RandomForestRegressor


def calculate_signals():
    data = pd.read_csv('data/student_data.csv')
    data['MktVal'] = data['PRC'] * data['SHROUT']
    data['date'] = pd.to_datetime(data['date'].astype('str'))
    data.sort_values(by='date', inplace=True)

    Y = 'MktVal'
    features = [
        'atq', 'dvpq', 'seqq', 'icaptq', 'pstkq', 'ppentq', 'ceqq', 'pstkrq',
        'dlttq', 'aoq', 'ltq', 'loq', 'acoq', 'lcoq', 'apq', 'ibadjq_MA4',
        'ibcomq_MA4', 'xidoq_MA4', 'doq_MA4', 'nopiq_MA4', 'ibq_MA4',
        'txtq_MA4', 'niq_MA4', 'cheq_MA4', 'saleq_MA4', 'dvy_MA4', 'piq_MA4'
    ]

    SignalOLS, SignalLasso, SignalRF = pd.Series(), pd.Series(), pd.Series()
    for date in sorted(data['date'].unique()):
        print(date)
        indices = (data['date'] == date)

        # OLS
        X, y = data.loc[indices, features], data.loc[indices, Y]
        reg = LinearRegression().fit(X, y)

        SignalOLS = SignalOLS.append((reg.predict(X) - y) / y)

        # Lasso
        lassocv = LassoCV(alphas=None, cv=10, max_iter=100000, normalize=True)
        lassocv.fit(X, y)

        lasso = Lasso(max_iter=10000, normalize=True)
        lasso.set_params(alpha=lassocv.alpha_)
        lasso.fit(X, y)

        coefs = pd.Series(lasso.coef_, index=X.columns)
        selected_features = list(coefs[coefs != 0].index)

        # Use OLS again
        X, y = data.loc[indices, selected_features], data.loc[indices, Y]
        reg = LinearRegression().fit(X, y)

        SignalLasso = SignalLasso.append((reg.predict(X) - y) / y)

        # Random Forests
        rf_reg = RandomForestRegressor(n_estimators=1000,
                                       random_state=42,
                                       min_samples_leaf=20)
        rf_reg.fit(X, y)

        SignalRF = SignalRF.append((rf_reg.predict(X) - y) / y)

    data['SignalOLS'] = SignalOLS
    data['SignalLasso'] = SignalLasso
    data['SignalRF'] = SignalRF
    res_cols = [
        'date', 'PERMNO', 'gvkey', 'COMNAM', 'CUSIP', 'SICCD', 'TICKER', 'PRC',
        'SHROUT', 'RET', 'nextmonth', 'next_Ret', 'SignalOLS', 'SignalLasso',
        'SignalRF'
    ]

    data['date'] = data['date'].apply(lambda x: x.strftime('%Y-%m-%d'))
    res = data[res_cols]
    # cost time
    # so save the result rather than return
    res.to_csv('out/signals.csv')


def get_portfolio_returns():
    data = pd.read_csv('out/signals.csv')

    ret_df = data.set_index(['date', 'PERMNO'])['next_Ret'].unstack()
    ret_df.index = pd.to_datetime(ret_df.index)
    ret_df.sort_index(inplace=True)

    portfolio_ret = {}

    for signal in ['SignalOLS', 'SignalLasso', 'SignalRF']:
        signal_df = data.set_index(['date', 'PERMNO'])[signal].unstack()
        signal_df.index = pd.to_datetime(signal_df.index)
        signal_df.sort_index(inplace=True)

        res = []

        for date in ret_df.index:
            EWR, SWR = 'EWR', 'SWR'
            row = {EWR: np.nan, SWR: np.nan}

            # rank for signal for the asset pool
            pool = signal_df.loc[date, \
                                 pd.notnull(signal_df.loc[date, :])].sort_values()

            q5 = pool.iloc[int(0.8 * len(pool)):].index
            q1 = pool.iloc[:int(0.2 * len(pool))].index

            row[EWR] = ret_df.loc[date, q5].mean() - \
                        ret_df.loc[date, q1].mean()

            wq1 = signal_df.loc[date, q1] / signal_df.loc[date, q1].sum()
            wq5 = signal_df.loc[date, q5] / signal_df.loc[date, q5].sum()

            row[SWR] = (ret_df.loc[date, q5] * wq5).sum() - \
                        (ret_df.loc[date, q1] * wq1).sum()

            res.append(row)

        portfolio_ret[signal] = pd.DataFrame(res, index=ret_df.index).fillna(0)

    return portfolio_ret


def get_cum_ret(portfolio_ret):

    cum_ret = {}

    for signal in portfolio_ret:
        ret = {}

        for p in portfolio_ret[signal].columns:
            ret[p] = (portfolio_ret[signal][p] + 1).prod() - 1
        cum_ret[signal] = ret

    return cum_ret


if __name__ == "__main__":
    # calculate_signals()

    portfolios = get_portfolio_returns()
    print(portfolios)
    for p in portfolios:    
        portfolios[p].to_csv('out/{}PortRet.csv'.format(p))

    cum_ret = get_cum_ret(portfolios)

    print(cum_ret)
