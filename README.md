# Timetable -> .ics

This small program can convert a .csv input into a .ics file ready to be imported into your calendar application of choice!

The code CANNOT be used as-is for any timetable.

## Modifications needed

arrays `timeSlots`, `cleanedStartTimeSlots`, `cleanedEndTimeSlots`, `venue`, and `classes` in `timetable_handler.hpp` need to be updated with data from your timetable.


## Timetable Format

timetable should be in the format described in test.csv. All lines should contain 7 elements. Venue can be kept blank. If the same time is used for multiple events the time can be kept blank for subsequent events as shown in the sample. Time will only be updated when a new time is encountered.

## TODOs


~~TODO: Get the array data from a .config file, so that it is easily modifiable by the user~~

~~TODO: Inform the user if no events matching the criteria are encountered, and halt file generation.~~

~~TODO: Inform the user if the wrong file name is entered, and allow user to retry.~~

TODO: Extend the program to be able to view, edit, and create single .ics events.

TODO: Extend the program to work with different timezones, and include timezone data in the .ics file generated
