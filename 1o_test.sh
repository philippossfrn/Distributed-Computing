make all
mpirun -np 14 --hostfile myhosts.txt main 10 testfiles/numservers10_np14.txt
