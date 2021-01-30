make all
mpirun -np 35 --hostfile myhosts.txt main 15 testfiles/numservers15_np35.txt
