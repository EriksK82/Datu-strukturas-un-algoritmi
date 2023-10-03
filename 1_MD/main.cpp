#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <string>

using namespace std;

class CSVProcessor {
public:
    CSVProcessor(const string& inputFileName, const string& outputFileName)
        : inputFileName_(inputFileName), outputFileName_(outputFileName) {}

    void process() {
        if (readData()) {
            sortData();
            writeSortedData();
            writeElapsedTime();
            displaySortingTime();
        }
    }


private:
    string inputFileName_;
    string outputFileName_;
    vector<string> data;
    chrono::milliseconds elapsedTime;

    bool readData() {
        ifstream file(inputFileName_);
        if (!file.is_open()) {
            cerr << "Failed to open the CSV file." << endl;
            return false;
        }

        string line;
        while (getline(file, line)) {
            data.push_back(line);
        }
        file.close();

        return true;
    }

    void sortData() {
        auto startTime = chrono::steady_clock::now();
        sort(data.begin(), data.end());
        auto endTime = chrono::steady_clock::now();
        elapsedTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    }

    void writeSortedData() {
        ofstream sortedFile(outputFileName_);
        if (!sortedFile.is_open()) {
            cerr << "Failed to create the sorted CSV file." << endl;
            return;
        }

        for (const string& record : data) {
            sortedFile << record << endl;
        }
        sortedFile.close();
    }

    void writeElapsedTime() {
        ofstream timeFile("sorting_time.csv");
        if (!timeFile.is_open()) {
            cerr << "Failed to create the time CSV file." << endl;
            return;
        }

        timeFile << "Elapsed Time (milliseconds)" << endl;
        timeFile << elapsedTime.count() << endl;
        timeFile.close();
    }

    void displaySortingTime() {
        cout << "Sorting time: " << elapsedTime.count() << " milliseconds." << endl;
    }
};

int main() {
    CSVProcessor processor("initial_data.csv", "sorted_a.csv");
    processor.process();

    return 0;
}