#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {

    ifstream file("../../data/access.log", ios::binary);

    if(!file){
        cout << "Failed to open file\n";
        return 1;
    }

    file.seekg(0, ios::end);
    cout << "File size: " << file.tellg() << " bytes\n";
    file.seekg(0, ios::beg);

    const int BUFFER_SIZE = 10;//my buffers size
    char buffer[BUFFER_SIZE];

    string leftover = "";

    while(file.read(buffer, BUFFER_SIZE) || file.gcount() > 0){

        streamsize bytesRead = file.gcount();

        cout << "Bytes read: " << bytesRead << endl;

        // Combine leftover from previous chunk
        // with current buffer
        string current = leftover;
        current.append(buffer, bytesRead);

        leftover.clear();

        size_t lineStart = 0;

        // Find complete lines
        for(size_t i = 0; i < current.size(); i++){

            if(current[i] == '\n'){

                string line = current.substr(lineStart, i - lineStart);

                cout << line << endl;

                lineStart = i + 1;
            }
        }

        // Save incomplete line for next chunk
        leftover = current.substr(lineStart);
    }

    // Handle final line if it doesn't end with '\n'
    if(!leftover.empty()){
        cout << leftover << endl;
    }

    return 0;
}