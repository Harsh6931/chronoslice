#include <fstream>
#include <string>

using namespace std;

int main() {

    string lines[] = {
        "192.168.1.1 - - [11/Sep/2026:13:55:36 +0000] \"GET /api/v1/products HTTP/1.1\" 200 1024 15\n",
        "192.168.1.2 - - [11/Sep/2026:13:55:37 +0000] \"POST /api/v1/login HTTP/1.1\" 404 512 8\n",
        "192.168.1.3 - - [11/Sep/2026:13:55:38 +0000] \"GET /api/v1/products HTTP/1.1\" 200 2048 22\n",
        "192.168.1.1 - - [11/Sep/2026:13:55:39 +0000] \"GET /api/v1/cart HTTP/1.1\" 500 128 120\n",
        "192.168.1.4 - - [11/Sep/2026:13:55:40 +0000] \"GET /index.html HTTP/1.1\" 200 4096 5\n"
    };

    const long long TARGET_SIZE = 100LL * 1024 * 1024;

    ofstream file("data/access_large.log");

    long long size = 0;

    while(size < TARGET_SIZE) {

        for(string line : lines) {

            file << line;
            size += line.size();

            if(size >= TARGET_SIZE)
                break;
        }
    }

    file.close();

    return 0;
}