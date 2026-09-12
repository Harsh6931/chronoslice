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