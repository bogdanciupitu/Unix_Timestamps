#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timelib.h"
#define seven 7
#define ten 10
#define twelve 12
#define twentyfour 24
#define twentyeight 28
#define twentynine 29
#define thirty 30
#define thirtyone 31
#define sixty 60
#define onehundred 100
#define oneyearwithoutleap 365
#define forhundred 400
#define onethousandninehundredseventy 1970
#define eithysixthousandfourhundred 86400

TTime convertUnixTimestampToTime(unsigned int timestamp) {
    TTime result;
    result.sec = timestamp % sixty;
    timestamp /= sixty;
    result.min = timestamp % sixty;
    timestamp /= sixty;
    result.hour = timestamp % twentyfour;
    return result;
}

TDate convertUnixTimestampToDateWithoutLeapYears(unsigned int timestamp) {
    TDate result;
    int nrmonth = 0;
    result.day = 1;
    result.month = 1;
    result.year = onethousandninehundredseventy;
    int oneday = eithysixthousandfourhundred;
    unsigned int nrdays = timestamp / oneday;
    while (nrdays > 0) {
        if (result.month == 2) {
            nrmonth = twentyeight;
        } else {
            nrmonth = thirtyone - ((result.month - 1) % seven % 2);
        }
        if (nrdays >= nrmonth) {
            nrdays -= nrmonth;
            result.month++;
            if (result.month > twelve) {
                result.month = 1;
                result.year++;
            }
        } else {
            result.day += nrdays;
            break;
        }
    }
    return result;
}

TDate convertUnixTimestampToDate(unsigned int timestamp) {
    TDate result;
    result.day = 1;
    result.month = 1;
    result.year = onethousandninehundredseventy;
    unsigned int oneday = eithysixthousandfourhundred;
    unsigned int nrdays = timestamp / oneday;
    while (nrdays > 0) {
    int nrmonth = 0, leapyear = 0;
        if (result.year % 4 == 0 && (result.year % onehundred != 0 || result.year % forhundred == 0)) {
            leapyear = 1;
        } else {
            leapyear = 0;
        }
        if (result.month == 2) {
            if (leapyear != 0) {
                nrmonth = twentynine;
            } else {
                nrmonth = twentyeight;
            }
        } else {
                nrmonth = thirtyone - ((result.month - 1) % seven % 2);
        }
        if (nrdays >= nrmonth) {
            nrdays -= nrmonth;
            result.month++;
            if (result.month > twelve) {
                result.month = 1;
                result.year++;
            }
        } else {
            result.day += nrdays;
            break;
        }
    }
    return result;
}

TDateTimeTZ convertUnixTimestampToDateTimeTZ(unsigned int timestamp, TTimezone *timezones, int timezone_index) {
    TDateTimeTZ result;
    result.date = convertUnixTimestampToDate(timestamp);
    result.time = convertUnixTimestampToTime(timestamp);
    result.time.hour += timezones[timezone_index].utc_hour_difference;
    if (result.time.hour >= twentyfour) {
        result.time.hour -= twentyfour;
        result.date.day++;
        if (result.date.day > thirtyone) {
            result.date.day = 1;
            result.date.month++;
            if (result.date.month > twelve) {
                result.date.month = 1;
                result.date.year++;
            }
        }
    } else if (result.time.hour < 0) {
        result.time.hour += twentyfour;
        result.date.day--;
        if (result.date.day < 1) {
            result.date.month--;
            if (result.date.month < 1) {
                result.date.month = twelve;
                result.date.year--;
            }
            if (result.date.month == 2) {
                if (result.date.year % 4 == 0 && (result.date.year % onehundred != 0 || result.date.year %
                 forhundred == 0)) {
                    result.date.day = twentynine;
                } else {
                    result.date.day = twentyeight;
                }
            } else {
                result.date.day = thirtyone - ((result.date.month - 1) % seven % 2);
            }
        }
    }
    result.tz = &timezones[timezone_index];
    return result;
}

unsigned int convertDateTimeTZToUnixTimestamp(TDateTimeTZ datetime) {
    unsigned int timestamp = 0, i = 0;
    int days_in_month[] = {thirtyone, twentyeight, thirtyone, thirty, thirtyone, thirty,
     thirtyone, thirtyone, thirty, thirtyone, thirty, thirtyone};
    for (i = onethousandninehundredseventy; i < datetime.date.year; i++) {
        timestamp += oneyearwithoutleap + (i % 4 == 0 && (i % onehundred != 0 || i % forhundred == 0) ? 1 : 0);
    }
    for (i = 1; i < datetime.date.month; i++) {
        if (i != 2) {
            timestamp += days_in_month[i - 1];
        } else {
            if (datetime.date.year % 4 != 0 || (datetime.date.year % onehundred == 0 &&
             datetime.date.year % forhundred != 0)) {
                timestamp += twentyeight;
            } else {
                timestamp += twentynine;
            }
        }
    }
    timestamp += datetime.date.day - 1;
    timestamp *= twentyfour;
    timestamp += datetime.time.hour;
    timestamp *= sixty;
    timestamp += datetime.time.min;
    timestamp *= sixty;
    timestamp += datetime.time.sec;
    int timezone_offset = datetime.tz->utc_hour_difference * sixty * sixty;
    return timestamp - timezone_offset;
}

void printDateTimeTZ(TDateTimeTZ datetimetz) {
    char *month[] = {"ianuarie", "februarie", "martie", "aprilie", "mai", "iunie",
        "iulie", "august", "septembrie", "octombrie", "noiembrie", "decembrie"};
    if (datetimetz.date.day < ten) {
        printf("0%d %s %d, %02d:%02d:%02d %s (UTC%+d)\n", datetimetz.date.day, month[datetimetz.date.month - 1],
         datetimetz.date.year, datetimetz.time.hour, datetimetz.time.min, datetimetz.time.sec,
         datetimetz.tz->name, datetimetz.tz->utc_hour_difference);
    } else {
        printf("%d %s %d, %02d:%02d:%02d %s (UTC%+d)\n", datetimetz.date.day, month[datetimetz.date.month - 1],
         datetimetz.date.year, datetimetz.time.hour, datetimetz.time.min, datetimetz.time.sec,
         datetimetz.tz->name, datetimetz.tz->utc_hour_difference);
    }
}
