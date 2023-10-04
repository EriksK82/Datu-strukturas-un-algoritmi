#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm> // Include for reverse

using namespace std;
using namespace std::chrono;

class CSVSorter {
public:
    vector<string> bubbleSortAscending(vector<string>& data) {
        int n = data.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (data[j] > data[j + 1]) {
                    swap(data[j], data[j + 1]);
                }
            }
        }
        return data;
    }
};

class CSVProcessor {
public:
    CSVProcessor(const string& inputFileName, const string& outputFileName)
        : inputFileName_(inputFileName), outputFileName_(outputFileName) {}

    void process() {
        ofstream outputFile(outputFileName_);
        if (!outputFile.is_open()) {
            cout << "Failed to create the output CSV file." << endl;
            return;
        }

        outputFile << "n,Ascending Sorted Time (ms),Descending Sorted Time (ms),Unsorted Time (ms)" << endl; 

        if (readData()) {
            vector<int> sizes = {10, 20, 50, 100, 200, 500, 1000, 2000, 10000, 20000, 50000, 100000};

            CSVSorter sorter;
            for (int size : sizes) {
                vector<string> subData(initialData_.begin(), initialData_.begin() + size);

                // Create a copy for ascending sorting
                vector<string> ascendingData = subData;
                double ascendingTime = measureSortingTime([&sorter, &ascendingData]() {
                    return sorter.bubbleSortAscending(ascendingData);
                });

                // Create a copy for descending sorting
                vector<string> descendingData = subData;
                reverse(descendingData.begin(), descendingData.end()); // Reverse for descending sorting
                double descendingTime = measureSortingTime([&sorter, &descendingData]() {
                    return sorter.bubbleSortAscending(descendingData);
                });

                // Create a copy for unsorted data and sort it
                vector<string> unsortedData = subData;
                double unsortedTime = measureSortingTime([&sorter, &unsortedData]() {
                    return sorter.bubbleSortAscending(unsortedData);
                }); 

                outputFile << size << "," << ascendingTime << "," << descendingTime << "," << unsortedTime << endl;
            }
        }

        outputFile.close();
    }

private:
    string inputFileName_;
    string outputFileName_;
    vector<string> initialData_;

    bool readData() {
        ifstream file(inputFileName_);
        if (!file.is_open()) {
            cout << "Failed to open the CSV file." << endl;
            return false;
        }

        string line;
        while (getline(file, line)) {
            initialData_.push_back(line);
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
    CSVProcessor processor("initial_data.csv", "output_data1.csv");
    processor.process();

    return 0;
}