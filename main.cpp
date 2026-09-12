#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main() {

    ifstream file("../data/access.log");

    if (!file) {
        cout << "Failed to open log file\n";
        return 1;
    }
    string line;
    while(getline(file,line)){
        string ip;
        stringstream ss(line);
        ss>>ip;
        cout<<ip<<endl;
    }

    return 0;
}