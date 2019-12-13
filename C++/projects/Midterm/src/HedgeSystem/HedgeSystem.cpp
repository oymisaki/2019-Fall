#include "HedgeSystem.h"
#include <memory>

shared_ptr<BSModel> newBSModel(string type)
{
    if (type == "C")
        return shared_ptr<BSModel>(new BSModelCall());
    else
        return shared_ptr<BSModel>(new BSModelPut());
}

shared_ptr<VanillaOption> newVanillaOption(string type, double k, double p, string exp)
{
    if (type == "C")
        return shared_ptr<VanillaOption>(new VanillaCallOption(k, p, exp));
    else
        return shared_ptr<VanillaOption>(new VanillaPutOption(k, p, exp));
}

void HedgeSystem::readInterestData(string path, int lineNum)
{
    // read interest data
    interestData.readFromCsv(path, lineNum);
    vector<string> headers = {"date", "rate"};
    interestData.setHeaders(headers);
}

void HedgeSystem::readOptionData(string path, int lineNum)
{
    optionData.readFromCsv(path, lineNum);
    vector<string> headers = {"date", "exdate", "cp_flag", "strike_price", "best_bid", "best_offer"};
    optionData.setHeaders(headers);

    // use exdate, cp_flag and strike price to construct multi column index
    vector<int> indexes = {1, 2, 3};
    optionData.setMultiIndex(indexes);
    const multimap<string, unsigned int> &index = optionData.getIndex();

    // each unique key of multi column index defines one unique option
    typedef multimap<string, unsigned int>::const_iterator iter;
    for (iter it = index.cbegin(), end = index.cend(); it != end;)
    {
        uniqueOptions.push_back(it->first);
        it = index.upper_bound(it->first);
    }
}

void HedgeSystem::readstockData(string path, int lineNum)
{
    stockData.readFromCsv(path, lineNum);
    vector<string> headers = {"date", "close_adj"};
    stockData.setHeaders(headers);
}

void HedgeSystem::setResultCsvDir(string path)
{
    outDir = path;
}

void HedgeSystem::setResultGatherer(vector<ResultGatherer *> gatherers_)
{
    gatherers = gatherers_;
}

void HedgeSystem::getResultForOneOption(Table &optionData_)
{
    vector<string> &dates = optionData_["date"];
    typedef vector<string>::const_iterator iter;

    vector<double> stockPrices;
    vector<double> rates;

    for (iter i = dates.begin(); i != dates.end(); ++i)
    {
        string today = *i;

        try
        {
            string expire = optionData_.getStr(today, "exdate");
            string flag = optionData_.getStr(today, "cp_flag");
            double K = optionData_.getDbl(today, "strike_price");
            double V = (optionData_.getDbl(today, "best_bid") +
                        optionData_.getDbl(today, "best_offer")) /
                       2;

            double S = stockData.getDbl(today, "close_adj");
            double r = interestData.getDbl(today, "rate") / 100;

            stockPrices.push_back(S);
            rates.push_back(r);

            shared_ptr<BSModel> model = newBSModel(flag);
            shared_ptr<VanillaOption> option = newVanillaOption(flag, K, V, expire);

            Parameters p(today, S, r, *model, *option);

            for (unsigned int j = 0; j < gatherers.size(); ++j)
                gatherers[j]->gatherResult(p);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    optionData_.addCol("S", stockPrices);
    optionData_.addCol("rate", rates);
}

void HedgeSystem::runAll()
{
    typedef vector<string>::iterator iter;

    this->clearGatherers();

    for (iter it = uniqueOptions.begin(); it != uniqueOptions.end(); ++it)
    {
        Table oneOptionData = optionData.tableEqualRange(*it);
        oneOptionData.setIndex("date");

        this->getResultForOneOption(oneOptionData);
        for (unsigned int i = 0; i < gatherers.size(); ++i)
            oneOptionData.addCol(gatherers[i]->getName(), gatherers[i]->getResult());

        oneOptionData.writeCsv(outDir + (*it) + ".csv");

        this->clearGatherers();
    }
}

void HedgeSystem::runOne(string t0, string tn, string expire, string flag, string K)
{
    this->clearGatherers();
    Table oneOptionData = optionData.tableEqualRange(expire + flag + K);
    oneOptionData.setIndex("date");
    oneOptionData = oneOptionData.tableInRange(t0, tn);
    oneOptionData.setIndex("date");

    this->getResultForOneOption(oneOptionData);
    for (unsigned int i = 0; i < gatherers.size(); ++i)
        oneOptionData.addCol(gatherers[i]->getName(), gatherers[i]->getResult());

    oneOptionData.writeCsv(outDir  + "result.csv");
    this->clearGatherers();
}

void HedgeSystem::clearGatherers()
{
    for (unsigned int i = 0; i < gatherers.size(); ++i)
        gatherers[i]->clear();
}
