#!/bin/bash

matrices_path="/raid/home/liji541/BIGMATRICES/MM"

cd ${matrices_path}
group_names=$(ls -d *)
echo ${group_names}

for group in ${group_names}
do
	cd ${group}
	pwd
	matrix_list=$(ls -d *)
	echo ${matrix_list}
	# rm *.tar.gz
	ls .
	cd ..
done