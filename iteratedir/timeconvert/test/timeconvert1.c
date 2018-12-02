#include <time.h>
#include <stdio.h>
#include <stdlib.h>

static const time_t default_time = 1230728833;
static const char default_format[] = "%a %b %d %Y";

int
main(int argc, char *argv[])
{
        time_t t = default_time;
        const char *format = default_format;

        struct tm lt;
        char res[32];

        if (argc >= 2) {
                t = (time_t) atoi(argv[1]);
        }

        if (argc >= 3) {
                format = argv[2];
        }

        (void) localtime_r(&t, &lt);

        if (strftime(res, sizeof(res), format, &lt) == 0) {
                (void) fprintf(stderr,  "strftime(3): cannot format supplied "
                                        "date/time into buffer of size %u "
                                        "using: '%s'\n",
                                        sizeof(res), format);
                return 1;
        }

        (void) printf("%u -> '%s'\n", (unsigned) t, res);

        return 0;
}