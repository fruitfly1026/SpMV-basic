#!/usr/bin/perl

#$infile = "../../../test_one_mat";
$infile = "./mats_ncomplex_nsmall";
$Fpath = "../offline/Mats_features";
$ofile = ">./mats_with_best_formats";

$line_num = 0;
$count = 0;
$int_len = 4;
$float_len = 4;
$min_speedup = 100;

open (OUT_MATS, $ofile) || die "Cannot open $ofile : $!\n";

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
    elsif ( $best_format eq "ELL" )
    {
      if ( $CSR_perf != 0 )
      {
        $CSR_speedup = $ELL_perf / $CSR_perf;
      }
      if ( $DIA_perf != 0 )
      {
        $DIA_speedup = $ELL_perf / $DIA_perf;
      }
      if ( $COO_perf != 0 )
      {
        $COO_speedup = $ELL_perf / $COO_perf;
      }
      if ( $CSR_speedup <= $DIA_speedup && $CSR_speedup != 0 )
      {
        $min_speedup = $DIA_speedup;
      }
      elsif ( $CSR_speedup > $DIA_speedup && $DIA_speedup != 0 )
      {
        $min_speedup = $DIA_speedup;
      }
      if ( $COO_speedup <= $min_speedup)
      {
        $min_speedup = $COO_speedup;
      }
    }
    elsif ( $best_format eq "CSR" )
    {
      if ( $DIA_perf != 0 )
      {
        $DIA_speedup = $CSR_perf / $DIA_perf;
      }
      if ( $ELL_perf != 0 )
      {
        $ELL_speedup = $CSR_perf / $ELL_perf;
      }
      if ( $COO_perf != 0 )
      {
        $COO_speedup = $CSR_perf / $COO_perf;
      }
      if ( $DIA_speedup <= $ELL_speedup && $DIA_speedup != 0 )
      {
        $min_speedup = $DIA_speedup;
      }
      elsif ( $DIA_speedup > $ELL_speedup && $ELL_speedup != 0 )
      {
        $min_speedup = $ELL_speedup;
      }
      if ( $COO_speedup <= $min_speedup)
      {
        $min_speedup = $COO_speedup;
      }
    }
    elsif ( $best_format eq "COO" )
    {
      if ( $CSR_perf != 0 )
      {
        $CSR_speedup = $COO_perf / $CSR_perf;
      }
      if ( $DIA_perf != 0 )
      {
        $DIA_speedup = $COO_perf / $DIA_perf;
      }
      if ( $ELL_perf != 0 )
      {
        $ELL_speedup = $COO_perf / $ELL_perf;
      }
      if ( $CSR_speedup <= $DIA_speedup && $CSR_speedup != 0 )
      {
        $min_speedup = $DIA_speedup;
      }
      elsif ( $CSR_speedup > $DIA_speedup && $DIA_speedup != 0 )
      {
        $min_speedup = $DIA_speedup;
      }
      if ( $ELL_speedup < $min_speedup && $ELL_speedup != 0 )
      {
        $min_speedup = $ELL_speedup;
      }
    }

    printf OUT_MATS ("%s\n", $file);
    printf OUT_MATS ("Best format : %s\n", $best_format);
    printf OUT_MATS ("Speedup : %.2f\n", $min_speedup);
}
close (INFILE);
close (OUT_MATS);
