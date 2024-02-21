# TimeLib
TimeLib is a C library for handling Unix timestamps and converting them to human-readable date and time. It also provides functionality to handle timezones.

# Functions
Here's a brief overview of the functions provided by TimeLib:

TTime convertUnixTimestampToTime(unsigned int timestamp): Converts a Unix timestamp to a time structure containing hours, minutes, and seconds.

TDate convertUnixTimestampToDateWithoutLeapYears(unsigned int timestamp): Converts a Unix timestamp to a date structure containing day, month, and year. It does not account for leap years.

TDate convertUnixTimestampToDate(unsigned int timestamp): Similar to the previous function, but this one does account for leap years.

TDateTimeTZ convertUnixTimestampToDateTimeTZ(unsigned int timestamp, TTimezone *timezones, int timezone_index): Converts a Unix timestamp to a date and time structure that also includes timezone information.

unsigned int convertDateTimeTZToUnixTimestamp(TDateTimeTZ datetime): Takes a date and time structure with timezone information and converts it to a Unix timestamp.

void printDateTimeTZ(TDateTimeTZ datetimetz): Takes a date and time structure with timezone information and prints it in a human-readable format.

# Usage
Include timelib.h in your C source file and link against timelib.c during compilation. You can then use the functions provided by TimeLib to handle Unix timestamps and convert them to human-readable date and time.

# Example
Here is an example of how to use TimeLib in a C program. This example is from the file **check16.c**.

This program takes a task number as a command-line argument and performs different operations based on the task number:

Task 1: Reads a Unix timestamp from the input, converts it to time (hours, minutes, seconds), and prints the result.
Task 2: Reads a Unix timestamp from the input, converts it to a date (day, month, year) without considering leap years, and prints the result.
Task 3: Similar to Task 2, but considers leap years in the conversion.
Task 4: Reads a Unix timestamp and a list of timezones from the input, converts the timestamp to a date and time in a specific timezone, and prints the result.
Task 5: Reads a date, time, and a list of timezones from the input, converts the date and time to a Unix timestamp in a specific timezone, and prints the result.
Task 6: Reads a date, time, and a timezone from the input, and prints the date and time in a human-readable format.
The program uses dynamic memory allocation to create an array of timezones. The memory is freed before the program ends.

# Makefile

CFLAGS=-Wall -Werror

.PHONY: clean

check16: check16.c timelib.c
	gcc check16.c timelib.c $(CFLAGS) -o check16

clean:
	rm -f check16
