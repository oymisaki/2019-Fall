import pandas as pd
import numpy as np


def link_cusip_cik():

    data_dir = "./resources/"
    data = pd.read_csv(data_dir + "records.csv")

    CIK_CUSIP = pd.read_csv(data_dir + "cik-cusip_mapping_1993-2018.csv")
    CIK_CUSIP.index = pd.MultiIndex.from_frame(CIK_CUSIP[["cik", "fyear"]])

    d = CIK_CUSIP[["cusip"]].to_dict()
    cik = [
        d["cusip"][x] if x in d["cusip"] else np.nan
        for x in zip(data["CIK"], [int(x[:4]) for x in data["date"]])
    ]

    data["CUSIP"] = pd.Series(cik, index=data.index)
    data.to_csv(data_dir + "records.csv", index=False)


if __name__ == "__main__":
    link_cusip_cik()
