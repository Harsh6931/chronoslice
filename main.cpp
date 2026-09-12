#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct logEntry {
    int status;
    long long bandwidth;
    int latency;



};

struct statistics {
    int code_2xx;
    int code_4xx;
    int code_5xx;
    long long total_bandwidth;
    vector<int> latencies; //store latency

};

void updateStastics(const logEntry &entry, statistics &stats){
    if(entry.status>=200 && entry.status<300){
        stats.code_2xx++;
    }
    else if(entry.status>=400 && entry.status<500){
        stats.code_4xx++;
    }
    else if(entry.status>=500 && entry.status<600){
        stats.code_5xx++;
    }
    stats.total_bandwidth+=entry.bandwidth;
    stats.latencies.push_back(entry.latency);

}
// using const as string line cant change
logEntry parseLine(const string &line){
    stringstream ss(line);
    string token;
    int count=0;
    logEntry entry;
    while(ss >> token){
        if(count==8){
            entry.status=stoi(token);
        }
        else if(count==9){
            entry.bandwidth=stoll(token);  //stoll as long long
        }
        else if(count==10){
            entry.latency=stoi(token);
        }
        count++;
    }
    return entry;
}

int main()
{
    ifstream file("../data/access.log");

    if (!file)
    {
        cout << "Failed to open log file\n";
        return 1;
    }

    string line;
    statistics stats={0,0,0,0,{}};  // default initialization of statistics struct

    while (getline(file, line)){
        logEntry entry = parseLine(line);
        updateStastics(entry,stats);
    }
    sort(stats.latencies.begin(),stats.latencies.end());
    cout << "2xx: " << stats.code_2xx << endl;
    cout << "4xx: " << stats.code_4xx << endl;
    cout << "5xx: " << stats.code_5xx << endl;
    cout << "Total Bandwidth: " << stats.total_bandwidth << endl;
    int p95_index=ceil(stats.latencies.size()*0.95);
    cout<<"P95 index :"<<p95_index<<endl;
    cout<<"P95 latency :"<<stats.latencies[p95_index-1]<<endl;

    return 0;
}