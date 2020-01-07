import pandas as pd
import numpy as np
import os
import re

from nltk.sentiment.vader import SentimentIntensityAnalyzer
from nltk import tokenize


def cal_tone():

    # re expressions to parse the text
    prog_line = re.compile('\n')
    prog_nbsp = re.compile('&nbsp;')
    prog_tag = re.compile('<.*?>')

    res = []
    sid = SentimentIntensityAnalyzer()
    for root, _, files in os.walk("data/"):
        for file in files:
            CIK, date = file.split('|')[0], file.split('|')[1][:10]
            entry = {'CIK': int(CIK), 'date': date}
            with open(root + '/' + file, 'r') as f:
                text = f.read()
                text = text[text.find('<DOCUMENT>'): text.find('</DOCUMENT>')]
                text = prog_line.sub(' ', text)
                text = prog_nbsp.sub(' ', text)
                text = prog_tag.sub('', text)

                sentences = tokenize.sent_tokenize(text)
                scores = [sid.polarity_scores(stn)['compound'] for stn in sentences[2:-4]]
                try:
                    entry['tone'] = sum(scores) / len(scores)
                except:
                    entry['tone'] = np.nan
            res.append(entry)

    res_df = pd.DataFrame(res)
    res_df.to_csv('out/adv_sentiment.csv')


if __name__ == "__main__":
    cal_tone()