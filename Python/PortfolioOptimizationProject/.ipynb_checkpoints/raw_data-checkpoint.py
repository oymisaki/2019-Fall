import re

if __name__ == "__main__":
    # deal with bad inputs in raw data
    # Some company names include ',' which cause bad input when reading
    # this code deals with it by
    # adding "" escaper to company names 
    # which enable pandas to read it correctly

    text = ''
    with open('data/rus1000_stocks_factors.csv', 'r') as file:
        text = file.read()
        text = re.sub(r',(.*)(,[\s0-9]+/)', r',"\1"\2', text)

    with open('data/factor_data.csv', 'w') as file:
        file.write(text)