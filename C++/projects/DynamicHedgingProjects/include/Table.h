#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <iterator>
#include <regex>
#include <algorithm>

using namespace std;

std::istream& safeGetline(std::istream& is, std::string& t);

// Self designed table class

class Table
{
private:
    vector<vector<string>> raw;

    vector<string> headers;
    multimap<string, unsigned int> index;

public:
    Table() {}
    ~Table() {}

    Table(unsigned int size);

    void readFromCsv(string path, int lineNum);
    void writeCsv(string path) const;
    void setHeaders(vector<string> headers);
    void setIndex(string colName);
    void setIndex(unsigned int colIndex);
    void setMultiIndex(vector<int> indexes);

    vector<string> &operator[](string colName);
    vector<string> &operator[](unsigned int colIndex);

    string getStr(string row, string col) const;
    double getDbl(string row, string col) const;
    int getInt(string row, string col) const;
    const multimap<string, unsigned int>& getIndex() const;

    Table tableEqualRange(string indexer) const;
    Table tableInRange(string start, string end) const;
    void printHead(int n) const;

    void addCol(string colName, const vector<string>& col);
    void addCol(string colName, const vector<double>& col);
};

#endif // !TABLE_H