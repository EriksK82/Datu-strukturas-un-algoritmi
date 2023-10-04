#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <functional>

using namespace std;
using namespace std::chrono;

class CSVSorter {
public:
    CSVSorter(const vector<string>& data) : data_(data) {}

    vector<string> sortAscending(vector<string>& data) {
        vector<string> sortedData = data;
        sort(sortedData.begin(), sortedData.end());
        return sortedData;
    }

    vector<string> sortDescending(vector<string>& data) {
        vector<string> sortedData = data;
        sort(sortedData.rbegin(), sortedData.rend());
        return sortedData;
    }

private:
    vector<string> data_;
};

class CSVProcessor {
public:
    CSVProcessor(const string& inputFileName, const string& outputFileName)
        : inputFileName_(inputFileName), outputFileName_(outputFileName), sorter_(initialdata) {}

    void process() {
        ofstream outputFile(outputFileName_);
        if (!outputFile.is_open()) {
            cout << "Failed to create the output CSV file." << endl;
            return;
        }

        outputFile << "n,Ascending Sorted Time (ms),Descending Sorted Time (ms),Unsorted Time (ms)" << endl;

        if (readData()) {
            vector<int> sizes = {10, 20, 50, 100, 200, 500, 1000, 2000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000};

            for (int size : sizes) {
                vector<string> subData(initialdata.begin(), initialdata.begin() + size);

                double ascendingTime = measureSortingTime([this, &subData]() { return sorter_.sortAscending(subData); });
                double descendingTime = measureSortingTime([this, &subData]() { return sorter_.sortAscending(subData); });
                double unsortedTime = measureSortingTime([this, &subData]() { return sorter_.sortAscending(subData); });

                outputFile << size << "," << ascendingTime << "," << descendingTime << "," << unsortedTime << endl;
            }
        }

        outputFile.close();
    }

private:
    string inputFileName_;
    string outputFileName_;
    vector<string> initialdata;
    CSVSorter sorter_;

    bool readData() {
        ifstream file(inputFileName_);
        if (!file.is_open()) {
            cout << "Failed to open the CSV file." << endl;
            return false;
        }

        string line;
        while (getline(file, line)) {
            initialdata.push_back(line);
        }
        file.close();

        return true;
    }

    template <typename SortFunction>
    double measureSortingTime(SortFunction sortFunction) {
        high_resolution_clock::time_point startTime = high_resolution_clock::now();
        sortFunction();
        high_resolution_clock::time_point endTime = high_resolution_clock::now();
        duration<double, milli> dur = endTime - startTime;
        return dur.count();
    }
};

int main() {
    CSVProcessor processor("initial_data.csv", "output_data.csv");
    processor.process();

    return 0;
}