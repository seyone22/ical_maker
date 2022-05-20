#include <iostream>

#include <vcalendar.hpp>
#include <datetime.hpp>
#include <timetable_handler.hpp>

const DateTime::time midnight;

//IO FUNCTIONS
string getInput(string description)
{
    cout << description << endl << ">> ";
    string input;
    cin >> input;
    return input;
}

//defaults to Zulu Time. datetime.hpp converts to ist. set relevant timezones seperately
// TODO: Make timezones changable using a config file
// TODO: Make event selectable from a list of all events imported through a config file or through CLI.
int main()
{
    initialize();

    TimeTable timetable;
    timetable.fileName = getInput("Please enter the name of the file. NOTE:FILE MUST BE IN THE SAME DIRECTORY, AND BE CSV OF A VALID FORMAT");
    timetable.parseCSV();

    string startDate, endDate;
    startDate = getInput("Please enter the day you want the calendar to begin printing");
    endDate = getInput("Please enter the date you want to end printing");

    vcalendar::vCalendar newCalendar;
    string now = DateTime::toTZtimestamp(DateTime::convert(DateTime::timeNow()), DateTime::dateNow());
    DateTime::date eventDay, startDay, endDay; // needs to be calculated per events[i]
    startDay.parseStandardDate(startDate);
    endDay.parseStandardDate(endDate);
    eventDay = startDay; //TODO: make convert safe for times before 5 am.
    
    for(int i = 0; i < timetable.events.size(); i++)
    {
        while (true)
        {
            if (eventDay.dayOfWeek == timetable.events[i].day)// same format?? Mon. if matches, eventday = startday, print. after full run, eventday++. check.
            {
                newCalendar.addEvent(timetable.events[i].name, now, now, DateTime::toTZtimestamp(DateTime::convert(timetable.events[i].startTime), eventDay), DateTime::toTZtimestamp(DateTime::convert(timetable.events[i].endTime), eventDay),DateTime::toTZtimestamp(midnight,endDay), timetable.events[i].location);
                break;
            }
            else
            {
                eventDay.incrementDay();
            }
        }
        eventDay = startDay;
    }
    newCalendar.generateFile();
    cout << "Please press any key to exit\n";
    int x;
    cin >> x;

    return 0;
}