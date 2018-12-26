#include "ical.h"

//
// minute (0-59), hour (0-23, 0 = midnight), day (1-31), month (1-12), weekday (0-6, 0 = Sunday), command
// 
// 01 04 1 1 1 /usr/bin/somedirectory/somecommand
//

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
  }

  fclose(f);

  *pLength = used;
  return string;
}

void parse_iCal(icalcomponent* comp)
{
  icalcomponent * c;
  icalproperty * rrule;
  icalproperty * exdate;

  icalrecur_iterator * data;
  struct icaldurationtype offset;
  struct icaltimetype time_start;
  struct icaltimetype time_end;
  struct icaltimetype time_next;
  struct icaltimetype exdatetime;
  struct icaltimetype now;
  struct icalrecurrencetype recur;
  int i = 0;
  double duration;

  // Get offset:
  time_t t = time(NULL);
  struct tm lt = {0};
  localtime_r(&t, &lt);
  if (debug_flag)
  {
    printf("Offset to GMT is %lds.\n", lt.tm_gmtoff);
  }
  offset.days = 0;
  offset.weeks = 0;
  offset.hours = 0;
  offset.minutes = 0;
  offset.seconds = 0;
  offset.hours = lt.tm_gmtoff / 3600;
  if (lt.tm_gmtoff < 0) {
   offset.is_neg = 1;
  } else {
   offset.is_neg = 0;
  }

  // Get time now:
  now = icaltime_today();

  // Read each event:
  for (c = icalcomponent_get_first_component(comp,ICAL_VEVENT_COMPONENT) ;
       c != 0 ;
       c = icalcomponent_get_next_component(comp,ICAL_VEVENT_COMPONENT)
  )
  {
    // Get details about the event:
    const char * summary = icalcomponent_get_summary(c);
    const char * description = icalcomponent_get_description(c);

    // Help:
    if (debug_flag)
    {
      if (summary != NULL)
      {
        printf("summary: %s\n", summary);
      }
      if (description != NULL)
      {
        printf("description: %s\n", description);
      }
    }

    // Ranging time:
    time_start = icalcomponent_get_dtstart (c);
    time_end = icalcomponent_get_dtend (c);
    duration = difftime(icaltime_as_timet(time_end), icaltime_as_timet(time_start));

    // Rules:
    rrule = icalcomponent_get_first_property(c,ICAL_RRULE_PROPERTY);
    recur = icalproperty_get_rrule(rrule);
    data = icalrecur_iterator_new (recur, time_start);

    // Find next event:
    while (1)
    {
      time_next = icalrecur_iterator_next (data);

      if (icaltime_is_null_time(time_next) == 1) break;
      if (icaltime_compare(time_next, now) >= 0) break;
    }

    // Help:
    if (debug_flag)
    {
      dump_icaltimetype("time start", time_start);
      dump_icaltimetype("time end", time_end);
      dump_icaltimetype("until", recur.until);
      dump_icaltimetype("time next", time_next);
    }

    // One shot event:
    if (
      icaltime_is_null_time(time_next) == 1
      &&
      icaltime_compare(time_start, now) < 0
    )
    {
      if (debug_flag) printf ("event (one shot) is in the past\n");
      continue;
    }

    // Old event: discard recurent events finished before today...
    if (
      icaltime_is_null_time(recur.until) == 0
      &&
      icaltime_compare(recur.until, now) < 0
    )
    {
      if (debug_flag) printf ("event is recurent in the past ...\n");
      continue;
    }

    // Some help:
    if (debug_flag > 2)
    {
      dump_icalrecurrencetype(recur);
    }


    // Set time and hours:
    struct icaltimetype local_time_start = icaltime_add(time_start, offset);
    printf ("%d	%d	", local_time_start.minute, local_time_start.hour);

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
          printf ("%02d	*	*	", recur.by_month_day[0]);
        }

        // Day of the week:
        else
        {
            printf ("%d	%d	*	", time_next.day, time_next.month);
        }
        break;


      // Event happening each year ...  OK???
      // OK: one shoot.
      case ICAL_YEARLY_RECURRENCE:
      case ICAL_NO_RECURRENCE:
      default:
        printf ("%02d	%02d	*	", 
          local_time_start.day,
          local_time_start.month
        );
    }



    // How many valid exclude dates:
    exdate = icalcomponent_get_first_property(c, ICAL_EXDATE_PROPERTY);
    unsigned int num_ex = 0;
    if (exdate != NULL)
    {
      for (; exdate != NULL; exdate = icalcomponent_get_next_property(c, ICAL_EXDATE_PROPERTY) )
      {
        exdatetime = icalvalue_get_datetime(icalproperty_get_value(exdate));
        if (icaltime_compare(exdatetime, now) >= 0)
        {
          num_ex++;
        }
      }
    }
    if (num_ex > 0)
    {
      exdate = icalcomponent_get_first_property(c, ICAL_EXDATE_PROPERTY);
      for (; exdate != NULL; exdate = icalcomponent_get_next_property(c, ICAL_EXDATE_PROPERTY) )
      {
        exdatetime = icalvalue_get_datetime(icalproperty_get_value(exdate));
        if (icaltime_compare(exdatetime, now) >= 0)
        {
          struct icaltimetype local_exdatetime = icaltime_add(exdatetime, offset);
          printf ("%s", "[ \"$(date \"+\\\%Y\\\%m\\\%d\")\" = \"");
          printf ("%04d%02d%02d", local_exdatetime.year, local_exdatetime.month, local_exdatetime.day);
          printf ("%s", "\" ] || ");
        }
      }
    }

    // And the action:
    printf ("%s %.0f\n", 
      summary,
      duration
    );

    icalrecur_iterator_free (data);
  }
}


