#!/usr/bin/perl

use Data::Dumper;
use Data::ICal;

my $calendar = Data::ICal->new(filename => 'basic.ics');
my $results = {};

foreach my $event (@{$calendar->{entries}})
{
  $event->{properties}->{dtstart}[0]->{value} =~ s/T.*//;
  $event->{properties}->{dtend}[0]->{value} =~ s/T.*//;
  $results->{$event->{properties}->{dtstart}[0]->{value}}->{end} = $event->{properties}->{dtend}[0]->{value};
  $results->{$event->{properties}->{dtstart}[0]->{value}}->{summary} = $event->{properties}->{summary}[0]->{value};
  $results->{$event->{properties}->{dtstart}[0]->{value}}->{description} = $event->{properties}->{description}[0]->{value};
  $results->{$event->{properties}->{dtstart}[0]->{value}}->{location} = $event->{properties}->{location}[0]->{value};
}

foreach my $ev (sort keys $results)
{
  print $ev;
  print "\t";
  print $results->{$ev}->{end};
  print "\t";
  print $results->{$ev}->{summary};
  print "\t\t\t";
  print $results->{$ev}->{location};
  if ($results->{$ev}->{description})
  {
    print "\t";
    print $results->{$ev}->{description};
  }
  print "\n";
}

