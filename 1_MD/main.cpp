#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;


// klase sortēšanai izmantojama datu masīvu sortēšanai pēc nepieciešamības (izsaucot) gan Ascending gan Descending

class CSVSorter {
public:

//Ascending datu masīvu sortēšanas algoritms
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

//Descending datu masīvu sortēšanas algoritms
    vector<string> bubbleSortDescending(vector<string>& data) {
        int n = data.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (data[j] < data[j + 1]) { // < lai apgrieztu visu algoritmu otrādi iegūtu Descending
                    swap(data[j], data[j + 1]);
                }
            }
        }
        return data;
    }
};

// klase sortēšananas uzdevuma veikšanai

class CSVProcessor {
public:
// konstruktoras ar ievades faila nosaukumu un izvades faila nosaukumu
    CSVProcessor(const string& inputFileName, const string& outputFileName)
        : inputFileName_(inputFileName), outputFileName_(outputFileName) {}

//Pats aprēķinu un faila izveides process
    void process() {
        ofstream outputFile(outputFileName_); // izveidojam CSV failu ar mūsu vēlamo nosaukumu
        if (!outputFile.is_open()) {//pārbaudam vai veras vaļā, ja neveras izdodam paziņojumu par kļūdu
            cout << "Failed to create the output CSV file." << endl;
            return;
        }

        outputFile << "n,Ascending Sorted Time (ms),Descending Sorted Time (ms),Unsorted Time (ms)" << endl; //izejas faila kollonu nosaukumi

        if (readData()) {//ja fails atveras un dati lasās skatīt readdata algoritmu zem private sadaļas
            vector<int> sizes = {10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000};//nosakam šķirošanas apjomus

            CSVSorter sorter;
            for (int size : sizes) {//uzsākam secīgu šķirošanas ciklu par parametriem izmantojot "Size"
                vector<string> subData(initialData_.begin(), initialData_.begin() + size);// definējas datu apmēru (n)

                // (Ascending datu) šīrošanas algoritms
                vector<string> ascendingData_ = subData; // pārdefinējam-pieķiram datu fragmentam vērtību
                double ascendingTime = measureSortingTime([&sorter, &ascendingData_]() {// izsaucam laika mērīšanas funkciju, lai nomērītu laiku cik ilgi proces aizņem aktivējas kad izsaucas sortes un nosauktie dati
                sorter.bubbleSortAscending(ascendingData_);// izsaucam iepriekš izveidoto sorteru un norādam uz šķirojamajiem datiem
                });

                // (Decending datu) šīrošanas algoritms
                vector<string> descendingData_ = subData;
                double descendingTime = measureSortingTime([&sorter, &descendingData_]() {
                sorter.bubbleSortAscending(descendingData_);
                });

                // (unsorted datu) šīrošanas algoritms
                vector<string> unsortedData_ = subData;
                double unsortedTime = measureSortingTime([&sorter, &unsortedData_]() {
                sorter.bubbleSortAscending(unsortedData_);
                }); 

                outputFile << size << "," << ascendingTime << "," << descendingTime << "," << unsortedTime << endl;// definējam datu kārtošanas struktūru failā
            }
        }

        outputFile.close();//noslēdzam failu
    }

private:
//definējam 
    string inputFileName_;
    string outputFileName_;
    vector<string> initialData_;
    vector<string> ascendingData_;
    vector<string> descendingData_;
    vector<string> unsortedData_;

// faila atvēršanas un nolasīšanas/pārbaudes algoritms
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

        // izveidoju datu masīvus kurus tālāk lietot;
        CSVSorter ascendingData_;
        ascendingData_.bubbleSortAscending(initialData_);

        CSVSorter descendingData_;
        descendingData_.bubbleSortDescending(initialData_);

        unsortedData_ = initialData_;

        return true;
    }

    //Laika uzskaites algoritms milisekundēs tiek lietots pie laika uzskaites
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
    CSVProcessor processor("initial_data.csv", "output_data.csv");//ievadu izejas datus un vēlamo izejas faila nosaukumu
    processor.process();//palaižu šķirošanas procesu

    

    return 0;
}
