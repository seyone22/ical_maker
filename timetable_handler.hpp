#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>

#include <datetime.hpp>

using namespace std;

const string timeSlots[7] = { "08.30AM-09.30AM", "09.30AM-10.30AM", "10.30AM-11.30AM", "11.30AM-12.30PM", "01.30PM-02.30PM", "02.30PM-03.30PM", "03.30PM-4.30PM" };
const string cleanedStartTimeSlots[7] = { "0830", "0930", "1030", "1130", "1330", "1430", "1530" };
const string cleanedEndTimeSlots[7] = { "0930", "1030", "1130", "1230", "1430", "1530", "1630" };
const string venue[7] = { "LH3", "ICTL", "CL-I", "CL-II", "ELTU", "AHEADLAB", "LH1/FBS" };
const string classes[12] = { "IT1214","IT1214(P)","IT1223","IT1223(P)","IT1232","IT1242","IT1252","IT1252(P)","IT1262","ACU1212","CSC1213(P)/IT1214(P)","CSC1213/IT1214" };

const string daysSP[7] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };

struct timetableEvent
{
    //Time startTime, endTime;
    string location, name, day;
    DateTime::time startTime, endTime;
};

struct TimeTable
{
    vector<timetableEvent> events;
    vector<string> tokens;
    string fileName;

private:
    string readCSV() // file cannot have whitespaces, tabs, or newlines
    {
        string stringtmp, stringoutput;
        ifstream file;
        file.open(fileName, ios::in);
        if (file)
        {
            cout << "SUCCESSFULLY OPENED FILE\n";
            while (!file.eof())
            {
                file >> stringtmp;
                stringoutput = stringoutput + ',' + stringtmp;
            }
        }
        else
        {
            cerr << "ERROR OPENING FILE\n";
            exit(1); //WEIRD CODE
        }
        file.close();
        return stringoutput;
    }

private:
    void tokenizeCSV() //counts blanks too
    {
        cout << "TOKENIZING CSV...\n";
        stringstream check1(readCSV());
        string intermediate;
        while (getline(check1, intermediate, ','))
        {
            tokens.push_back(intermediate);
        }
        //for(int i = 0; i < tokens.size(); i++)
            //cout << tokens[i] << endl;
    }

public:
    void parseCSV()
    {
        tokenizeCSV();
        cout << "TOKENIZING SUCCESSFUL!\n";
        string tempStartTime, tempEndTime, tempVenue;
        int counter = -7, rowCounter = 1;
        cout << "GENERATING EVENT STRUCTS\n";
        for (string x : tokens)
        {
            //if-else soup time!
            //SETS THE TIME SLOTS
            for (int i = 0; i < 7; i++)
                if (x == timeSlots[i])
                {
                    tempStartTime = cleanedStartTimeSlots[i];
                    tempEndTime = cleanedEndTimeSlots[i];
                }
            //SET THE VENUE
            for (int i = 0; i < 7; i++)
                if (x == venue[i])
                    tempVenue = venue[i];
            //FINDS THE ELEMENT
            for (int i = 0; i < 12; i++)
                if (x == classes[i])
                {
                    double temp = (static_cast <double>(counter) / 7);
                    int tempInt = ceil(temp);
                    tempInt = ((counter - (2 * tempInt)) % 5) - 1;
                    if (tempInt < 0)
                        tempInt = 4;

                    timetableEvent event;
                    event.day = daysSP[tempInt];
                    event.name = classes[i];
                    event.location = tempVenue;
                    event.startTime.parseStandardTime(tempStartTime);
                    event.endTime.parseStandardTime(tempEndTime);
                    events.push_back(event);
                }
            counter++;
        }
        cout << "EVENT STRUCT GENERATION SUCCESSFUL\n";

    }

    void printStructs(int i)
    {
        cout << events[i].name << endl
                << events[i].startTime.hour << events[i].startTime.minute << endl
                << events[i].endTime.hour << events[i].endTime.minute << endl
                << events[i].location << endl
                << events[i].day << endl;
    }
};