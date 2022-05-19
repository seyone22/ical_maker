#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <fstream>

using namespace std;
typedef string tz;

namespace vcalendar
{
    //GENERAL FUNCTIONS TO DEAL WITH TIME IN THE TZ TIMESTAMP FORMAT
    string fixTimeNumber(int number) //replace with stringify
    {
        string output = to_string(number);
        if (number < 10)
            output = "0" + output;
        return output;
    }

    tz getTime(int day = 00, int month = 00, int year = 2000, int hour = 00, int minute = 00, int second = 00)
    {
        tz time;
        time = time + to_string(year) + fixTimeNumber(month) + fixTimeNumber(day) + "T" + fixTimeNumber(hour) + fixTimeNumber(minute) + fixTimeNumber(second) + "Z";
        return time;
    }


    //STRUCTS CONTAINING THE DATA FOR THE CALENDAR ITSELF
    struct vEvent
    {
        string uid = "", cn = "", mailto = "", summary = "", geo = "", description = "", eventClass = "PUBLIC", recurranceFreq = "";
        tz dtstamp, dtstart, dtend, last_modified, recurranceEnd;
        string recurrance = "RRULE:FREQ=" + recurranceFreq + ";UNTIL=" + recurranceEnd;

        string constructEvent()
        {
            string eventString = "";
            eventString = eventString + "\nBEGIN:VEVENT\nUID:" + uid + "\nDTSTAMP:" + dtstamp + "\nORGANIZER;CN=" + cn + ":MAILTO:" + mailto + "\nLAST-MODIFIED:" + last_modified + "\nDTSTART:" + dtstart + "\nDTEND:" + dtend + "\nSUMMARY:" + summary + "\nDESCRIPTION:" + description + "\nCLASS:" + eventClass + "\nGEO:" + geo + "\n" + recurrance + "\nEND:VEVENT";
            return eventString;
        }
    };


    struct vCalendar
    {
        string version = "2.0", prodid = "-//test", method = "PUBLISH";
        vector<vEvent> events;

        void addEvent(string summary, string lastmod, string dtstamp, string dtstart, string dtend)
        {
            vEvent newEvent;

            //adding data to event
            newEvent.summary = summary;
            newEvent.last_modified = lastmod;
            newEvent.dtstamp = dtstamp;
            newEvent.dtstart = dtstart;
            newEvent.dtend = dtend;

            events.push_back(newEvent);

        }
    private:
        string constructCalendar()
        {
            string calendarString = "";
            calendarString = calendarString + "BEGIN:VCALENDAR\nVERSION:" + version + "\nMETHOD:" + method + "\nPRODID:" + prodid;
            for (int i = 0; i < events.size(); i++)
            {
                cout << "CONSTRUCTING EVENT\n";
                calendarString = calendarString + events[i].constructEvent();
            }
            cout << "CONSTRUCTING CALENDAR\n";
            calendarString = calendarString + "\nEND:VCALENDAR";
            return calendarString;
        }
    public:
        void generateFile(string filename = "calendar.ics")
        {
            ofstream outputFile;
            outputFile.open(filename, ios_base::trunc);
            if (outputFile)
            {
                cout << "GENERATING FILE\n";
                outputFile << constructCalendar();
            }
            outputFile.close();
            cout << "FILE GENERATED.\n";
        }
    public:
        void printEvent(int i)
        {
            cout << events[i].uid << endl
                << events[i].cn << events[i].mailto << endl
                << events[i].summary << events[i].description << endl
                << events[i].geo << endl
                << events[i].dtstamp << endl
                << events[i].last_modified << endl;
        }
    };
}