#include <bits/stdc++.h>
#include <string_view>
using namespace std;

string_view nextToken(string_view line, size_t& pos) {
    size_t space = line.find(' ', pos);

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
int main() {
    string_view line = "hello world rest";
    size_t pos = 0;
    while (pos < line.size()) {
        string_view token = nextToken(line, pos);
        cout << token << endl;
    }
    return 0;
}