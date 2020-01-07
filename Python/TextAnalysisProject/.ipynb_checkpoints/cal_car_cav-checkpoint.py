import pandas as pd
import numpy as np
from datetime import datetime, timedelta


def serilize():
    data = pd.read_csv('resources/sub_dsf.csv', na_values=["C", "B"])
    print(data.head())
    data.to_pickle('resources/sub_dsf.pkl')


def serilize_ret_data():
    data = pd.read_csv('resources/sub_dsf.csv', na_values=["C", "B"])

    ret_data = data.set_index(["DATE", "CUSIP"])['RET'].unstack()

    ret_data.astype('float64', copy=False)
    ret_data.index = pd.to_datetime(ret_data.index)
    ret_data.sort_index(inplace=True)

    ret_data.to_pickle('resources/ret_data.pkl')


def serilize_to_data():
    data = pd.read_csv('resources/sub_dsf.csv', na_values=["C", "B"])

    to_data = data.set_index(["DATE", "CUSIP"])['TO'].unstack()
    to_data.astype('float64', copy=False)
    to_data.index = pd.to_datetime(to_data.index)
    to_data.sort_index(inplace=True)

    to_data.to_pickle('resources/to_data.pkl')


def gather_car():
    ret_data = pd.read_pickle('resources/ret_data.pkl')

    index_data = pd.read_sas("resources/dsi.sas7bdat")
    index_data.index = pd.to_datetime(index_data["date"])

    def cal_car(date, cusip):
        date_format = "%Y-%m-%d"
        date = datetime.strptime(date, date_format)
        begin, end = date - timedelta(345), date - timedelta(96)
        result = {0: np.nan, 1: np.nan, 3: np.nan, 5: np.nan}
        keys = ["car_w0", "car_w1", "car_w3", "car_w5"]
        if cusip not in ret_data.columns:
            print(cusip)
            return {k: v for k, v in zip(keys, list(result.values()))}

        ret = ret_data[cusip][begin:end].astype('float64')
        mkt = index_data.loc[begin:end, "VWRETX"]
        beta = ret.cov(mkt)
        alpha = ret.mean() - mkt.mean() * beta

        for w in result:
            try:
                wbegin, wend = date - timedelta(w), date + timedelta(w)
                result[w] = (
                    ret_data.loc[wbegin:wend, cusip].astype('float64') -
                    beta * index_data.loc[wbegin:wend, "VWRETX"] -
                    alpha).sum()
            except Exception as e:
                print(cusip, e)
                pass

        return {k: v for k, v in zip(keys, list(result.values()))}

    records = pd.read_csv("out/records.csv")
    car = [cal_car(row["date"], row["CUSIP"]) for _, row in records.iterrows()]
    car_df = pd.DataFrame(car, index=records.index)

    records = pd.concat([records, car_df], axis=1)
    records.to_csv("out/records_car.csv", index=False)


def gather_cav():
    to_data = pd.read_pickle('resources/to_data.pkl')

    def cal_cav(date, cusip):
        print(cusip)
        date_format = "%Y-%m-%d"
        date = datetime.strptime(date, date_format)
        begin, end = date - timedelta(71), date - timedelta(11)
        result = {0: np.nan, 1: np.nan, 3: np.nan, 5: np.nan}
        keys = ["cav_w0", "cav_w1", "cav_w3", "cav_w5"]
        if cusip not in to_data.columns:
            return {k: v for k, v in zip(keys, list(result.values()))}

        C = 2.55 * 10**(-6)
        to = np.log(to_data[cusip][begin:end].astype('float64') + C)
        mean, std = to.mean(), to.std()

        for w in result:
            try:
                wbegin, wend = date - timedelta(w), date + timedelta(w)
                result[w] = (
                    (np.log(to_data.loc[wbegin:wend, cusip].astype('float64') + C) - mean)
                    / std).sum()
            except:
                pass

        return {k: v for k, v in zip(keys, list(result.values()))}

    records = pd.read_csv("out/records_car.csv")
    cav = [cal_cav(row["date"], row["CUSIP"]) for _, row in records.iterrows()]
    cav_df = pd.DataFrame(cav, index=records.index)

    records = pd.concat([records, cav_df], axis=1)
    records.to_csv("out/records_car_cav.csv", index=False)


if __name__ == "__main__":
    gather_car()
