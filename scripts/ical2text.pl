#!/usr/bin/perl

use Data::Dumper;
use Data::ICal;
use Time::Piece;

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


  my $from = Time::Piece->strptime($ev, "%Y%m%d");
  my $to = Time::Piece->strptime($results->{$ev}->{end}, "%Y%m%d");
  my $diff = $to - $from;

  print $from->strftime("%Y-%m-%d");
  print "\t";
  print $to->strftime("%Y-%m-%d");
  print "\t";
  print int($diff->days);
  print "\t";
  print $results->{$ev}->{location};
  print "\t";
  print $results->{$ev}->{summary};
#  if ($results->{$ev}->{description})
#  {
#    print "\t";
#    print $results->{$ev}->{description};
#  }
  print "\n";
}

