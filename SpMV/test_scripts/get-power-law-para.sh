#!//bin/bash

matrices_path="/raid/home/liji541/BIGMATRICES/MM"
spmv_ext="/raid/home/liji541/Codes/SpMV-basic/SpMV/spmv"

cd ${matrices_path}
group_names=$(ls -d *)
# echo ${group_names}

for group in ${group_names}
do
	cd ${group}
	# pwd
	matrix_list=$(ls -d *)
	# echo ${matrix_list}
	for matrix in ${matrix_list}
	do
		cd ${matrix}
		# pwd
		${spmv_ext} ${matrix}.mtx
		cd ..
	done
	cd ..
done