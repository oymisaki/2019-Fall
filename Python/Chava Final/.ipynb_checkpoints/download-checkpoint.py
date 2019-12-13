import requests
import re
import random
import os

random.seed(903532922)


def download():
    # years to extract
    years = [str(y) for y in range(1995, 2019)]

    # quarters
    qtrs = ['QTR1', 'QTR2', 'QTR3', 'QTR4']

    # url for archive
    archive = 'https://www.sec.gov/Archives/'

    # url for full-index
    full_index = 'https://www.sec.gov/Archives/edgar/full-index/'

    # url for data
    os.mkdir('./data')
    data_path = './data/'

    # re expressions to match 8-k lines
    prog = re.compile('[0-9]*\\|[^\\|]*\\|8-K\\|[^\\|]*\\|.*txt')

    entries = 'CIK,date\n'

    for year in years:
        year_path = data_path + year + '/'
        os.mkdir(year_path)
        for qtr in qtrs:
            qtr_path = year_path + qtr + '/'
            os.mkdir(qtr_path)

            # get master.idx
            master = requests.get(full_index + '/' + year + '/' + qtr +
                                  '/master.idx')

            # parse for all the 8ks entries
            all8ks = prog.findall(master.text)

            # randomly select 100 entries
            rnd100 = random.choices(all8ks, k=100)

            for item in rnd100:
                try:
                    splits = item.split('|')
                    company = splits[0]
                    company.replace("/", "")

                    date = splits[3]
                    url = archive + splits[-1]

                    # get specific 8-k file and save
                    r = requests.get(url)
                    with open(qtr_path + '/' + company + "|" + date + '.txt',
                              'w') as file:
                        file.write(r.text)

                    entries += company + ',' + date + '\n'
                except:
                    pass

    # save CIKs and dates
    with open(data_path + 'records.csv', 'w') as file:
        file.write(entries)


if __name__ == '__main__':
    download()