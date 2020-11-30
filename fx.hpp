#ifndef FX_HPP_INCLUDED
#define FX_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <time.h>
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>

using namespace std;

unsigned int iloscOperacji = 0;

bool AreVariablesLegal(const unsigned int splitCount, const unsigned int packageLength, const unsigned int generatedAmount)
{
    return true;
}

fstream AccessFile(string fileName, ios_base::openmode mode = ios_base::in | ios_base::out)
{

    fstream file;
    file.open(fileName, mode);

    if (file.good() == false)
    {
        cout << "Error opening file: " << fileName << endl;
        //system("pause");
    }

    return file;
}

vector<fstream> AccessArrayOfFiles(string nameRoot = "split", const unsigned int splitCount = 2, ios_base::openmode mode = ios_base::in | ios_base::out)
{
    nameRoot = "files/" + nameRoot;
    vector<fstream> filesToReturn;

    for(int i=0; i<splitCount; i++)
    {
        string name = nameRoot + to_string(i) + ".txt";
        filesToReturn.push_back( AccessFile(name, mode) );
    }

    return filesToReturn;
}


void RenameFile(const string oldName, const string newName)
{
    char charOldName[oldName.length()+1];
    strcpy(charOldName, oldName.c_str());

    char charNewName[newName.length()+1];
    strcpy(charNewName, newName.c_str());

    if (rename(charOldName, charNewName) != 0)
    {
        perror("Error renaming file");
    }
    else
    {
        cout << "Changed: " << oldName << " -> " << newName << endl;;
    }
}

void GenerateRandomInput(const unsigned int length, string fileName = "unsorted.txt")
{
    fileName = "files/" + fileName;
    fstream unsorted = AccessFile(fileName, ios::out);

    srand(time(NULL));

    //cout << "Generated numbers:" << endl << "[ ";
    for (int i=0; i<length; i++)
    {
        int rnd = rand()%INT_MAX-1;
        unsorted << rnd << " ";
        //cout << rnd << " ";
    }
    //cout << "]" << endl;

    unsorted << "#";

    unsorted.close();
}

void SplitToFiles(const unsigned int splitCount, const unsigned int packageLength, string inputSource = "unsorted.txt")
{
    //cout << "SPLITTING to: " << splitCount << " files. | LENGTH: " << packageLength << endl;
    inputSource = "files/" + inputSource;

    // Otwieramy unsorted i splittery
    fstream unsorted = AccessFile(inputSource, ios::in);
    vector<fstream> splitter = AccessArrayOfFiles("split", splitCount, ios::out);

    // Rozdzielamy unsorted do splitterow jak na karuzeli
    string str = "";
    int karuzela = 0;
    unsorted >> str;
    iloscOperacji++;
    while(str != "#")
    {
        // Rozdzielaj w pakietach
        for (int i=0; i<packageLength; i++)
        {
            if (str == "|")
            {
                unsorted >> str;
                iloscOperacji++;
                if(str == "#")
                {
                    break;
                }
            }
            splitter[karuzela%splitCount] << stoi(str) << " ";
            iloscOperacji++;
            unsorted >> str;
            iloscOperacji++;
            if(str == "#") break;
        }
        splitter[karuzela%splitCount] << "| ";
        iloscOperacji++;
        karuzela++;
    }

    // Dodaj # na koniec ka¿dego splittera i go zamknij
    for(int i=0; i<splitCount; i++)
    {
        splitter[i] << "#";
        iloscOperacji++;
        splitter[i].close();
    }

    unsorted.close();

    // Wypisz do konsoli, ze zakonczone powodzeniem
    //cout << "--DONE--" << endl << endl;
}

