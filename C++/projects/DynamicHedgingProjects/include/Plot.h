#ifndef PLOT_H
#define PLOT_H

#include <string>
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

using std::string;

// plotting functions using matplotlib's Apis

void plotLine(vector<double> x, vector<double> y)
{
    plt::plot(x, y);
}

void plotScatter(vector<double> x, vector<double> y)
{
    plt::scatter(x, y);
}

void plotHistogram(vector<double> x, long numOfBins)
{
    plt::hist(x, numOfBins);
}

void plotTitle(string title)
{
    plt::title(title);
}

void savePlot(string path)
{
    plt::save(path);
}

void plotClose()
{
    plt::close();
}

#endif // PLOT_H