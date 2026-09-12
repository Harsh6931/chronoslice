#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;
void updateStastics(int &status,int &latency,long long &bandwidth,int &code_2xx,int &code_4xx,int &code_5xx,long long &total_bandwith,vector<int> &latencies){
    if(status>=200 && status<300){
        code_2xx++;
    }
    else if(status>=400 && status<500){
        code_4xx++;
    }
    else if(status>=500 && status<600){
        code_5xx++;
    }
    total_bandwith+=bandwidth;
    latencies.push_back(latency);

}
// using const as string line cant change
void parseLine(const string &line,int &status,int &latency,long long &bandwidth){
    stringstream ss(line);
    string token;
    int count=0;
    while(ss >> token){
        if(count==8){
            status=stoi(token);
        }
        else if(count==9){
            bandwidth=stoll(token);  //stoll as long long
        }
        else if(count==10){
            latency=stoi(token);
        }
        count++;


    }
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
    int code_2xx = 0;
    int code_4xx = 0;
    int code_5xx = 0;
    long long total_bandwith=0;
    //store latency
    vector<int> latencies;
    while (getline(file, line)){
        int status;
        int latency;
        long long bandwidth;
        parseLine(line,status,latency,bandwidth);
        
        updateStastics(status,latency,bandwidth,code_2xx,code_4xx,code_5xx,total_bandwith,latencies);
    }
    sort(latencies.begin(),latencies.end());
    cout << "2xx: " << code_2xx << endl;
    cout << "4xx: " << code_4xx << endl;
    cout << "5xx: " << code_5xx << endl;
    cout << "Total Bandwidth: " << total_bandwith << endl;
    int p95_index=ceil(latencies.size()*0.95);
    cout<<"P95 index :"<<p95_index<<endl;
    cout<<"P95 latency :"<<latencies[p95_index-1]<<endl;

    return 0;
}