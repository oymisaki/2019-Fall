#include "PrintLatex.h"
#include <iostream>
#include <vector>
#include <list>
#include <string>

using namespace std;

string format(int i, int j, int m, int n)
{
    return "\\draw[->] (tree-" + to_string(i) + "-" + to_string(j) + ") -- (tree-" + to_string(m) + "-" + to_string(n) +
           ") node [midway, above] {};";
}
void printLatex(double *vec, int count, int periods)
{
    vector<vector<string>> model(2 * periods + 1);
    for (int i = 0; i < 2 * periods + 1; i++)
    {
        model[i] = vector<string>(periods + 1, " &");
        model[i][periods] = "\\\\";
    }

    vector<vector<int>> v(count);
    v[0].push_back(periods);
    v[0].push_back(0);
    model[periods][0] = to_string(vec[0]) + " " + model[periods][0];

    cout << " \\begin{tikzpicture}[>=stealth,sloped] \n \
        \\matrix (tree) [% \n \
            matrix of nodes, \n \
            minimum size=0.1cm,\n \
            column sep=0.1cm,\n \
            row sep=0.5cm,\n \
        ] \n \
        {" <<endl;

    for (int i = 1; i < count; i++)
    {

        int flag = i % 2;
        if (flag == 1)
        {
            v[i].push_back(v[(i - flag) / 2][0] + 1);
            v[i].push_back(v[(i - flag) / 2][1] + 1);
        }
        else
        {
            v[i].push_back(v[(i + flag) / 2 - 1][0] - 1);
            v[i].push_back(v[(i + flag) / 2 - 1][1] + 1);
        }

        if(model[v[i][0]][v[i][1]].size() == 2)
            model[v[i][0]][v[i][1]] = to_string(vec[i]) + " " + model[v[i][0]][v[i][1]];
    }

    for (int i = 0; i < 2 * periods + 1; i++)
    {
        for (int j = 0; j < periods + 1; j++)
        {
            cout << model[i][j] << " ";
        }
        cout << endl;
    }

    cout << "};"<<endl;;

    list<pair<int, int>> l;
    l.push_back(pair<int, int>(periods + 1, 1));
    int now_j = 0;

    while (!l.empty())
    {
        int i = l.front().first;
        int j = l.front().second;

        l.pop_front();

        cout << format(i, j, i - 1, j + 1) << endl;
        cout << format(i, j, i + 1, j + 1) << endl;

        if (j < periods)
        {
            if (now_j < j)
            {
                l.push_back(pair<int, int>(i - 1, j + 1));
                now_j = j;
            }
            l.push_back(pair<int, int>(i + 1, j + 1));
        }
    }

    cout << "\\end{tikzpicture}" <<endl;
}
