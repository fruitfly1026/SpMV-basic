#!/usr/bin/perl

#$infile = "../../../test_one_mat";
$infile = "../mats_ncomplex_nsmall";
$Fpath = "../../offline/Mats_features";
$ofile = ">./DIA_mats";
$ofile_2 = ">./DIA_mats_speedup";

$line_num = 0;
$count = 0;
$int_len = 4;
$float_len = 4;
$min_speedup = 10000;

open (OUT_MATS, $ofile) || die "Cannot open $ofile : $!\n";
open (OUT_MATS_2, $ofile_2) || die "Cannot open $ofile_2 : $!\n";

printf STDOUT ("Reading %s\n", $infile);
open (INFILE, $infile) || die "Cannot open $infile : $!\n";
while ($file = <INFILE>)
{
    chomp $file;
    $line_num ++;
    @name = split ( /\//, $file);  
    @tmp = split (/.mtx/, $name[-1]);
    $Ffile = $Fpath . "\/$name[5]\/$name[6]\/$tmp[0]_perf\/mat_features";
    open (FEA_FILE, $Ffile) || die "Cannot open $Ffile : $!\n";
    while ($feature_file = <FEA_FILE>)
    {
      chomp $feature_file;
      @words = split (/\ :\ /, $feature_file);  
#      printf ("%s\n", $words[0]);
      if ($words[0] eq "DIA perf")
      {
        $DIA_perf = $words[1];
      }
      if ($words[0] eq "ELL perf")
      {
        $ELL_perf = $words[1];
      }
      if ($words[0] eq "CSR perf")
      {
        $CSR_perf = $words[1];
      }
      if ($words[0] eq "COO perf")
      {
        $COO_perf = $words[1];
      }
      if ($words[0] eq "Best_format")
      {
        $best_format = $words[1];
      }
    }
    close(FEA_FILE);
#    printf ("Best format : %s\n", $best_format);
    if ( $best_format eq "DIA" )
    {
      if ( $CSR_perf != 0 )
      {
        $CSR_speedup = $DIA_perf / $CSR_perf;
      }
      if ( $ELL_perf != 0 )
      {
        $ELL_speedup = $DIA_perf / $ELL_perf;
      }
      if ( $COO_perf != 0 )
      {
        $COO_speedup = $DIA_perf / $COO_perf;
      }
      if ( $CSR_speedup <= $ELL_speedup && $CSR_speedup != 0 )
      {
        $min_speedup = $DIA_speedup;
      }
      elsif ( $CSR_speedup > $ELL_speedup && $ELL_speedup != 0 )
      {
        $min_speedup = $ELL_speedup;
      }
      if ( $COO_speedup <= $min_speedup)
      {
        $min_speedup = $COO_speedup;
      }
    }
=pod
    printf ("DIA_Perf: %f\n", $DIA_perf);
    printf ("ELL Perf: %f\n", $ELL_perf);
    printf ("CSR Perf: %f\n", $CSR_perf);
    printf ("COO Perf: %f\n", $COO_perf);
=cut
#    printf ("CSR Speedup: %f\n", $CSR_speedup);
#    printf ("ELL Speedup: %f\n", $ELL_speedup);
#    printf ("COO Speedup: %f\n", $COO_speedup);
    if ( $best_format eq "DIA" && $min_speedup > 1.1)
    {
      $count ++;
      printf OUT_MATS ("%s\n", $file);
      printf OUT_MATS_2 ("%s\n", $file);
      printf OUT_MATS_2 ("Speedup : %f\n", $min_speedup);
    }
    $CSR_speedup = 0;
    $ELL_speedup = 0;
    $COO_speedup = 0;
}
close (INFILE);
close (OUT_MATS);
close (OUT_MATS_2);
printf ("%d\n", $count);
