import pandas as pd
import numpy as np

def link_cusip_cik():
    out_dir = "./out/"
    data_dir = './resources/'

    data = pd.read_csv(out_dir + "records.csv")

    CIK_CUSIP = pd.read_csv(data_dir + "link.csv")
    CIK_CUSIP = CIK_CUSIP[pd.notna(CIK_CUSIP['CIK'])]
    CIK_CUSIP = CIK_CUSIP[pd.notna(CIK_CUSIP['CUSIP'])]

    CIK_CUSIP['CIK'] = CIK_CUSIP['CIK'].astype('int64')
    CIK_CUSIP['CUSIP'] = CIK_CUSIP['CUSIP'].astype('str').apply(lambda x: x[:8])

    CIK_CUSIP.index = CIK_CUSIP['CIK']

    d = CIK_CUSIP["CUSIP"].to_dict()

    cik = [
        d[x] if x in d else np.nan
        for x in data['CIK']
    ]

    data["CUSIP"] = pd.Series(cik, index=data.index)
    data.to_csv(out_dir + "records.csv", index=False)

if __name__ == "__main__":
    link_cusip_cik()
