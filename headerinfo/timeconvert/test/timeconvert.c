#include <stdio.h>
#include <time.h>

int main(void)
{
    time_t     now;
    struct tm  ts;
    char       buf[80];

    // Get current time
    time(&now);

    // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    ts = *localtime(&now);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
    printf("%s\n", buf);


    // part 2
    struct tm t;
    time_t t_of_day;

    t.tm_year = 2011-1900;
    t.tm_mon = 7;           // Month, 0 - jan
    t.tm_mday = 8;          // Day of the month
    t.tm_hour = 16;
    t.tm_min = 11;
    t.tm_sec = 42;
    t.tm_isdst = -1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
    t_of_day = mktime(&t);

    printf("seconds since the Epoch: %ld\n", (long) t_of_day);


    return 0;
}
