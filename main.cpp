#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int main()
{

    ifstream file("../data/access.log");

    if (!file)
    {
        cout << "Failed to open log file\n";
        return 1;
    }
    string line;
    int code_2xx = 0;
    int code_4xx = 0;
    int code_5xx = 0;
    long long total_bandwith=0;
    //store latency
    vector<int> latency;
    while (getline(file, line))
    {
        int count = 0;

        string token;
        stringstream ss(line);

        while (ss >> token && count<=10)
        {
            if(count==9)
            {
                total_bandwith += stoll(token);
            }
            else if (count == 8)
            {
                int status;
                status = stoi(token);
                if (status >= 200 && status <= 299)
                {
                    code_2xx++;
                }
                else if (status >= 400 && status <= 499)
                {
                    code_4xx++;
                }
                else if (status >= 500 && status <= 599)
                {
                    code_5xx++;
                }
            }
            else if(count==10)
            {
                latency.push_back(stoi(token));
            }
            count++;
        }
    }
    sort(latency.begin(),latency.end());
    cout << "2xx: " << code_2xx << endl;
    cout << "4xx: " << code_4xx << endl;
    cout << "5xx: " << code_5xx << endl;
    cout << "Total Bandwidth: " << total_bandwith << endl;
    int p95_index=ceil(latency.size()*0.95);
    cout<<"P95 index :"<<p95_index<<endl;
    cout<<"P95 latency :"<<latency[p95_index-1]<<endl;

    return 0;
}