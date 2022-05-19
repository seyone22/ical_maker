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
    //STRUCTS CONTAINING THE DATA FOR THE CALENDAR ITSELF
    struct vEvent
    {
        string uid = "", cn = "", mailto = "", summary = "", geo = "", description = "", eventClass = "PUBLIC", location;
        tz dtstamp, dtstart, dtend, last_modified;
        string recurrance;

        string constructEvent()
        {
            string eventString = "";
            eventString = eventString + "\nBEGIN:VEVENT\nUID:" + uid + "\nDTSTAMP:" + dtstamp + "\nORGANIZER;CN=" + cn + ":MAILTO:" + mailto + "\nLAST-MODIFIED:" + last_modified + "\nDTSTART:" + dtstart + "\nDTEND:" + dtend + "\nSUMMARY:" + summary + "\nDESCRIPTION:" + description + "\nCLASS:" + eventClass + "\nLOCATION:" + location + "\n" + recurrance + "\nEND:VEVENT";
            return eventString;
        }

        void setRecurrance(string recurranceEnd, string recurranceFreq = "WEEKLY")
        {
            recurrance = "RRULE:FREQ=" + recurranceFreq + ";UNTIL=" + recurranceEnd;
        }
    };


    struct vCalendar
    {
        string version = "2.0", prodid = "-//test", method = "PUBLISH";
        vector<vEvent> events;

        void addEvent(string summary, string lastmod, string dtstamp, string dtstart, string dtend, string recurranceEnd, string venue)
        {
            vEvent newEvent;

            //adding data to event
            //newEvent.uid
            //newEvent.cn
           // newEvent.mailto
            //neweEvent.geo
            //newEvent.description

            newEvent.setRecurrance(recurranceEnd);
            newEvent.location = venue;

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