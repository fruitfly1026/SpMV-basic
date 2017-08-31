#!/usr/bin/perl

#$infile = "../test_one_mat";
$infile = "../mats_ncomplex_nsmall";
$infile_test = "./test_mats_list";
$ofile = ">./learn_mats_list";

$line_num = 0;
$count = 1;
$group_name = "NULL";
$group_num = 0;
$total = 0 - 1;
$tag = 0;

open (OUT_MATS, $ofile) || die "Cannot open $ofile : $!\n";

printf STDOUT ("Reading %s\n", $infile);
open (INFILE, $infile) || die "Cannot open $infile : $!\n";
while ($file = <INFILE>)
{
    chomp $file;
    open (INFILE_TEST, $infile_test) || die "Cannot open $infile_test : $!\n";
    while ($test_file = <INFILE_TEST>)
    {
        chomp $test_file;
        if ( $file eq $test_file )
        {
          $tag = 1;
        }
    }
    close(INFILE_TEST);
    if ( $tag == 0 )
    {
      printf OUT_MATS ("%s\n", $file);
    }
    $tag = 0;
}
close (INFILE);
close (OUT_MATS);
