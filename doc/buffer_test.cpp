#include <iostream>
#include <fstream>

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

const int BUFFER_SIZE = 64 * 1024;
char buffer[BUFFER_SIZE];
file.read(buffer, BUFFER_SIZE);

streamsize bytesRead = file.gcount();

cout << "Bytes read: " << bytesRead << endl;

cout.write(buffer, bytesRead);
return 0;
}