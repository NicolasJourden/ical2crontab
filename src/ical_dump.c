
#include "ical.h"

void dump_icaltimetype (const char * format, struct icaltimetype time)
{
  printf ("%s:\t\t%02d:%02d:%02d %02d-%02d-%04d\n", 
    format,
    time.hour,  time.minute,  time.second, 
    time.day, time.month, time.year
  );
}

void dump_icalrecurrencetype (struct icalrecurrencetype recur)
{
  int i;
  int day = (int) icalrecurrencetype_day_day_of_week (recur.by_month_day[0]);
  printf ("r icalrecurrencetype_day_day_of_week : %d\n", ((int) icalrecurrencetype_day_day_of_week(recur.by_month_day[0])));
  printf ("r icalrecurrencetype_day_position : %d\n", ((int) icalrecurrencetype_day_position(recur.by_month_day[0])));
//  printf ("r icalrecurrencetype_month_is_leap : %d\n", ((int) icalrecurrencetype_month_is_leap(recur.by_month_day[0])));
//  printf ("r icalrecurrencetype_month_month : %d\n", ((int) icalrecurrencetype_month_month(recur.by_month_day[0])));
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
}
