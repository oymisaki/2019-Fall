import pandas as pd
import numpy as np
import os
import re


def cal_pos_neg():

    # import dictionary
    dct = pd.read_csv('resources/LoughranMcDonald_MasterDictionary_2018.csv')
    negative = dct.loc[dct['Negative'] > 0 ,'Word']
    positive = dct.loc[dct['Positive'] > 0 ,'Word']

    positive = set(s.lower() for s in positive)
    negative = set(s.lower() for s in negative)

    prog_wd = re.compile('[a-zA-Z]+') # etract words
    prog_line = re.compile('\n') # detect line
    prog_nbsp = re.compile('&nbsp;') # detect nbsp
    prog_tag = re.compile('<.*?>') # delete html tag

    res = []

    # walk through all the files and analyze
    for root, _, files in os.walk("data/"):
        for file in files:
            CIK, date = file.split('|')[0], file.split('|')[1][:10]
            d = {'CIK': int(CIK), 'date': date}
            with open(root + '/' + file, 'r') as f:
                print(file)
                text = f.read()

                text = prog_line.sub(' ', text)
                text = prog_nbsp.sub(' ', text)
                text = prog_tag.sub(' ', text)
                
                # extract words
                words = prog_wd.findall(text)
                words = [w.lower() for w in words]
                
                diff = 0
                for w in words:
                    if w in positive:
                        diff += 1
                    elif w in negative:
                        diff -= 1
                
                try:
                    d['pos_neg'] = diff / len(words)
                except:
                    d['pos_neg'] = np.nan
                res.append(d)
                
    res_df = pd.DataFrame(res)
    res_df.to_csv('out/rudi_sentiment.csv', index=False)


if __name__ == "__main__":
    cal_pos_neg()

