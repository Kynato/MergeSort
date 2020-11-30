#include "fx.hpp"
#include "record.hpp"

int main()
{
    // Stale
    unsigned int splitCount = 2;
    unsigned int packageLength = 1;
    unsigned int generatedAmount = 1000000;
    unsigned int tries = 10;

    vector<Record> records;

    for (int i=1000; i<=1000; i+=1000)
    {
        cout << "size=" << i << endl;
        for (int j=2; j<=20; j++)
        {
            cout << "division=" << j << endl;
            double execAvg = 0;
            double ioAvg = 0;
            for (int k=0; k<tries; k++)
            {
                /// i - generatedAmount , j - splitCount
                // Wygeneruj losowe dane
                GenerateRandomInput(i, "unsorted.txt");

                /// MERGE SORT
                StartCounter();
                MergeSort(j, packageLength, i);
                double execTime = GetCounter();
                execAvg += execTime;
                ioAvg += iloscOperacji;
                /// KONIEC MERGE SORT
            }
            execAvg /= tries;
            ioAvg /= tries;
            records.push_back(Record(i, j, ioAvg, execAvg));
        }
    }

    for (int i=0; i<records.size(); i++)
    {
        records[i].print();
    }

    // Porzadkowe funkcje
    RenameFile("files/unsorted.txt", "files/sorted.txt");
    Cleanup(splitCount);

    return 0;
}
