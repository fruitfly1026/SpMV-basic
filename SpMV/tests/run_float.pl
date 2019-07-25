#!/usr/bin/perl

#$matrices = "../mats_list/sc09_mats_list";
$matrices = "./tmp_list";
$out = "results";
#$matrices = "../../../mats_list/mats_ncomplex_nsmall";
#$out = "../Mats_features_float";

$line_num = 0;

print "Using $matrices input MM matrices\n";
open (MM_MAT, $matrices) || die "Cannot open $matrices : $!\n";
while ($mm_file = <MM_MAT>)
{
    $line_num ++;
    chomp $mm_file;
    print $mm_file, "\n";
    @name = split ( /\//, $mm_file);  
    @tmp = split (/.mtx/, $name[-1]);
    mkdir "$out\/$name[5]";
    mkdir "$out\/$name[5]\/$name[6]";
    mkdir "$out\/$name[5]\/$name[6]\/$tmp[0]_perf";

    print "Generate new \"config.h\"\n";
    open (CONFIG_FILE, "config_init.h") || die "Cannot open \"config_init.h\" : $!\n";
    @config_init = <CONFIG_FILE>; 
    $config_num = @config_init;
    $config_init[$config_num] = "//output file names\n";
    $config_init[$config_num+1] = "#define KERNEL_TAGS \".\/kernel_tags_float\"\n";
    $config_init[$config_num+2] = "#define DIAG_RATIOS \"$out\/$name[5]\/$name[6]\/$tmp[0]_perf\/diag_ratios\"\n";
    $config_init[$config_num+3] = "#define NZS_DISTRIBUTION \"$out\/$name[5]\/$name[6]\/$tmp[0]_perf\/nzs_distribution\"\n";
    $config_init[$config_num+4] = "#define NZS_PER_ROW \"$out\/$name[5]\/$name[6]\/$tmp[0]_perf\/nzs_per_row\"\n";
    $config_init[$config_num+5] = "#define MAT_FEATURES \"$out\/$name[5]\/$name[6]\/$tmp[0]_perf\/mat_features\"\n";
    open (WRITE_CONFIG, ">config.h") || die "Cannot write to \"config.h\": $!\n";
    print WRITE_CONFIG @config_init;
#    print @config_init;
    close (WRITE_CONFIG); 
    close (CONFIG_FILE);
    
    system ("make clean; make\n"); 
    
    print "./spmv-icc --precision=32 ${mm_file} >
    $out\/$name[5]\/$name[6]\/$tmp[0]_perf\/running_log\n";
    system ("./spmv-icc --precision=32 ${mm_file} > \\
      $out\/$name[5]\/$name[6]\/$tmp[0]_perf\/running_log\n");
    print ("rm config.h\n\n");
    system ("rm config.h\n");
}
close(MM_MAT);

