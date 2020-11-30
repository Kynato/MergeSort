#ifndef RECORD_HPP_INCLUDED
#define RECORD_HPP_INCLUDED

#include <iostream>

class Record
{
    public:
    int length = 0;
    int division = 0;
    double operationsCounter = 0;
    double execTimer;

    Record(int l, int d, double operationcount, double exectime)
    {
        length = l;
        division = d;
        operationsCounter = operationcount;
        execTimer = exectime;
    }

    void print()
    {
        cout << length << " " << division << " " << operationsCounter << " " << execTimer << endl;
    }
};

#endif // RECORD_HPP_INCLUDED
