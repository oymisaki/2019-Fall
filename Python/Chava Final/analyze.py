import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from collections import defaultdict


def analyze_car_cav():
    car_cav = pd.read_csv('out/records_car_cav.csv')
    car_cav.index = pd.MultiIndex.from_frame(car_cav[['CIK', 'date']])

    cols = [
        'car_w0', 'car_w1', 'car_w3', 'car_w5', 'cav_w0', 'cav_w1', 'cav_w3',
        'cav_w5'
    ]

    mean = car_cav[cols].mean()
    std = car_cav[cols].std()
    median = car_cav[cols].quantile()
    q25 = car_cav[cols].quantile(0.25)
    q75 = car_cav[cols].quantile(0.75)
    mi = car_cav[cols].min()
    ma = car_cav[cols].max()

    stats = pd.concat([mean, std, median, q25, q75, mi, ma], axis=1)
    stats.columns = ['mean', 'std', 'median', 'q25', 'q75', 'min', 'max']

    print(stats)

    # plot distribution and save
    for col in cols:
        plt.hist(car_cav[col][pd.notnull(car_cav[col])],
                 bins=100,
                 range=(-0.5, 0.5))
        plt.title(col)
        plt.savefig('out/pic/{}.png'.format(col))
        plt.close()


def analyze_rudi_quintiles():
    rudi = pd.read_csv('out/rudi_sentiment.csv')
    rudi.index = pd.MultiIndex.from_frame(rudi[['CIK', 'date']])

    car_cav = pd.read_csv('out/records_car_cav.csv')
    car_cav.index = pd.MultiIndex.from_frame(car_cav[['CIK', 'date']])

    rudi['year'] = rudi['date'].apply(lambda x: x[:4])
    years = sorted(rudi['year'].unique())

    res = defaultdict(lambda: defaultdict(list))
    stats = {
        'mean': lambda x: x.mean(),
        'std': lambda x: x.std(),
        'median': lambda x: x.quantile(),
        'q25': lambda x: x.quantile(0.25),
        'q75': lambda x: x.quantile(0.75)
    }

    for year in years:
        print(year)
        qs = ['q1', 'q2', 'q3', 'q4', 'q5', 'q6', 'q7', 'q8', 'q9', 'q10']
        pool = rudi.loc[rudi['year'] == year, :].sort_values(by='pos_neg')
        n = len(pool)

        for s in stats:
            cols = ['car_w0', 'car_w1', 'car_w3', 'car_w5', 'cav_w0', 'cav_w1', 'cav_w3', 'cav_w5']
            for col in cols:
                x = {}
                for i, q in enumerate(qs):
                    l, r = int(i / 10 * n), int((i + 1) / 10 * n)
                    indices = pool.iloc[l: r].index
                    x[q] = stats[s](car_cav.loc[indices, col])

                res[s][col].append(x)

    for stat in res.keys():
        for measure in res[stat].keys():
            res[stat][measure] = pd.DataFrame(res[stat][measure], index=years)

            res[stat][measure].to_csv('out/quintiles/{}_{}_{}.csv'.format('rudi', stat, measure))
            print('Quintiles {} for {}: \n'.format(stat, measure),
                    res[stat][measure])


def analyze_advance_quintiles():
    advance = pd.read_csv('out/adv_sentiment.csv')
    advance.index = pd.MultiIndex.from_frame(advance[['CIK', 'date']])

    car_cav = pd.read_csv('out/records_car_cav.csv')
    car_cav.index = pd.MultiIndex.from_frame(car_cav[['CIK', 'date']])

    advance['year'] = advance['date'].apply(lambda x: x[:4])
    years = sorted(advance['year'].unique())

    res = defaultdict(lambda: defaultdict(list))
    stats = {
        'mean': lambda x: x.mean(),
        'std': lambda x: x.std(),
        'median': lambda x: x.quantile(),
        'q25': lambda x: x.quantile(0.25),
        'q75': lambda x: x.quantile(0.75)
    }

    for year in years:
        print(year)
        qs = ['q1', 'q2', 'q3', 'q4', 'q5', 'q6', 'q7', 'q8', 'q9', 'q10']
        pool = advance.loc[advance['year'] == year, :].sort_values(by='tone')
        n = len(pool)

        for s in stats:
            cols = [
                'car_w0', 'car_w1', 'car_w3', 'car_w5', 'cav_w0', 'cav_w1',
                'cav_w3', 'cav_w5'
            ]
            for col in cols:
                x = {}
                for i, q in enumerate(qs):
                    l, r = int(i / 10 * n), int((i + 1) / 10 * n)
                    indices = pool.iloc[l:r].index
                    x[q] = stats[s](car_cav.loc[indices, col])

                res[s][col].append(x)

    for stat in res.keys():
        for measure in res[stat].keys():
            res[stat][measure] = pd.DataFrame(res[stat][measure], index=years)

            res[stat][measure].to_csv('out/quintiles/{}_{}_{}.csv'.format('adv', stat, measure))
            print('Quintiles {} for {}: \n'.format(stat, measure),
                    res[stat][measure])


if __name__ == "__main__":
    analyze_car_cav()

    analyze_rudi_quintiles()
    analyze_advance_quintiles()

    data = pd.read_csv('out/quintiles/adv_q75_cav_w5.csv')
    print(data.mean())
