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


# dont declare a variable each time, use a accumulator

Imagine a 1 GB log containing millions of requests.

We could do:

request 1 → 5321 → add to total
request 2 → 4210 → add to total
request 3 → 1200 → add to total
...

We only need:

long long totalBytes = 0;

and repeatedly:

totalBytes += bytes;

This is called an accumulator.

You've probably used this idea in LeetCode, but here we're applying it to a continuous stream of real data.

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
    while (getline(file, line))
    {
        int count = 0;

        string token;
        stringstream ss(line);

        while (ss >> token && count<=10)
        {
            if(count==9)
            {
                total_bandwith += stoll(token);
            }
            else if (count == 8)
            {
                int status;
                status = stoi(token);
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
            count++;
        }
    }
    cout << "2xx: " << code_2xx << endl;
    cout << "4xx: " << code_4xx << endl;
    cout << "5xx: " << code_5xx << endl;
    cout << "Total Bandwidth: " << total_bandwith << endl;

    return 0;
}

# What is P95?

Suppose your server receives these requests:

10 ms
20 ms
30 ms
40 ms
50 ms
60 ms
70 ms
80 ms
90 ms
100 ms

If we ask:

What's the average?

We calculate:

55 ms

But average doesn't tell the whole story.

Imagine:

10
11
12
13
14
15
16
17
18
1000

Average:

112.6 ms

That sounds terrible.

But actually 9 out of 10 requests finished in 18 ms or less. One request took 1000 ms.

That's why systems engineers often care about percentiles.

What does P95 mean?

P95 means:

The latency value below which approximately 95% of requests fall.

Imagine 100 requests sorted from fastest to slowest:

1st   2nd   3rd  ...  95th  ... 100th
 ↓     ↓     ↓       ↓          ↓
fast                    P95      slow

If:

P95 = 250 ms

we can say:

About 95% of requests completed within 250 ms.

That's much more useful for understanding user experience.

# Calculate P95

Suppose after sorting we have:

[15, 21, 32, 67, 150]

There are:

n = 5

A simple percentile position calculation is:

P95 position = 0.95 × (n - 1)

So:

0.95 × (5 - 1)
= 3.8

That gives us a position between:

index 3 → 67
index 4 → 150

For our first simple implementation, we'll use the nearest rank approach instead. This is easier and very common for a first implementation.

Nearest-rank formula
position = ceil(0.95 × n)

For 5 values:

ceil(0.95 × 5)
= ceil(4.75)
= 5

So P95 is:

150 ms
But let's make the dataset bigger

With only 5 requests, P95 isn't very meaningful.

Let's imagine 20 sorted latencies:

10
12
15
17
20
21
25
28
30
32
35
40
43
47
50
55
60
70
90
150

n = 20

position = ceil(0.95 × 20)
         = ceil(19)
         = 19

Therefore:

P95 = 90 ms

Meaning approximately 95% of requests took 90 ms or less.

# Your program now does:

                  access.log
                      │
                      ▼
                getline()
                      │
                      ▼
                one log line
                      │
                      ▼
                 stringstream
                      │
             ┌────────┼─────────┐
             ▼        ▼         ▼
          status    bytes     latency
             │        │         │
             ▼        ▼         ▼
          2xx/4xx   total     vector
             │                  │
             ▼                  ▼
          counters             sort
                                │
                                ▼
                               P95

That's a real data-processing pipeline.

But now comes an important part

I don't want us to immediately add more features.

Let's look critically at what we've written.

Your parser currently relies on:

count == 8
count == 9
count == 10

That works because your current format is:

IP
-
-
timestamp
timezone
method
path
HTTP version
status
bytes
latency

But imagine tomorrow your log becomes:

192.168.1.1 - - [date +0000] "GET /api HTTP/1.1" 200 1024 15 extra

or:

192.168.1.1 - - [date +0000] "POST /api HTTP/2" 201 2048 20

Our parser is tightly coupled to token positions.

That's not necessarily bad for a first prototype.

In fact, this is exactly what I wanted you to do:

Get a simple working version first.

Now we're going to start thinking like project developers.

# Phase 2: Refactoring

Before introducing string_view, I want you to learn separation of responsibilities.

Right now main() is doing everything:

main()
 ├── open file
 ├── read file
 ├── parse line
 ├── identify status
 ├── calculate bandwidth
 ├── collect latency
 ├── sort
 └── calculate P95

That's going to become difficult to maintain.

Eventually we'll have something like:

main.cpp
    ↓
LogParser
    ↓
Statistics

For example:

LogParser
    └── parse(line)

Statistics
    ├── addRequest(...)
    ├── totalBandwidth()
    ├── statusCounts()
    └── p95Latency()

This is your first introduction to project architecture.

Your next step isn't coding yet

I want you to understand one concept first:

What should main() be responsible for?

Ideally, something close to:

1. Open the file
2. Read lines
3. Give each line to the parser
4. Give parsed data to statistics
5. Print final results

Instead of knowing how a log is parsed.

That's called separation of concerns.

So your next mini-task

Look at your current code and identify these three responsibilities:

A. File handling

ifstream
getline

B. Parsing

stringstream
token
stoi
stoll

C. Statistics

code_2xx
code_4xx
code_5xx
total_bandwidth
latency
P95