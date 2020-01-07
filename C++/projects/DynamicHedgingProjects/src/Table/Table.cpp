#include "Table.h"
#include <cassert>

Table::Table(unsigned int size)
    : raw(size)
{
}

void Table::readFromCsv(string path, int lineNum)
{
    ifstream infile;
    try
    {
        infile = ifstream(path);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    string line;

    for (int num = 0; safeGetline(infile, line) && num < lineNum; ++num)
    {
        // parse line into string tokens separated by comma, mind the "" escaper
        if (line == "")
            continue;
        regex separator(",(?=(?:[^\"]*\"[^\"]*\")*(?![^\"]*\"))");
        sregex_token_iterator tokens(line.cbegin(), line.cend(), separator, -1);
        sregex_token_iterator end;

        if (num == 0)
        {
            while (tokens != end)
                this->headers.push_back(*tokens++);

            this->raw = vector<vector<string>>(headers.size());
        }
        int colIndex = 0;
        while (tokens != end)
            this->raw[colIndex++].push_back(*tokens++);
    }

    // default the first column is the index
    this->setIndex(0);
}

void Table::writeCsv(string path) const
{
    ofstream outfile;
    try
    {
        outfile = ofstream(path);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    copy(headers.begin(), headers.end() - 1, ostream_iterator<string>(outfile, ","));
    outfile << *(headers.end() - 1) << endl;

    for (unsigned int i = 0; i < index.size(); i++)
    {
        for (unsigned int j = 0; j < headers.size() - 1; j++)
        {
            outfile << raw[j][i] << ",";
        }
        outfile << raw[headers.size() - 1][i] << endl;
    }
}

void Table::setHeaders(vector<string> headers)
{
    assert(headers.size() == raw.size() && "New headers' length should equal size of table");
    this->headers = headers;
}

void Table::setIndex(string colName)
{
    typedef vector<string>::iterator viter;
    viter j = find(headers.begin(), headers.end(), colName);
    if (j == headers.end())
        throw "Table::ColumnKeyError, " + colName;

    unsigned int colIndex = j - headers.begin();
    index = multimap<string, unsigned int>();
    for (unsigned int k = 0; k < raw[colIndex].size(); k++)
    {
        index.insert(pair<string, int>(raw[colIndex][k], k));
    }
}

void Table::setIndex(unsigned int colIndex)
{
    if (colIndex >= raw.size())
        throw "Table::ColumnKeyError, " + to_string(colIndex);

    index = multimap<string, unsigned int>();
    for (unsigned int k = 0; k < raw[colIndex].size(); k++)
    {
        index.insert(pair<string, int>(raw[colIndex][k], k));
    }
}

void Table::setMultiIndex(vector<int> indexes)
{
    // use concatenation of table entries to construct multiple index
    index = multimap<string, unsigned int>();

    if (raw.size() == 0)
        return;

    for (unsigned int k = 0; k < raw[0].size(); ++k)
    {
        string key = "";
        for (unsigned int i = 0; i < indexes.size(); ++i)
        {
            if (indexes[i] >= (int)raw.size() || indexes[i] < 0)
                throw "Table::ColumnKeyError, " + to_string(indexes[i]);
            key += raw[indexes[i]][k];
        }
        index.insert(pair<string, int>(key, k));
    }
}

vector<string> &Table::operator[](string colName)
{
    typedef vector<string>::const_iterator iter;
    iter i = find(headers.cbegin(), headers.cend(), colName);
    if (i != headers.end())
        return raw[i - headers.begin()];
    else
        throw "Table::ColumnKeyError, " + colName;
}

vector<string> &Table::operator[](unsigned int colIndex)
{
    if (colIndex < raw.size())
        return raw[colIndex];
    else
        throw "Table::ColumnKeyError, " + to_string(colIndex);
}

string Table::getStr(string row, string col) const
{
    typedef vector<string>::const_iterator viter;
    viter j = find(headers.cbegin(), headers.cend(), col);
    if (j == headers.end())
        throw "Table::ColumnKeyError, " + col;

    typedef multimap<string, unsigned int>::const_iterator miter;
    miter i = index.find(row);
    if (i == index.end())
        throw "Table::IndexKeyError, " + row;

    return raw[j - headers.begin()][i->second];
}

double Table::getDbl(string row, string col) const
{
    string str = getStr(row, col);
    try
    {
        return stod(str);
    }
    catch (const std::exception &e)
    {
        auto res = index.find(row);
        throw "Invaid value at line " + to_string(res->second);
    }
}

int Table::getInt(string row, string col) const
{
    return stoi(getStr(row, col));
}

const multimap<string, unsigned int> &Table::getIndex() const
{
    return this->index;
}

Table Table::tableEqualRange(string indexer) const
{
    auto range = index.equal_range(indexer);
    Table slice(raw.size());
    slice.setHeaders(this->headers);
    for (auto i = range.first; i != range.second; ++i)
    {
        for (unsigned int j = 0; j < headers.size(); ++j)
        {
            slice[j].push_back(raw[j][i->second]);
        }
    }

    return slice;
}

Table Table::tableInRange(string start, string end) const
{
    // use string compare to get a table whose index is in the range
    Table slice(raw.size());
    slice.setHeaders(this->headers);
    for (auto i = index.begin(); i != index.end(); ++i)
    {
        if (i->first >= start && i->first <= end)
        {
            for (unsigned int j = 0; j < headers.size(); ++j)
            {
                slice[j].push_back(raw[j][i->second]);
            }
        }
    }

    return slice;
}

void Table::printHead(int n) const
{
    copy(headers.begin(), headers.end(), ostream_iterator<string>(cout, " "));
    cout << endl;
    int size = static_cast<int>(raw.size() > 0 ? raw[0].size() : 0);
    n = n < size ? n : size;
    for (int i = 0; i < n; i++)
    {
        for (unsigned int j = 0; j < headers.size(); j++)
        {
            cout << raw[j][i] << " ";
        }
        cout << endl;
    }
}

void Table::addCol(string colName, const vector<string> &col)
{
    if (raw.size() > 0 && raw[0].size() != col.size())
        throw "New columns' length should equal length of table";

    this->headers.push_back(colName);
    this->raw.push_back(col);
}

void Table::addCol(string colName, const vector<double> &col)
{
    if (raw.size() > 0 && raw[0].size() != col.size())
        throw "New columns' length should equal length of table";

    this->headers.push_back(colName);
    this->raw.push_back(vector<string>());
    vector<string> &thisCol = this->raw[this->raw.size() - 1];
    transform(col.begin(), col.end(), back_insert_iterator<vector<string>>(thisCol), [](double x) { return to_string(x); });
}

// This code is copied from the internet to get better performance than getline
std::istream &safeGetline(std::istream &is, std::string &t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf *sb = is.rdbuf();

    for (;;)
    {
        int c = sb->sbumpc();
        switch (c)
        {
        case '\n':
            return is;
        case '\r':
            if (sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if (t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}
