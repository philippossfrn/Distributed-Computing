make all
mpirun -np 70 --hostfile myhosts.txt main 30 testfiles/numservers30_np70.txt
