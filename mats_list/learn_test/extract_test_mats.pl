#!/usr/bin/perl

#$infile = "../test_one_mat";
$infile = "./total_mats_groups";
$ofile = ">./test_mats_num_dis";

$line_num = 0;
$sum = 0;
$group_num = 167;
$total_mats_num = 2378;

open (OUT_MATS, $ofile) || die "Cannot open $ofile : $!\n";

printf STDOUT ("Reading %s\n", $infile);
open (INFILE, $infile) || die "Cannot open $infile : $!\n";
while ($file = <INFILE>)
{
    chomp $file;
    $line_num ++;
    @words = split (/\ :\ /, $file);  

#    printf OUT_MATS ("%d : ", $words[1]);
#    Use the round method to get the integer
    $tmp = ($group_num * 2 * $words[1])/ $total_mats_num;
    $tail = $tmp - int ($tmp);
    if ($tail < 0.5 )
    {
      $mats_num = int ($tmp);
    }
    else
    {
      $mats_num = int ($tmp) + 1;
    }
    if ( $mats_num != 0  )
    {
      $sum += $mats_num;
      printf OUT_MATS ("%s : ", $words[0]);
      printf OUT_MATS ("%d\n", $mats_num);
    }
}
close (INFILE);

printf OUT_MATS ("Total matrices : %d\n", $sum);
close (OUT_MATS);

