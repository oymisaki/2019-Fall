import pandas as pd 


def main():
    data = pd.read_excel("./data/BL Solutions (Part 1).xlsx")
    cov = data.iloc[:, 2:].cov()
    tau = 0.1
    risk_aversion = 3.0
    market_variance = cov.sum()
    print(cov.head())


if __name__ == "__main__":
    main()