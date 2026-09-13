#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include<charconv>
#include <chrono>
#include <string_view>

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
    if(entry.status==0) return ; //igonore the bad lines
    if(entry.bandwidth < 0 || entry.latency < 0){  // igonore line if bandwith|latency =-ve
        return;
    }
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
logEntry parseLine(string_view line){         
    size_t pos=0; // starting position for parsing
    int token_count=0;
    logEntry entry{};
    while(pos<line.size()){
        string_view token = nextToken(line,pos);

        if(token_count==8){
            // directly convert string_view to INT
            // used from_chars() as previously stoi(string(token)) first created a extra string
            auto result = from_chars(token.data(),token.data()+token.size(),entry.status);
            if(result.ec != std::errc{}){
                return {};
            }
        }
        else if(token_count==9){
            auto result = from_chars(token.data(),token.data()+token.size(),entry.bandwidth);
            if(result.ec != std::errc{}){
                return {};
            }        
        }

        else if(token_count==10){
            auto result = from_chars(token.data(),token.data()+token.size(),entry.latency);
            if(result.ec != std::errc{}){
                return {};
            }        
        }
        token_count++;

    }
    if(token_count < 11){
        return {};
    }

    return entry;
}

void processLine(string_view line, statistics &stats){

    logEntry entry = parseLine(line);

    updateStastics(entry, stats);
}

int main() {
    //starting timer
    auto start = chrono::high_resolution_clock::now();
    
    ifstream file("../data/access.log");

    if(!file){
        cout << "Failed to open log file\n";
        return 1;
    }

    statistics stats;  // default initialization of statistics struct

const int BUFFER_SIZE = 64*1024;
char buffer[BUFFER_SIZE];

string leftover = "";

while(file.read(buffer, BUFFER_SIZE) || file.gcount() > 0){

    streamsize bytesRead = file.gcount();

    // If there is an incomplete line from the previous chunk
    if(!leftover.empty()){

        leftover.append(buffer, bytesRead);

        size_t lineStart = 0;

        for(size_t i = 0; i < leftover.size(); i++){

            if(leftover[i] == '\n'){

                string_view line(
                    leftover.data() + lineStart,
                    i - lineStart
                );

                processLine(line, stats);

                lineStart = i + 1;
            }
        }

        // Keep incomplete part
        leftover = leftover.substr(lineStart);

        // The entire current buffer has been added to leftover,
        // so don't process buffer separately.
        continue;
    }

    // No leftover → process buffer directly

    size_t lineStart = 0;

    for(size_t i = 0; i < bytesRead; i++){

        if(buffer[i] == '\n'){

            string_view line(
                buffer + lineStart,
                i - lineStart
            );

            processLine(line, stats);

            lineStart = i + 1;
        }
    }

    // Save incomplete line
    if(lineStart < bytesRead){

        leftover = string(
            buffer + lineStart,
            bytesRead - lineStart
        );
    }
}

// Process final line
if(!leftover.empty()){

    processLine(leftover, stats);
}

if(!stats.latencies.empty()){

    size_t p95_index =static_cast<size_t>(ceil(stats.latencies.size()*0.95))-1;
    nth_element(stats.latencies.begin(),stats.latencies.begin()+p95_index,stats.latencies.end());
    cout<<"P95 latency: "<<stats.latencies[p95_index]<<endl;
}

else{
    cout << "No latency data available." << endl;
}

    cout << "2xx: " << stats.code_2xx << endl;
    cout << "4xx: " << stats.code_4xx << endl;
    cout << "5xx: " << stats.code_5xx << endl;
    cout << "Total Bandwidth: " << stats.total_bandwidth << endl;

    //ending timer & printing performace
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(
        end - start
    );
    cout << "Time: " << duration.count() << " ms\n";
    return 0;
}