void MergeSplits(const unsigned int splitCount, const unsigned int packageLength, string outputPath = "unsorted.txt")
{
    //cout << "MERGING: " << splitCount << " files. | LENGTH: " << packageLength << endl;
    outputPath = "files/" + outputPath;

    // Otwieramy unsorted i splittery
    vector<fstream> splitter = AccessArrayOfFiles("split", splitCount, ios::in);
    fstream unsorted = AccessFile(outputPath, ios::out);

    // Merge sortuj pakiety i wypisuj z '|' do unsorted.txt
    int ileEndow = 0;
    while(ileEndow < splitCount) // czy koniec wszystkich pakietow
    {
        string str = "";
        int karuzela = 0;
        int ileStopow = 0;
        string number[splitCount];

        // Przypisz pierwsze wartosci i sprawdz znaki specjalne jezeli to pakiety konczace plik
        for(int i=0; i<splitCount; i++)
        {
            splitter[i] >> number[i];
            iloscOperacji++;
            if (number[i] == "|")
            {
                ileStopow++;
                splitter[i] >> number[i];
                iloscOperacji++;

            }
            if(number[i] == "#")
            {
                ileEndow++;
                //cout << "ileEndow: " << ileEndow << endl;

            }
        }
        if (ileEndow >= splitCount) break;

        // Merguj pakiety
        //cout << "Merguje pakiet" << endl;
        while(ileStopow < splitCount) //czy koniec zestawu pakietow
        {
            // reset minimum
            int minimum = INT_MAX;
            int minIndex = 0;
            bool ifBreak = true;

            for(int i=0; i<splitCount; i++)
            {
                if (number[i] != "|" && number[i] != "#")
                {
                    // reset minimum
                    minimum = stoi(number[i]);
                    minIndex = i;
                    ifBreak = false;
                    break;
                }
            }
            if(ifBreak)
            {
                ileEndow++;
                break;
            }

            //cout << "sprawdzam: " << number[0] << endl;
            //znajdz minimum z pionu
            for(int i=0; i<splitCount; i++)
            {
                //cout << "sprawdzam: " << number[i] << endl;
                if (number[i] != "|" && number[i] != "#")
                {
                    if (minimum > stoi(number[i]))
                    {
                        minimum = stoi(number[i]);
                        minIndex = i;
                    }
                }
            }
            // Minimum znalezione pod indexem minIndex

            // Teraz zapisujemy minimum do outputu
            //cout << "minimum: " << minimum << endl;
            unsorted << minimum << " ";
            iloscOperacji++;

            // Ladujemy kolejna liczbe pod miejsce minimum
            splitter[minIndex] >> number[minIndex];
            iloscOperacji++;

            // Sprawdzamy znaki specjalne
            if (number[minIndex] == "|")
            {
                ileStopow++;
            }
            else if(number[minIndex] == "#")
            {
                ileStopow++;
                ileEndow++;
            }
        }
        //cout << endl;
        unsorted << "| ";
    }

    // Zamknij splittery
    for(int i=0; i<splitCount; i++)
    {
        splitter[i].close();
    }

    // Dopisz # do konca unsorted.txt i go zamknij
    unsorted << "#";
    unsorted.close();



    // Wypisz do konsoli, ze zakonczone powodzeniem
    //cout << "--DONE--" << endl << endl;
}

void Cleanup(const unsigned int numberOfFiles, string nameRoot = "split")
{
    nameRoot = "files/" + nameRoot;

    for(int i=0; i<numberOfFiles; i++)
    {
        string name = nameRoot + to_string(i) + ".txt";
        char charName[name.length()+1];
        strcpy(charName, name.c_str());

        if (remove(charName) != 0)
            perror("File deletion failed");
        else
            cout << charName << " deleted successfully" << endl;
    }

    if (remove("files/unsorted.txt") != 0)
        perror("File deletion failed");
    else
        cout << "files/unsorted.txt" << " deleted successfully" << endl;
}

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
    cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart)/1000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
}




unsigned int MergeSort(unsigned int splitCount, unsigned int packageLength, unsigned int generatedAmount)
{
    iloscOperacji = 0;
    while(packageLength < generatedAmount)
    {
        SplitToFiles(splitCount, packageLength,  "unsorted.txt");
        //system("pause");
        MergeSplits(splitCount, packageLength);
        //system("pause");
        packageLength *= splitCount;
    }

    return iloscOperacji;
}


#endif // FX_HPP_INCLUDED
