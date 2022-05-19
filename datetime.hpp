#pragma once
#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>
using namespace std;

const string months[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
const string days[7] = { "Sat", "Sun" , "Mon", "Tue", "Wed", "Thu", "Fri" };

string zellersAlgorithm(int day, int month, int year) 
{
    int mon;
    if (month > 2)
        mon = month; //for march to december month code is same as month
    else {
        mon = (12 + month); //for Jan and Feb, month code will be 13 and 14
        year--; //decrease year for month Jan and Feb
    }
    int y = year % 100; //last two digit
    int c = year / 100; //first two digit
    int w = (day + floor((13 * (mon + 1)) / 5) + y + floor(y / 4) + floor(c / 4) + (5 * c));
    w = w % 7;
    return days[w];
}

namespace DateTime {

    string stringify(int input)
    {
        string output = to_string(input);
        if (input < 10)
            output = "0" + output;
        return output;
    }

    struct date
    {
        int day = 0, month = 0, year = 0;
        string dayOfWeek = ""; //Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday

        void parseStandardDate(string inputData)
        {
            //Input data must be in the form dd/mm/yyyy or dd.mm.yyyy
            string temp;
            //GETS DAY
            temp = "";
            temp = temp + inputData[0] + inputData[1];
            day = stoi(temp);
            //GETS MONTH
            temp = "";
            temp = temp + inputData[3] + inputData[4];
            month = stoi(temp);
            //GETS YEAR
            temp = "";
            temp = temp + inputData[6] + inputData[7] + inputData[8] + inputData[9];
            year = stoi(temp);
            //GETS DAY OF WEEK
            dayOfWeek = zellersAlgorithm(day, month, year);
        }

        void incrementDay()
        {
            int maxVal = 0;
            if (month == 2)
            {
                if (year % 400 == 0)
                    maxVal = 29;
                else if (year % 100 == 0)
                    maxVal = 28;
                else if (year % 4 == 0)
                    maxVal == 29;
                else
                    maxVal == 28;
            }
            else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
                maxVal = 31;
            else if (month == 4 || month == 6 || month == 9 || month == 11)
                maxVal = 30;

            if (day < maxVal)
                day++;
            else
            {
                day = 1;
                if (month < 12)
                    month++;
                else
                {
                    month = 1;
                    year++;
                }
            }
            dayOfWeek = zellersAlgorithm(day, month, year);
        }
    };

    struct time
    {
        int hour = 0, minute = 0, second = 0; //00-24, 00-60, 00-60;

        void parseStandardTime(string inputData) //data must be in the format HHMM or HHMMSS
        {
            string temp;
            //GETS HOUR
            temp = "";
            temp = temp + inputData[0] + inputData[1];
            hour = stoi(temp);
            //GETS MINUTE
            temp = "";
            temp = temp + inputData[2] + inputData[3];
            minute = stoi(temp);
            //IF EXISTS, GETS SECONDS - UNIMPLEMENTED
        }
    };

    string toTZtimestamp(time time, date date)
    {
        string tztimestamp;
        tztimestamp = tztimestamp + to_string(date.year) + stringify(date.month) + stringify(date.day) + "T" + stringify(time.hour) + stringify(time.minute) + stringify(time.second) + "Z";
        return tztimestamp;
    }

    date dateNow()
    {
        std::time_t now = std::time(0);
        string nowString = ctime(&now);
        date dateNow;
        dateNow.day = stoi(nowString.substr(8, 2));
        dateNow.dayOfWeek = nowString.substr(0, 2);
        dateNow.year = stoi(nowString.substr(20, 4));
        string monthTemp = nowString.substr(4,3);
        for (int i = 0; i < 12; i++)
        {
            if (monthTemp == months[i])
                dateNow.month = i + 1;
        }

        return dateNow;
    }

    time timeNow()
    {
        std::time_t now = std::time(0);
        string nowString = ctime(&now);

        time timeNow;
        timeNow.hour = stoi(nowString.substr(11, 2));
        timeNow.minute = stoi(nowString.substr(14, 2));
        timeNow.second = stoi(nowString.substr(17, 2));

        return timeNow;
    }

    time convert(time time)
    {
        time.hour -= 5;
        time.minute -= 30;

        if (time.minute < 0)
        {
            time.minute = 60 + time.minute;
            time.hour--;
        }
        
        if (time.hour < 0)
            time.hour = 24 + time.hour;
        return time;
    }

}