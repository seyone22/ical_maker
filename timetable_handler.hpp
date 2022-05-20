#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <filesystem>

#include <datetime.hpp>

using namespace std;

//const string timeSlots[7] = { "08.30AM-09.30AM", "09.30AM-10.30AM", "10.30AM-11.30AM", "11.30AM-12.30PM", "01.30PM-02.30PM", "02.30PM-03.30PM", "03.30PM-4.30PM" };
//const string cleanedStartTimeSlots[7] = { "0830", "0930", "1030", "1130", "1330", "1430", "1530" };
//const string cleanedEndTimeSlots[7] = { "0930", "1030", "1130", "1230", "1430", "1530", "1630" };
//const string venue[7] = { "LH3", "ICTL", "CL-I", "CL-II", "ELTU", "AHEADLAB", "LH1/FBS" };
//const string classes[12] = { "IT1214","IT1214(P)","IT1223","IT1223(P)","IT1232","IT1242","IT1252","IT1252(P)","IT1262","ACU1212","CSC1213(P)/IT1214(P)","CSC1213/IT1214" };

vector<string> Venues;
vector<string> TimeSlots;
vector<string> Classes;

const string daysSP[7] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };

void initialize()
{
    std::filesystem::path f{ "timetable.cfg" };
    if (!std::filesystem::exists(f))
        cout << "No config file found (timetable.cfg)! Please create one according to the given format.\n";
    string line;
    ifstream configFile;
    configFile.open("timetable.cfg", ios::in);
    if (configFile)
    {
        cout << "Checking config file. This is the criteria by which events will be parsed.\n";
        while (!configFile.eof())
        {
            getline(configFile, line, '\n');
            if (line[0] == '#')
                continue;
            if (line[0] == 't')
            {
                cout << "Timeslots:\n";
                while (true)
                {//test
                    getline(configFile, line, '\n');
                    if (line[0] == '}')
                        break;
                    //cout << line << endl;
                    TimeSlots.push_back(line);
                    cout << "\t" << line << endl;
                }
            }
            if (line[0] == 'v')
            {
                cout << "Venues:\n";
                while (true)
                {
                    getline(configFile, line, '\n');
                    if (line[0] == '}')
                        break;
                    Venues.push_back(line);
                    cout << "\t" << line << endl;
                }
            }
            if (line[0] == 'c')
            {
                cout << "Classes:\n";
                while (true)
                {
                    getline(configFile, line, '\n');
                    if (line[0] == '}')
                        break;
                    Classes.push_back(line);
                    cout << "\t" << line << endl;
                }
            }
            line = "";
        }
    }
    else
        cout << "Error opening configuration file!\n";
}


void generateTimeObjects(string toClean, DateTime::time &startTime, DateTime::time &endTime)
{
    string shour, smin, ehour, emin;
    shour = toClean.substr(0, 2);
    smin = toClean.substr(3, 2);
    ehour = toClean.substr(8, 2);
    emin = toClean.substr(11, 2);
    if (toClean.substr(5, 2) == "AM")
    {
        if (shour == "12")
            startTime.hour = (stoi(shour) + 12);
        else
            startTime.hour = stoi(shour);
        startTime.minute = stoi(smin);
    }
    else
    {
        if(shour != "12")
            startTime.hour = (stoi(shour) + 12);
        else
            startTime.hour = stoi(shour);
        startTime.minute = stoi(smin);
    }
    if (toClean.substr(13, 2) == "AM")
    {
        if (ehour == "12")
            endTime.hour = (stoi(ehour) + 12);
        else
            endTime.hour = stoi(ehour);
        endTime.minute = stoi(emin);
    }
    else
    {
        if (ehour != "12")
            endTime.hour = (stoi(ehour) + 12);
        else
            endTime.hour = stoi(ehour);
        endTime.minute = stoi(emin);
    }
    cout << startTime.hour << startTime.minute << " " << endTime.hour << endTime.minute << endl;
}

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
            cout << "Please enter the name of the file. NOTE:FILE MUST BE IN THE SAME DIRECTORY, AND BE CSV OF A VALID FORMAT" << endl << ">> ";
            cin >> fileName;
            stringoutput = readCSV();
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
        string tempVenue;
        DateTime::time startTime, endTime;
        int counter = -7, rowCounter = 1;
        cout << "GENERATING EVENT STRUCTS\n";
        for (string x : tokens)
        {
            //if-else soup time!
            //SETS THE TIME SLOTS
            for (int i = 0; i < TimeSlots.size(); i++)
                if (x == TimeSlots[i])
                    generateTimeObjects(x, startTime, endTime);
            //SET THE VENUE
            for (int i = 0; i < Venues.size(); i++)
                if (x == Venues[i])
                    tempVenue = Venues[i];
            //FINDS THE ELEMENT
            for (int i = 0; i < Classes.size(); i++)
                if (x == Classes[i])
                {
                    double temp = (static_cast <double>(counter) / 7);
                    int tempInt = ceil(temp);
                    tempInt = ((counter - (2 * tempInt)) % 5) - 1;
                    if (tempInt < 0)
                        tempInt = 4;

                    timetableEvent event;
                    event.day = daysSP[tempInt];
                    event.name = Classes[i];
                    event.location = tempVenue;
                    event.startTime = startTime;
                    event.endTime = endTime;
                    events.push_back(event);
                }
            counter++;
        }
        if (events.size() != 0)
            cout << "EVENT STRUCT GENERATION SUCCESSFUL\n";
        else
            cout << "NO SUITABLE EVENTS FOUND!";

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