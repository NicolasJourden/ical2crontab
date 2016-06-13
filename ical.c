#include <stdio.h>
#include <stdlib.h>
#include <libical/ical.h>
#include <time.h>
#include <string.h>

//
// minute (0-59), hour (0-23, 0 = midnight), day (1-31), month (1-12), weekday (0-6, 0 = Sunday), command
// 
// 01 04 1 1 1 /usr/bin/somedirectory/somecommand
//


#define ICAL_READ_STEP_BUFFER	1024
char * readFile(const char * pFile, long * pLength)
{
  FILE * f = NULL;
  char * buffer = NULL;
  char * string = NULL;
  size_t size = 0;
  size_t used = 0;

  f = fopen(pFile, "r");

  while (used == size)
  {
//printf("1-used: %d, size: %d\n", used, size);

    size += ICAL_READ_STEP_BUFFER;
    buffer = (char *) realloc( (void *) string, size);

    // Over flow:
    if (buffer == NULL)
    {
      free(string);
      return NULL;
    }
    string = buffer;

    // Read data:
    used += fread(string+used, 1, size-used, f);
//printf("2-used: %d, size: %d\n", used, size);
  }

  fclose(f);

  *pLength = used;
  return string;
}

void parse_iCal(icalcomponent* comp)
{
  icalcomponent *c;
  icalproperty * rrule;
  icalproperty * exdate;

  icalrecur_iterator * data;
  struct icaltimetype time_start;
  struct icaltimetype time_end;
  struct icaltimetype exdatetime;
  struct icaltimetype now;
  struct icalrecurrencetype recur;
  int i = 0;
  double duration;

  // Get time now:
  now = icaltime_today();

  // Read each event:
  for (c = icalcomponent_get_first_component(comp,ICAL_VEVENT_COMPONENT) ;
       c != 0 ;
       c = icalcomponent_get_next_component(comp,ICAL_VEVENT_COMPONENT)
  )
  {
    puts(" -------- ");

    // Get details about the event:
    const char * summary = icalcomponent_get_summary(c);
    const char * description = icalcomponent_get_description(c);
#ifdef DEBUG_DETAILS
    if (summary != NULL)
    {
      printf("summary: %s\n", summary);
    }
    if (description != NULL)
    {
      printf("description: %s\n", description);
    }
#endif

    time_start = icalcomponent_get_dtstart (c);
    time_end = icalcomponent_get_dtend (c);
    duration = difftime(icaltime_as_timet(time_end), icaltime_as_timet(time_start));

    // Skip stuff from the past:
    if (!icaltime_is_null_time (recur.until) && recur.until.is_date) 
    {
      if (icaltime_compare(now, recur.until) > 0)
      {
        continue;
      }
    }

#ifdef DEBUG_START_END
    printf ("start: %02d:%02d:%02d %02d-%02d-%04d\n", 
      time_start.hour,  time_start.minute,  time_start.second, 
      time_start.day, time_start.month, time_start.year
    );
    printf ("end: %02d:%02d:%02d %02d-%02d-%04d\n", 
      time_end.hour,  time_end.minute,  time_end.second, 
      time_end.day, time_end.month, time_end.year
    );
#endif

    // Rule:
    rrule = icalcomponent_get_first_property(c,ICAL_RRULE_PROPERTY);
    recur = icalproperty_get_rrule(rrule);
    data = icalrecur_iterator_new (recur, time_start);
    struct icaltimetype time_tmp = icalrecur_iterator_next (data);

#ifdef DEBUG_RECURRENCE
    printf ("next: %02d:%02d:%02d %02d-%02d-%04d\n", 
      time_tmp.hour,  time_tmp.minute,  time_tmp.second, 
      time_tmp.day, time_tmp.month, time_tmp.year
    );
    int day = (int) icalrecurrencetype_day_day_of_week (recur.by_month_day[0]);
    printf ("r icalrecurrencetype_day_day_of_week : %d\n", ((int) icalrecurrencetype_day_day_of_week(recur.by_month_day[0])));
    printf ("r icalrecurrencetype_day_position : %d\n", ((int) icalrecurrencetype_day_position(recur.by_month_day[0])));
    printf ("r by_hour: %d\n", recur.by_hour[0]);
    printf ("r by_minute: %d\n", recur.by_minute[0]);
    printf ("r by_month: %d\n", recur.by_month[0]);
    printf ("r by_month_day: %d\n", recur.by_month_day[0]);
    printf ("r by_second: %d\n", recur.by_second[0]);
    printf ("r by_set_pos: %d\n", recur.by_set_pos[0]);
    printf ("r by_week_no: %d\n", recur.by_week_no[0]);
    printf ("r by_year_day: %d\n", recur.by_year_day[0]);
    printf ("r count: %d\n", recur.count);
    printf ("r interval: %d\n", recur.interval);
//    printf ("r rscale: %s\n", recur.rscale);
//    printf ("r skip: %d\n", recur.skip);
//    printf ("r freq: %d\n", recur.freq);
    printf ("r week_start: %d\n", recur.week_start);
    for (i = 0; i < 7; i++)
    {
      if (recur.by_day[i] == 32639) break;
      printf ("r by_day: %03d-%02d\n", i, recur.by_day[i]);
    } 
#endif

// minute (0-59), hour (0-23, 0 = midnight), day (1-31), month (1-12), weekday (0-6, 0 = Sunday), command


    // Set time and hours:
    printf ("%02d	%02d	",
          time_start.minute,
          time_start.hour
    );

    // Set option depending of recurence:
    switch (recur.freq)
    {
      // Weekly: OK but need to remove days not needed... ?
      case ICAL_WEEKLY_RECURRENCE:
        printf ("%s", "*	*	");
        for (i = 0; recur.by_day[i] != 32639; i++)
        {
          printf ("%02d", icalrecurrencetype_day_day_of_week(recur.by_day[i]) -1);
          if (recur.by_day[i+1] != 32639)
          {
            printf ("%s", ",");
          }
        }
        printf ("%s", "	");
        break;

      // Happening each month:
      case ICAL_MONTHLY_RECURRENCE:
        // The Xth of the month:
        if (recur.by_month_day[0] != 32639)
        {
          printf ("%02d	*	*		", recur.by_month_day[0]);
        }
        // Day of the week:
        else
        {

    printf ("next: %02d:%02d:%02d %02d-%02d-%04d\n", 
      time_tmp.hour,  time_tmp.minute,  time_tmp.second, 
      time_tmp.day, time_tmp.month, time_tmp.year
    );
    int day = (int) icalrecurrencetype_day_day_of_week (recur.by_month_day[0]);
    printf ("r icalrecurrencetype_day_day_of_week : %d\n", ((int) icalrecurrencetype_day_day_of_week(recur.by_month_day[0])));
    printf ("r icalrecurrencetype_day_position : %d\n", ((int) icalrecurrencetype_day_position(recur.by_month_day[0])));
//    printf ("r icalrecurrencetype_month_is_leap : %d\n", ((int) icalrecurrencetype_month_is_leap(recur.by_month_day[0])));
//    printf ("r icalrecurrencetype_month_month : %d\n", ((int) icalrecurrencetype_month_month(recur.by_month_day[0])));
    printf ("r by_hour: %d\n", recur.by_hour[0]);
    printf ("r by_minute: %d\n", recur.by_minute[0]);
    printf ("r by_month: %d\n", recur.by_month[0]);
    printf ("r by_month_day: %d\n", recur.by_month_day[0]);
    printf ("r by_second: %d\n", recur.by_second[0]);
    printf ("r by_set_pos: %d\n", recur.by_set_pos[0]);
    printf ("r by_week_no: %d\n", recur.by_week_no[0]);
    printf ("r by_year_day: %d\n", recur.by_year_day[0]);
    printf ("r count: %d\n", recur.count);
    printf ("r interval: %d\n", recur.interval);
//    printf ("r rscale: %s\n", recur.rscale);
//    printf ("r skip: %d\n", recur.skip);
//    printf ("r freq: %d\n", recur.freq);
    printf ("r week_start: %d\n", recur.week_start);
    for (i = 0; i < 7; i++)
    {
      if (recur.by_day[i] == 32639) break;
      printf ("r by_day: %03d-%02d\n", i, recur.by_day[i]);
    } 

      switch ( ( (icalrecurrencetype_day_position(recur.by_day[0]) ) / 10 ) * 10 )
      {
        case ICAL_SUNDAY_WEEKDAY :
          printf(" SUNDAY ");
          break;
        case ICAL_MONDAY_WEEKDAY :
          printf(" MONDAY ");
          break;
        case ICAL_TUESDAY_WEEKDAY :
          printf(" TUESDAY ");
          break;
        case ICAL_WEDNESDAY_WEEKDAY :
          printf(" WEDNESDAY ");
          break;
        case ICAL_THURSDAY_WEEKDAY :
          printf(" THURSDAY ");
          break;
        case ICAL_FRIDAY_WEEKDAY :
          printf(" FRIDAY ");
          break;
        case ICAL_SATURDAY_WEEKDAY :
          printf(" SATURDAY ");
          break;
        case ICAL_NO_WEEKDAY :
          printf(" ICAL_NO_WEEKDAY ");
          break;
        default:
          printf(" never ");
      }

          // TODO: possible?
          printf ("*	*	%02d?		", ( (int) icalrecurrencetype_day_day_of_week( recur.by_month_day[0] ) ) - 1 );
        }
        break;

      // Event happening each year ...  OK??
      // OK: one shoot.
      case ICAL_YEARLY_RECURRENCE:
      case ICAL_NO_RECURRENCE:
      default:
        printf ("%02d	%02d	*		", 
          time_start.day,
          time_start.month
        );
    }

    // Exclude some days?
    exdate = icalcomponent_get_first_property(c, ICAL_EXDATE_PROPERTY);
    if (exdate != NULL)
    {
      /* $ date "+%Y%m%d" | grep -q "20160304\|20150607\|20160405" && echo "OK" */
      printf ("%s", "date \"+%Y%m%d\" | grep -q \"");
      for (;
           exdate != NULL;
           exdate = icalcomponent_get_next_property(c, ICAL_EXDATE_PROPERTY)
      )
      {
        exdatetime = icalvalue_get_datetime(icalproperty_get_value(exdate));
        printf ("%04d%02d%02d\\|", exdatetime.year, exdatetime.month, exdatetime.day);
      }
      printf ("1\" && ");
    }

    // And the action:
    printf ("# %s -t %.0f\n", 
          summary,
          duration
    );


    icalrecur_iterator_free (data);

  }
}

int main (int argc, char *argv[])
{
  char * content;
  icalcomponent * calendar;
  long nb = 0;

  // Read from file: 
//  content = readFile("../tests/data_1.ical", &nb);
  content = readFile("/dev/stdin", &nb);
//  printf("%s", content);

  calendar = icalparser_parse_string(content); 
  parse_iCal(calendar);
  icalcomponent_free(calendar);

  return 0;
}

