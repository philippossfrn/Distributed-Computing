#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 
#include <stddef.h> 

#define TAG 0 


int main(int argc, char *argv[]){

	int rank, world_size, i;
	/** MPI Initialisation **/
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	MPI_Datatype ;
	
	/** Create custom datatype to send array of MPI_INTs **/
	MPI_Datatype CUSTOM_ARRAY;
	MPI_Type_contiguous(3, MPI_INT, &CUSTOM_ARRAY);
	MPI_Type_commit(&CUSTOM_ARRAY);

	int dummy_int[3];
	if(rank == 0){
		// Coordinator
		printf("[rank: %d] Coordinator started\n", rank);
		
		
		for(i = 0; i< world_size; i++){
			if(i == rank) continue;
			dummy_int[0]= i;
			dummy_int[1]= i;
			dummy_int[2]= i;

			MPI_Send(&dummy_int, 1, CUSTOM_ARRAY, i, TAG, MPI_COMM_WORLD);
		}
	}else{
		// Peers
		printf("[rank: %d] Peer started\n", rank);
		MPI_Recv(&dummy_int, 1, CUSTOM_ARRAY, MPI_ANY_SOURCE, TAG, MPI_COMM_WORLD, &status);
		
		printf("[rank: %d] Peer received message: [%d, %d, %d] from %d\n", rank, dummy_int[0], dummy_int[1], dummy_int[2], status.MPI_SOURCE);
	}
	MPI_Finalize();
}
