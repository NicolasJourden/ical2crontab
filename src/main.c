#include "ical.h"

void parseFile(const char * pFile)
{
  char * content;
  icalcomponent * calendar;
  long nb = 0;

  // Read from file: 
  content = readFile(pFile, &nb);
//  printf("%s", content);

  calendar = icalparser_parse_string(content); 
  parse_iCal(calendar);
  icalcomponent_free(calendar);
}

int main (int argc, char *argv[])
{
  char * input_file = "/dev/stdin";
  int index;
  int c;

  // Read option from the menu:
  while ((c = getopt (argc, argv, "dhi:")) != -1)
  {
    switch (c)
    {
      case 'd':
        debug_flag = 1;
        break;
      case 'h':
        help_flag = 1;
        break;
      case 'i':
        input_file = optarg;
        break;
      case '?':
        if (optopt == 'i')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        return 1;
      default:
        abort ();
    }
  }

  // Need help:
  if (help_flag == 1)
  {
    puts ("ical2crontab usage:");
    puts ("  -i for input file, if not provided input is from STDIN.");
    puts ("  -h for this help.");
    puts ("\n\nNicolas JOURDEN - 2016");
  }

  // Parse the file:
  else {
    parseFile(input_file);
  }

  return 0;
}

