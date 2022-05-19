#include <iostream>

#include <vcalendar.hpp>
#include <datetime.hpp>
#include <timetable_handler.hpp>

//IO FUNCTIONS
string getInput(string description)
{
    cout << description << endl << ">> ";
    string input;
    cin >> input;
    return input;
}

int main()
{
    TimeTable timetable;
    timetable.fileName = getInput("Please enter the name of the file. NOTE:FILE MUST BE IN THE SAME DIRECTORY, AND BE CSV OF A VALID FORMAT");
    timetable.parseCSV();

    string startDate, endDate;
    startDate = getInput("Please enter the day you want the calendar to begin printing");
    endDate = getInput("Please enter the date you want to end printing (default 6mo)");

    vcalendar::vCalendar newCalendar;
    string now = DateTime::toTZtimestamp(DateTime::convert(DateTime::timeNow()), DateTime::dateNow());
    DateTime::date eventDay, startDay; // needs to be calculated per events[i]
    startDay.parseStandardDate(startDate);
    eventDay = startDay;

    if(startDay.dayOfWeek == timetable.events[1].day)// same format?? Mon. if matches, eventday = startday, print. after full run, eventday++. check.
    //worry about date conversion earlier than 5 am
    for (int i = 0; i < timetable.events.size(); i++)
    {
        cout << "entering loop";
        newCalendar.addEvent(timetable.events[i].name, now, now, DateTime::toTZtimestamp(DateTime::convert(timetable.events[i].startTime), eventDay), DateTime::toTZtimestamp(DateTime::convert(timetable.events[i].endTime), eventDay));
        newCalendar.printEvent(i);
    }

    newCalendar.generateFile();

    return 0;
}