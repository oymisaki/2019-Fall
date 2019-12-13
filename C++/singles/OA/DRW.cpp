#include <iostream>
#include <map>
#include <vector>

using namespace std;

struct cond
{
    int turb = 1;
    int flag = 0; // 0 for start, 1 for up, -1 for down 
};

int solution(vector<int> &A) {
    // write your code in C++14 (g++ 6.2.0)
    vector<cond> dp = vector<cond>(A.size());
    int now_max = 1;

    for (int i = 1; i < A.size(); i++)
    {
        if(A[i] > A[i-1])
        {
            dp[i].flag = 1;
            dp[i].turb = dp[i-1].flag < 1 ? dp[i-1].turb + 1: 2;
        }
        if(A[i] < A[i-1])
        {
            dp[i].flag = -1;
            dp[i].turb = dp[i-1].flag > -1 ? dp[i-1].turb + 1: 2;
        }
        now_max = now_max < dp[i].turb ? dp[i].turb : now_max;
    }
    
    return now_max;
}

int main()
{
    int test[] = {9, 4, 2, 10, 7, 8, 8, 1, 9};
    vector<int> test_vec{9, 4, 2, 10, 7, 8, 8, 1, 9};
    cout<<solution(test_vec)<<endl;
    return 0;
}