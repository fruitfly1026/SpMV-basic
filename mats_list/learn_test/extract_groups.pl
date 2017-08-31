#!/usr/bin/perl

#$infile = "../test_one_mat";
$infile = "../mats_ncomplex_nsmall";
$ofile = ">./mats_groups";

$line_num = 0;
$count = 1;
$int_len = 4;
$float_len = 4;
$group_name = "NULL";
$group_num = 0;
$total = 0 - 1;

open (OUT_MATS, $ofile) || die "Cannot open $ofile : $!\n";

printf STDOUT ("Reading %s\n", $infile);
open (INFILE, $infile) || die "Cannot open $infile : $!\n";
while ($file = <INFILE>)
{
    chomp $file;
    $line_num ++;
    @name = split ( /\//, $file);  
    @tmp = split (/.mtx/, $name[-1]);

    if ( $name[5] ne $group_name )
    {
      $group_num ++;
      $total += $count;
      if ( $count > 10 )
      {
        printf OUT_MATS ("%s : %d\n", $group_name, $count);
      }
      $count = 1;
    }
    else
    {
      $count ++;
    }
    $group_name = $name[5];
    if ( $line_num == 2378 )
    {
#      $group_num ++;  //Because the first group has counted extra group.
      $total += $count;
      if ( $count > 10 )
      {
        printf OUT_MATS ("%s : %d\n", $group_name, $count);
      }
    }
}
close (INFILE);
close (OUT_MATS);
printf ("Group number : %d\n", $group_num);
printf ("Total matrices number : %d\n", $total);
