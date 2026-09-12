ifstream = used to allow C++ read files

A relative file path depends on where your program is being run from, not where main.cpp is located.
This distinction will become important when we introduce CMake later.

using stringstream to parse a string


# print only the status code in the string at token pos 8

int main() {

    ifstream file("../data/access.log");

    if (!file) {
        cout << "Failed to open log file\n";
        return 1;
    }
    string line;
    while(getline(file,line)){
        string status;
        stringstream ss(line);
        int count=9;
        while(ss>>status && count--){
            if(count==0){
                cout<<status<<endl;

            }
        }
    }

    return 0;
}

# Why use range 2xx ,5xx for codes?
Because HTTP has many successful status codes:

200 OK
201 Created
202 Accepted
204 No Content
...

All of them belong to the 2xx category.

# count frequency of status code

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
    while (getline(file, line))
    {
        int count = 9;

        string statuscode;
        stringstream ss(line);

        while (ss >> statuscode && count--)
        {
            if (count == 0)
            {
                int status;
                status = stoi(statuscode);
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
        }
    }
    cout << "2xx: " << code_2xx << endl;
    cout << "4xx: " << code_4xx << endl;
    cout << "5xx: " << code_5xx << endl;

    return 0;
}

# 1st actual step of log analysis

File → read lines → parse → extract status → categorize → count
