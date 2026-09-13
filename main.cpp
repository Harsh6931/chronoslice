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
    int code_2xx = 0;
    int code_4xx = 0 ;
    int code_5xx = 0;
    long long total_bandwidth=0;
    vector<int> latencies={}; //store latency

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

string_view nextToken(const string_view &line, size_t &pos){
    size_t space=line.find(' ',pos);

    // Last token
    if (space == string_view::npos) {
        string_view token = line.substr(pos);
        pos = line.size();
        return token;
    }
    // Token before the space
    string_view token = line.substr(pos, space - pos);

    // Move position to the next token
    pos = space + 1;
    return token;
}




// using const as string line cant change
// represent one request in log file
logEntry parseLine(const string &s){         
    string_view line=s;
    size_t pos=0; // starting position for parsing
    int token_count=0;
    logEntry entry{};
    while(pos<line.size()){
        string_view token = nextToken(line,pos);

        if(token_count==8){
            entry.status=stoi(string(token)); // convert string_view to string and then to int
        }
        else if(token_count==9){
            entry.bandwidth=stoll(string(token));  // convert string_view to string and then to long long
        }
        else if(token_count==10){
            entry.latency=stoi(string(token));  // convert string_view to string and then to int
        }
        token_count++;

    }
    return entry;
}

int main() {
    ifstream file("../data/access.log");

    if(!file){
        cout << "Failed to open log file\n";
        return 1;
    }

    string line;
    statistics stats;  // default initialization of statistics struct

    while (getline(file, line)){
        logEntry entry = parseLine(line);
        updateStastics(entry,stats);
    }

    sort(stats.latencies.begin(),stats.latencies.end());
    
    int p95_index=0;

    if(!stats.latencies.empty()){
        p95_index=ceil(stats.latencies.size()*0.95);
    }
    else{
        cout<<"No latency data available."<<endl;
    }

    cout << "2xx: " << stats.code_2xx << endl;
    cout << "4xx: " << stats.code_4xx << endl;
    cout << "5xx: " << stats.code_5xx << endl;
    cout << "Total Bandwidth: " << stats.total_bandwidth << endl;
        
    cout<<"P95 index :"<<p95_index<<endl;
    cout<<"P95 latency :"<<stats.latencies[p95_index-1]<<endl;

    return 0;
}