#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libical/ical.h>
#include <unistd.h>
#include <time.h>

#define ICAL_READ_STEP_BUFFER	1024

void dump_icaltimetype (const char * format, struct icaltimetype time);

void dump_icalrecurrencetype (struct icalrecurrencetype recur);

char * readFile(const char * pFile, long * pLength);

void parse_iCal(icalcomponent* comp);

void parseFile(const char * pFile);

int help_flag;

int debug_flag;

