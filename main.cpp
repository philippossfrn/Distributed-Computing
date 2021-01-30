#include "hashtable.h"
//#include "functions.h"

int main(int argc, char *argv[]){

	if(argc != 3){
		cout << "mpirun -np <count> <executable> <NUM_SERVERS> <testfile>" << endl;
		cout << "                             OR                         " << endl;
		cout << "mpirun -np <count> --hostfile <hostfile.txt> <executable> <NUM_SERVERS> <testfile>" << endl;
		return 1;
	}
	srand(time(NULL));


	int count_send=0;
	int rank, i;
	num_of_servers = atoi(argv[1]);
	int temp = num_of_servers + 1;
	/** MPI Initialisation **/
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	// MPI_Comm_size(MPI_COMM_INFORM_SERVERS, &(num_of_servers+1));
	// MPI_Comm_rank(MPI_COMM_INFORM_SERVERS, &rank);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	//MPI_Datatype ;
	
	
    MPI_Type_contiguous(3, MPI_INT, &struct_with_three_integers);
    MPI_Type_commit(&struct_with_three_integers);

    //ack = "ACK";
    MPI_Type_contiguous(3, MPI_BYTE, &ack_res);
    MPI_Type_commit(&ack_res);

    //--------------------------------
    int leader_id = rank;
    int servers_array_used_for_leader_election[num_of_servers];
    int candidate_msgs_received=0;
    int i_already_sent_candidate_msg=0;
    vector<int> unique_ids;
    vector<int> unique_sources_ids;
    int local_candidate_msgs_received=0;
    int choose_random;



	int dummy_int[3];
	//while(loop){
	if(rank == 0){ // Coordinator
		cout << endl << "HERE"  << endl;

		for (int i = 0; i < world_size; i++) 
        	total_np.push_back(i);

		
		//printf("Coordinator started\n");
		cout << endl << "Coordinator started reading from this file" << argv[2] << endl;
		cout << "NP count = " << world_size << endl;
		cout << "num of servers = " << num_of_servers << endl;

		// for (int i = 1; i <= 14; i++) 
  //       	total_servers.push_back(i); 
		
		//start_reading_test_file(argv[2]);
		string line;
		int index=0;
		  ifstream myfile (argv[2]);
		  if (myfile.is_open()){
		  	getline (myfile,line);
		  	start_leader++;
		    while ( ! myfile.eof() )
		    {

		    	istringstream ss(line); 
			    string word; 
			    ss >> word; 
				check_for_leader_election = word;
				//cout<<"---------------"<<check_for_leader_election<<endl;
				if(((check_for_leader_election).compare("START_LEADER_ELECTION")) == 0){
		    		for (int i = 0; i < (total_np.size()-(total_servers.size()-1)); i++) {
			        	if(total_np[i] == 0) continue;
			        	cout << endl << "Not a server->" << total_np[i] << endl; 
			            data_to_send.am_i_server = 0;
			            data_to_send.left_neighbour_rank = -1;
			            data_to_send.right_neighbour_rank = -1;
			            MPI_Send(&data_to_send, 1, struct_with_three_integers,total_np[i], YOU_ARE_NOT_A_SERVER, MPI_COMM_WORLD);
			            flag = 1;
			            wait_for_ack_first_stage++;
			            MPI_Recv(&ack, 1, ack_res, MPI_ANY_SOURCE, ACK, MPI_COMM_WORLD, &status);
			            cout << endl << "I received ack from "<< status.MPI_SOURCE << endl<< endl;
		            
		        	}
		         	create_map_with_commands(index,line);
		        	//cout << "Valid servers" << endl;
		         	for (int i = 0; i < (total_servers.size()-1); i++) {

				        //cout <<" "<< total_servers[i] << " ";              
				        third_stage = 1;
				        data_to_send.am_i_server = 1;
				        data_to_send.left_neighbour_rank = -1;
				        data_to_send.right_neighbour_rank = -1;
				        MPI_Send(&data_to_send, 1, struct_with_three_integers,total_servers[i], START_LEADER_ELECTION, MPI_COMM_WORLD);

				        //MPI_Recv(&ack, 1, ack_res, MPI_ANY_SOURCE, ACK, MPI_COMM_WORLD, &status);
				        //cout << endl << "I received ack from "<< status.MPI_SOURCE << endl<< endl;
			      	}
			      	MPI_Recv(&ack, 1, ack_res, MPI_ANY_SOURCE, LEADER_ELECTION_DONE, MPI_COMM_WORLD, &status);
				   	if((status.MPI_TAG == LEADER_ELECTION_DONE)){
						//<SERVER_ID>   CONNECTEDTO   <LEADER_ID>
						cout<<endl<< "--------I AM COORDINATOR AND I KNOW THAT LEADER ELECTION DONE--------"<< endl<<endl;
						// cout << "ALL SERVERS ARE" ;
			   //       	for (int i = 0; i < (total_servers.size()-1); i++) 
			   //       		cout<<" " <<total_servers[i]<< " ";
			   //       	cout<<endl;

			         						//Init clients vector with all np
						vector<int> clients;
						for(int i=1;i<world_size;i++)
							clients.push_back(i);
								
						
						//if clients vector contains server erase it
						//so clients vector now has only clients ids
						for(int i=0;i<(total_servers.size()-1);i++){
							if(contains(clients,total_servers[i])){
								clients.erase(std::remove(clients.begin(), clients.end(), total_servers[i]), clients.end());
							}
						}

						//send msg to every client inform them that i am the leader 
						data_to_send.am_i_server = status.MPI_SOURCE;
						for(int i=0;i<clients.size();i++){
							//cout<<" "<<clients[i]<<" ";
							
							MPI_Send(&data_to_send, 1, struct_with_three_integers,clients[i], CLIENT, MPI_COMM_WORLD);
							
							MPI_Recv(&ack, 1, ack_res, clients[i], ACK, MPI_COMM_WORLD, &status);
						}
					}
				  }else{
					create_map_with_commands(index,line);
				}
		    	
		    	if(first_stage < num_of_servers){
				    //create_map_with_commands(index,line);
				    MPI_Recv(&ack, 1, ack_res, MPI_ANY_SOURCE, ACK, MPI_COMM_WORLD, &status);
				    cout << endl << "I received ack from "<< status.MPI_SOURCE << endl<< endl;
			    	if(total_servers.size() == num_of_servers){
			    		//cout << "PHILIPPOS" ;
			    		total_servers.push_back(-42);
				        for (int i = 0; i < total_servers.size()-1; i++) {
				            //cout <<"server->"<< total_servers[i] << " ";
				            std::remove(total_np.begin(), total_np.end(), total_servers[i]);
				        }
				       	
				        
			      	}
		    		first_stage++;
		    	}
		    	index++;
		    getline (myfile,line);
		    start_leader++;
		    }
		    create_map_with_commands(index,line);
		    myfile.close();
		  }else cout << "Unable to open file";

    	//cout << endl << "Printing map with struct" << endl;

    	//print_commands(commands);
 
	}else {

		while(1){
			MPI_Recv(&data_to_send, 1, struct_with_three_integers, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			//MPI_Recv(&data_to_send, 1, struct_with_three_integers, my_data.my_right_neighbour, CANDIDATE_ID, MPI_COMM_WORLD, &status);
			if ((status.MPI_TAG == YOU_ARE_SERVER) || ((status.MPI_TAG == YOU_ARE_NOT_A_SERVER))){
				
				my_data.my_rank = rank;
				my_data.am_i_server = data_to_send.am_i_server;
				my_data.my_left_neighbour = data_to_send.left_neighbour_rank;
				my_data.my_right_neighbour = data_to_send.right_neighbour_rank;
				my_data.am_i_lucky = 0;
				cout << "My rank is " << my_data.my_rank << " and i am server if this is one->" << data_to_send.am_i_server << endl;
				cout << "My neighbors are left " << my_data.my_left_neighbour << " and right " <<  my_data.my_right_neighbour << endl;
				cout << status.MPI_SOURCE << " has send me these infos" << endl;
				MPI_Send(&ack, 1, ack_res,0, ACK, MPI_COMM_WORLD);
				first_stage_count++;
				//if(first_stage_count == world_size) third_stage = 1;
			}

			 if((status.MPI_TAG == START_LEADER_ELECTION)){
		        data_to_send.am_i_server = rank;
		        data_to_send.left_neighbour_rank = -1;
		        data_to_send.right_neighbour_rank = -1;
		        //MPI_ANY_SOURCE
		        if (!i_already_sent_candidate_msg){
		        	i_already_sent_candidate_msg = 1;
		        	//cout <<"@@@@@@@@"<<" I am " << rank << " and i will sent to "<<my_data.my_left_neighbour<<endl;
			 		MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, CANDIDATE_ID, MPI_COMM_WORLD);
		        }
			 	
			 	//cout<<endl;
				//cout<< "!!!!Start leader election other threads" << "My rank is----->" << leader_id << " "<< my_data.my_left_neighbour << " and " <<  my_data.my_right_neighbour<< endl;
				//cout << "My neighbors are " << my_data.my_left_neighbour << " and " <<  my_data.my_right_neighbour << endl;
				//cout<<endl;
			    		
			}
			if((status.MPI_TAG == CANDIDATE_ID) && my_data.am_i_server && (status.MPI_SOURCE == my_data.my_right_neighbour)){
				int temp = candidate_msgs_received;
				local_candidate_msgs_received++;
				candidate_msgs_received++;
				if (!i_already_sent_candidate_msg){
					data_to_send.am_i_server = rank;
					MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, CANDIDATE_ID, MPI_COMM_WORLD);
				}

				if(data_to_send.am_i_server > leader_id) leader_id = data_to_send.am_i_server;
				//cout<<"I AM "<<rank<<" and i received "<<data_to_send.am_i_server<<" from "<<status.MPI_SOURCE<<"%%%%%%%%%%%%%%%%%%%%%%%"<<endl;
				unique_ids.push_back(data_to_send.am_i_server);
				unique_sources_ids.push_back(status.MPI_SOURCE);

				servers_array_used_for_leader_election[temp]=data_to_send.am_i_server;
				



				if((unique_ids.size() == num_of_servers) && (unique(unique_ids))){
					//cout<<"I AM "<<rank<<" and i know that "<<"LEADER IS----->"<<leader_id<<endl;
					client_knows_leader = leader_id;
					// cout<<"VECTOR unique IDS";
					// for(int i=0;i<num_of_servers;i++)
					// 	cout<<" "<<unique_ids[i]<<" ";
					// cout<<endl;
					// cout<<"ARRAY SERVERS FOR SERVER "<<rank<< "-->";
					// for(int i=0;i<num_of_servers;i++)
					// 	cout<<" "<<servers_array_used_for_leader_election[i]<<" ";
					// cout<<endl;

				}
				if(data_to_send.am_i_server != rank){
					//data_to_send.am_i_server = leader_id;
					MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, CANDIDATE_ID, MPI_COMM_WORLD);
				}


				if((local_candidate_msgs_received == num_of_servers) && leader_id == rank){


					

					vector<int> to_choose_from;
					int lucky = (num_of_servers-3) / 4;
					//cout << "I WILL ELECT MY SELF +++++++++++++++++++++++++++++++++++++ " << rank << endl;
					
					for(int i=0;i<num_of_servers;i++){
						if((servers_array_used_for_leader_election[i] != rank) && (servers_array_used_for_leader_election[i] != my_data.my_left_neighbour) && (servers_array_used_for_leader_election[i]!=my_data.my_right_neighbour))
							to_choose_from.push_back(servers_array_used_for_leader_election[i]);
					}
					vector<int> rands;
					rands = choose_lucky_servers(to_choose_from,lucky);


					//int rands = (rand() % (num_of_servers-3)) ; // ??????????????????????? OXI TOUS IDIUS
					//cout<< "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& "<<to_choose_from.size();
					//cout<<"I KNOW THESE SERVERS ARE VALID";
					// for(int i=0;i<to_choose_from.size();i++){
					// 	cout<<" "<<to_choose_from[i]<<" ";
							
					// }
					cout<<endl;
					//cout<<"++++++++++++++++++++++++++++++++++I CHOOSE " <<to_choose_from[rands]<<endl;
					for(int i=0;i<lucky;i++){
						//data_to_send.am_i_server = to_choose_from[rands];
						data_to_send.am_i_server = rands[i];
						MPI_Send(&data_to_send, 1, struct_with_three_integers,rands[i], CONNECT, MPI_COMM_WORLD);
					}
				}
			}
			if((status.MPI_TAG == CONNECT)){
				my_data.am_i_lucky = 1;
				// cout<<"I AM "<<rank<<" and i am lucky if this is one->"<<my_data.am_i_lucky<<" leader is "<<leader_id<< endl;
				// cout << "My rank is " << my_data.my_rank << " and i am server if this is one->" << data_to_send.am_i_server << endl;
				// cout << "My neighbors are left " << my_data.my_left_neighbour << " and right " <<  my_data.my_right_neighbour << endl;
				MPI_Send(&ack, 1, ack_res,leader_id, CONNECTED_ACK, MPI_COMM_WORLD);

			}
			if((status.MPI_TAG == CONNECTED_ACK)){
				//<SERVER_ID>   CONNECTEDTO   <LEADER_ID>
				who_are_connected_with_me.push_back(status.MPI_SOURCE);
				cout<<green<< "--------" << status.MPI_SOURCE << " CONNECTED TO "<< rank << "--------"<<reset<< endl;
				MPI_Send(&ack, 1, ack_res,0, LEADER_ELECTION_DONE, MPI_COMM_WORLD);
			}
			if((status.MPI_TAG == CLIENT)){
				my_data_as_client.am_i_client = 1;
				my_data_as_client.my_rank = rank;
				my_data_as_client.leader_id = data_to_send.am_i_server;

				//cout << "My rank is " << my_data_as_client.my_rank << " and i am client if this is one->" << my_data_as_client.am_i_client <<" leader is "<<my_data_as_client.leader_id<< endl;
				MPI_Send(&ack, 1, ack_res,0, ACK, MPI_COMM_WORLD);

			}

			if((status.MPI_TAG == UPLOAD)){
				if(my_data.am_i_server == 0){//client received upload message
					//cout<<"================ I AM "<<rank<< " BUT AM I SERVER???? ----->"<<my_data.am_i_server<< " AND I FEEL LUCKY???? ----->"<<my_data.am_i_lucky<<endl;
					my_data_as_client.file_id_to_upload = data_to_send.left_neighbour_rank;
					my_data_as_client.file_version = 1;

					data_to_hold_client.file_id = data_to_send.left_neighbour_rank;
					data_to_hold_client.version = 1;
					local_data_for_file_client_keeps.push_back(data_to_hold_client);

					//cout << "My rank is " << my_data_as_client.my_rank << " and i am client if this is one->" << my_data_as_client.am_i_client << " I want to upload this file----> " << my_data_as_client.file_id_to_upload  <<" with file version "<<my_data_as_client.file_version<<" leader is "<<my_data_as_client.leader_id<< endl;
					//cout << "I am "<<rank<<" client and i want to upload "<<my_data_as_client.file_id_to_upload<<endl;
					data_to_send.am_i_server = my_data_as_client.file_id_to_upload;
					MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data_as_client.leader_id, SENT_FILE_TO_LEADER, MPI_COMM_WORLD);
				}else{// server received upload message
					if(data_to_send.left_neighbour_rank != rank){
						// I must forward the message to my left neighbour
						data_to_send.am_i_server = data_to_send.am_i_server;
						data_to_send.left_neighbour_rank = data_to_send.left_neighbour_rank;
						MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, UPLOAD, MPI_COMM_WORLD);
					}else if(data_to_send.left_neighbour_rank == rank){
						// i was the destination
						// save data for file
						data_to_hold.file_id = data_to_send.am_i_server;
						data_to_hold.version = 1;
						local_data_for_file_server_keeps.push_back(data_to_hold);

						if(my_data.am_i_lucky){// I am directly connected to leader
							data_to_send.am_i_server = data_to_hold.file_id;
							MPI_Send(&data_to_send, 1, struct_with_three_integers,leader_id, UPLOAD_ACK, MPI_COMM_WORLD);
						}else{
							data_to_send.am_i_server = data_to_hold.file_id;
							MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, UPLOAD_ACK, MPI_COMM_WORLD);
						}

					}

				}
			}
			if((status.MPI_TAG == SENT_FILE_TO_LEADER)){
				int file_to_upload = data_to_send.am_i_server;
				//cout<< "I AM "<<rank<<" AND "<<status.MPI_SOURCE<<" WANTS TO UPLOAD FILE "<<file_to_upload<<endl;
				//main2();
				// h.search_Hash(file_to_upload)
				if(search_in_hashtable(hashtable,file_to_upload)){//search_in_hashtable(hashtable,file_to_upload)
					total_fails++;
					//cout<<"++++++++++++++++++++++++++++++++++++++++SORRY "<<status.MPI_SOURCE << " ALREADY THERE------------------------------>"<<file_to_upload<<" TOTAL FAILS "<<total_fails<<endl;
					data_to_send.am_i_server = file_to_upload;
					MPI_Send(&data_to_send, 1, struct_with_three_integers,status.MPI_SOURCE, UPLOAD_FAILED, MPI_COMM_WORLD);

				}else{
					//Insert in HashTable
					//h.insertItem(file_to_upload);

					// Insert new node in file queue
					queue<queue_node> file_queue;
					file_queue.push(make_new_node_upload(status.MPI_SOURCE,num_of_servers));
					hashtable.insert(pair<int, queue<queue_node> >(file_to_upload, file_queue));

					//Print hashtable
					

					// Find Random servers
					vector<int> random_servers;
					random_servers = choose_random_servers(rank,servers_array_used_for_leader_election,num_of_servers);
					int shortest_path;
					for(int i=0;i<random_servers.size();i++){
						//cout<<"------------"<<random_servers[i]<<"----------";
					 	data_to_send.am_i_server = file_to_upload; 
					 	data_to_send.left_neighbour_rank = random_servers[i];
					 	// 	for(int i = 0; i<who_are_connected_with_me.size(); i++)
							// 	cout<<red<<who_are_connected_with_me[i]<<"-"<<reset;
							// cout<<endl;
							//(contains(who_are_connected_with_me,random_servers[i]))
							
						if(std::count(who_are_connected_with_me.begin(), who_are_connected_with_me.end(), random_servers[i])){
							//cout<<"@@@@@@@@@@ I AM "<<rank<<" and i want to send msg to "<<random_servers[i]<<" and it's directly connected with me"<<endl;
							//send directly to server here
						 	//data_to_send.am_i_server = file_to_upload;
					 		//data_to_send.left_neighbour_rank = random_servers[i];
						 	MPI_Send(&data_to_send, 1, struct_with_three_integers,random_servers[i], UPLOAD, MPI_COMM_WORLD);
						}else{
							if(random_servers[i] == my_data.my_left_neighbour){
								//cout<<"@@@@@@@@@@ I AM "<<rank<<" and i want to send msg to "<<random_servers[i]<<" and i will NOT need help from so i will forward to "<<my_data.my_left_neighbour<<endl;
								// data_to_send.am_i_server = file_to_upload;
							 	// data_to_send.left_neighbour_rank = random_servers[i];
							 	MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, UPLOAD, MPI_COMM_WORLD);
							}else{
							// find_shortest_path will return rank of leader to tell him to use the ring or the id of server to use
							shortest_path=find_shortest_path(rank,random_servers[i],num_of_servers,servers_array_used_for_leader_election,who_are_connected_with_me);
							if(shortest_path == rank){
								//cout<<"@@@@@@@@@@ I AM "<<rank<<" and i want to send msg to "<<random_servers[i]<<" and i will NOT need help from so i will forward to "<<my_data.my_left_neighbour<<endl;
								// data_to_send.am_i_server = file_to_upload;
							 	// data_to_send.left_neighbour_rank = random_servers[i];
							 	MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, UPLOAD, MPI_COMM_WORLD);
							}else{
								//cout<<"@@@@@@@@@@ I AM "<<rank<<" and i want to send msg to "<<random_servers[i]<<" and i will need help from "<<shortest_path<<endl;
								// data_to_send.am_i_server = file_to_upload;
							 	// data_to_send.left_neighbour_rank = random_servers[i];
							 	MPI_Send(&data_to_send, 1, struct_with_three_integers,shortest_path, UPLOAD, MPI_COMM_WORLD);
							}
							}
							
						}
					//	find_shortest_path(rank,random_servers[i],num_of_servers,servers_array_used_for_leader_election,who_are_connected_with_me);
					// 	data_to_send.am_i_server = file_to_upload;
					// 	MPI_Send(&data_to_send, 1, struct_with_three_integers,status.MPI_SOURCE, UPLOAD_FAILED, MPI_COMM_WORLD);
					}
					//cout<<endl;
					//cout<<"RANDOM SERVERS";
					//for(int i=0;i<random_servers.size();i++){
					//	cout<<" "<<random_servers[i]<<" ";
						//find_shortest_path(rank,random_servers[i],num_of_servers,servers_array_used_for_leader_election,who_are_connected_with_me);
					// 	data_to_send.am_i_server = file_to_upload;
					// 	MPI_Send(&data_to_send, 1, struct_with_three_integers,status.MPI_SOURCE, UPLOAD_FAILED, MPI_COMM_WORLD);
					//}
					//cout<<endl;


				}
				//h.deleteItem(28); 
				// h.displayHash(); 
				// cout<<"REQUEST QUEUE"<<endl;
				// while(!requests_queue.empty()){
				// 	cout<<"-------------------------------------------------------->>>>>>"<<(requests_queue.front()).file_id<<endl;
				// 	requests_queue.pop();
				// }
			}
			if((status.MPI_TAG == UPLOAD_FAILED)){
				cout << red <<"CLIENT "<<rank<<" FAILED TO UPLOAD "<<data_to_send.am_i_server << reset <<endl;
				//cout << red << "red text" << reset << endl;
				//print_hashtable(hashtable);
			}
			if((status.MPI_TAG == UPLOAD_ACK)){
				if(rank == leader_id){
					// diecrease server remaining of file
					int file = data_to_send.am_i_server;
					test++;
					//int what_to_do = test;
					//int what_to_do = dicrease_remaining_servers(hashtable,file);
					int what_to_do = 0;
					//print_hashtable(hashtable);
					//cout<<"=============================================================== "<<what_to_do<<"====="<<file<<endl;
					/*
						int what_to_do;
						queue<queue_node> file_queue = hashtable.find(file)->second;
						int temp = (file_queue.front()).remaining_servers;
						temp--;
						(file_queue.front()).remaining_servers--;
						cout<<">?????????????????????????????????????????? "<<file<<"-----"<<(file_queue.front()).remaining_servers<<endl;
					*/

					// Dicrease remaining servers by one
					( (hashtable.find(file)->second).front() ).remaining_servers--;
					if( ((hashtable.find(file)->second).front() ).remaining_servers == 0) what_to_do = ((hashtable.find(file)->second).front()).client_id_made_request;

					if( what_to_do != 0){
						// remaining servers are 0
						data_to_send.am_i_server = file;
						//cout<<"CLIENT WANTED TO UPLAOD FILE ----------------------------------------------->"<<what_to_do<<" "<<file<<endl;
						MPI_Send(&data_to_send, 1, struct_with_three_integers,what_to_do, UPLOAD_OK, MPI_COMM_WORLD);
						//pop_front_from_queue(hashtable,file);
						hashtable.find(file)->second.pop();
						//print_hashtable(hashtable);
						
						if (check_hashtable(hashtable,file)){
							execute_retrieve(hashtable,file,rank,servers_array_used_for_leader_election,who_are_connected_with_me,my_data.my_left_neighbour);
						} //cout<<endl;//call execute retrieve
						else if(check_hashtable_for_update(hashtable,file)){
							cout<<yellow<<"I AM LEADERR "<<rank<<" AND CLIENT "<<return_who_made_request(hashtable,file)<<" WANTS TO UPDATE FILE "<<file<<reset<<endl;
							//hashtable.find(file)->second.pop();
							int version = ((hashtable.find(file)->second).front()).help_version;
							//execute_update_check(file,1,1,my_data.my_left_neighbour);
							//execute_legit_update(hashtable,file,rank,servers_array_used_for_leader_election,who_are_connected_with_me,my_data.my_left_neighbour,version);
						    data_to_send.am_i_server = file;
						    data_to_send.right_neighbour_rank = version;
						    data_to_send.left_neighbour_rank = 1;
						    //MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, VERSION_CHECK, MPI_COMM_WORLD);
						}

					}

				}else{
					//forwqard message
					if(my_data.am_i_lucky){// I am directly connected to leader
						//data_to_send.am_i_server = data_to_send.am_i_server;
						MPI_Send(&data_to_send, 1, struct_with_three_integers,leader_id, UPLOAD_ACK, MPI_COMM_WORLD);
					}else{
						//data_to_send.am_i_server = data_to_send.am_i_server;
						MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, UPLOAD_ACK, MPI_COMM_WORLD);
					}
				}
				//cout<<"CLIENT "<<rank<<" FAILED TO UPLOAD "<<data_to_send.am_i_server<<endl;
			}
			if((status.MPI_TAG == UPLOAD_OK) ){
				//( (hashtable.find(file)->second).front() ).remaining_servers--;
				//cout<<yellow<<"------------------------------------------>BEFORE "<<my_data_as_client.file_version<<" FILE "<<data_to_send.am_i_server<<reset<<endl;
				//my_data_as_client.file_version++;
				icrease_file_version(local_data_for_file_client_keeps,data_to_send.am_i_server);
				//cout<<green<<"------------------------------------------>AFTER "<<my_data_as_client.file_version<<" FILE "<<data_to_send.am_i_server<<reset<<endl;
				cout << "CLIENT " << rank <<" UPLOADED "<<data_to_send.am_i_server<<endl;
				//print_hashtable(hashtable);
				//cout<<"QUEUE"<<endl;
				//print_queue(hashtable.find(data_to_send.am_i_server)->second);
				//print_hashtable(hashtable);


			}
			if((status.MPI_TAG == RETRIEVE)){
				if(my_data.am_i_server == 0){
					//cout<<green<<"I AM "<<rank<<" CLIENT AND I WANT TO RETRIEVE FILE "<<data_to_send.am_i_server<<" LEADER IS "<<my_data_as_client.leader_id<<reset<<endl;
					send_retrieve_data_to_client.file_id = data_to_send.am_i_server;
					MPI_Send(&send_retrieve_data_to_client, 1, struct_with_three_integers,my_data_as_client.leader_id, SENT_RETRIEVE_FILE_TO_LEADER, MPI_COMM_WORLD);


				}else{
					// Server received RETRIEVE message
					int destination = data_to_send.left_neighbour_rank;
					int file_to_retrieve = data_to_send.am_i_server;
					if (destination != rank){//I must forward this message in the ring
				      	data_to_send.am_i_server = file_to_retrieve;
      					data_to_send.left_neighbour_rank = destination;
						MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, RETRIEVE, MPI_COMM_WORLD);
					}else{// I was the destination 
						//local_data_for_file_server_keeps
						// data_to_hold
						// Search in my local data
						int what_to_do = search_for_file_in_local_data_server_keeps(local_data_for_file_server_keeps,file_to_retrieve);
						if( what_to_do != -42){ // file version
							// i found the file in my local data
							// send retrieve ack to leader
							data_to_send.am_i_server = file_to_retrieve;
      						data_to_send.left_neighbour_rank = what_to_do;
							if(my_data.am_i_lucky)// I am directly connected to leader
								MPI_Send(&data_to_send, 1, struct_with_three_integers,leader_id, RETRIEVE_ACK, MPI_COMM_WORLD);
							else // forward message
								MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, RETRIEVE_ACK, MPI_COMM_WORLD);
						}else{
							// i have no record for this file
							data_to_send.am_i_server = file_to_retrieve;
      						data_to_send.left_neighbour_rank = 0; // file version
							if(my_data.am_i_lucky)// I am directly connected to leader
								MPI_Send(&data_to_send, 1, struct_with_three_integers,leader_id, RETRIEVE_ACK, MPI_COMM_WORLD);
							else // forward message
								MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, RETRIEVE_ACK, MPI_COMM_WORLD);
						}
					}
				}
			}
			if((status.MPI_TAG == SENT_RETRIEVE_FILE_TO_LEADER)){
				//ONLY LEADER COMES HERE

				//print_hashtable(hashtable);
				//hashtable.find( key ) != hashtable.end()
				
				// queue<queue_node> queue_of_file = hashtable.find(data_to_send.am_i_server)->second;
				if(hashtable.count(data_to_send.am_i_server) != 0){//exists in hashtable
					//cout<<green<<"I AM "<<rank<<"  AND CLIENT "<<status.MPI_SOURCE<< " WANTS TO RETRIEVE FILE "<<data_to_send.am_i_server<<reset<<endl;
					queue_node new_node = make_new_node_retrieve(status.MPI_SOURCE,num_of_servers);
					(hashtable.find(data_to_send.am_i_server)->second).push(new_node);
					//print_hashtable(hashtable);

					//queue of file to retrieve
					//queue<queue_node> q = hashtable.find(data_to_send.am_i_server)->second;
					if (check_hashtable(hashtable,data_to_send.am_i_server)) 
						execute_retrieve(hashtable,data_to_send.am_i_server,rank,servers_array_used_for_leader_election,who_are_connected_with_me,my_data.my_left_neighbour);
					if (check_hashtable_for_update(hashtable,data_to_send.am_i_server)){
						//cout<<green<<"I AM LEADER "<<rank<<" AND CLIENT "<<return_who_made_request(hashtable,data_to_send.am_i_server)<<" WANTS TO UPDATE FILE "<<data_to_send.am_i_server<<reset<<endl;
						//hashtable.find(data_to_send.am_i_server)->second.pop();
						int version = ((hashtable.find(data_to_send.am_i_server)->second).front()).help_version;
						execute_update_check(data_to_send.am_i_server,version,1,my_data.my_left_neighbour);
					}
					//cout<<endl;//call execute retrieve
					
					//check_hashtable(hashtable,data_to_send.am_i_server);
				}else{//couldn't find file in hashtable
					send_retrieve_data_to_client.file_id  = data_to_send.am_i_server;
					MPI_Send(&send_retrieve_data_to_client, 1, struct_with_three_integers,status.MPI_SOURCE, RETRIEVE_FAILED, MPI_COMM_WORLD);
				}
				

				//print_hashtable(hashtable);
			}
			if((status.MPI_TAG == RETRIEVE_FAILED)){

				cout << red <<"CLIENT "<<rank<<" FAILED TO RETRIEVE "<<data_to_send.am_i_server << reset <<endl;
								total_retrieved_failed++;
				//cout<<red<<"---------------------------------------------------------------> FAILED "<<total_retrieved_failed<<reset<<endl;
			}
			if((status.MPI_TAG == RETRIEVE_ACK)){
				if(rank == leader_id){//i am leader
					int file = data_to_send.am_i_server;
					int received_file_version = data_to_send.left_neighbour_rank;
					int most_recent_version;
					int what_to_do = 0;
					( (hashtable.find(file)->second).front() ).remaining_servers--;
					if( ((hashtable.find(file)->second).front() ).help_version < received_file_version){
						((hashtable.find(file)->second).front() ).help_version = received_file_version;
						most_recent_version = received_file_version;
					}else{
						most_recent_version = ((hashtable.find(file)->second).front() ).help_version;
					}

					if( ((hashtable.find(file)->second).front() ).remaining_servers == 0) what_to_do = ((hashtable.find(file)->second).front()).client_id_made_request;

					if (what_to_do != 0){
						// remaining servers are 0
						data_to_send.am_i_server = file;
						data_to_send.left_neighbour_rank = most_recent_version;
						//cout<<"CLIENT WANTED TO UPLAOD FILE ----------------------------------------------->"<<what_to_do<<" "<<file<<endl;
						//print_hashtable(hashtable);
						MPI_Send(&data_to_send, 1, struct_with_three_integers,what_to_do, RETRIEVE_OK, MPI_COMM_WORLD);
						//pop_front_from_queue(hashtable,file);
						hashtable.find(file)->second.pop();
						if(!(hashtable.find(file)->second).empty()){
							if (check_hashtable_for_update(hashtable,file)){
								//cout<<green<<"I AM LEADER "<<rank<<" AND CLIENT "<<return_who_made_request(hashtable,data_to_send.am_i_server)<<" WANTS TO UPDATE FILE "<<data_to_send.am_i_server<<reset<<endl;
								//hashtable.find(file)->second.pop();
								int version = ((hashtable.find(file)->second).front()).help_version;
								execute_update_check(file,version,1,my_data.my_left_neighbour);
							}
							else
								execute_retrieve(hashtable,file,rank,servers_array_used_for_leader_election,who_are_connected_with_me,my_data.my_left_neighbour);
						}
						//print_hashtable(hashtable);

					}
				}else{
					if(my_data.am_i_lucky)// I am directly connected to leader
						MPI_Send(&data_to_send, 1, struct_with_three_integers,leader_id, RETRIEVE_ACK, MPI_COMM_WORLD);
					else//forward message
						MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, RETRIEVE_ACK, MPI_COMM_WORLD);
				}
				//cout << red <<"CLIENT "<<rank<<" FAILED TO RETRIEVE "<<data_to_send.am_i_server << reset <<endl;
			}
			if((status.MPI_TAG == RETRIEVE_OK) ){
				cout << "CLIENT " << rank <<" RETRIEVED VERSION "<<data_to_send.left_neighbour_rank<<" OF "<<data_to_send.am_i_server<<endl;
				total_retrieved++;
				///cout<<green<<"---------------------------------------------------------------> RETRIEVED "<<total_retrieved<<reset<<endl;
			}
			if((status.MPI_TAG == UPDATE) ){
				if(my_data.am_i_server == 0){
					//cout<<green<<"I AM CLIENT "<<rank<<" AND I WANT TO UPDATE FILE "<<data_to_send.am_i_server<<reset<<endl;				
					data_to_send.am_i_server = data_to_send.am_i_server;
					int version_i_have = search_for_file_in_local_data_client_keeps(local_data_for_file_client_keeps,data_to_send.am_i_server);
					if( version_i_have != -42){
						data_to_send.left_neighbour_rank = version_i_have;
					}else{
						data_to_send.left_neighbour_rank = 0;
					}
					
					MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data_as_client.leader_id, SENT_UPDATE_FILE_TO_LEADER, MPI_COMM_WORLD);
				}else{
					int destination = data_to_send.left_neighbour_rank;
					int file_id = data_to_send.am_i_server;
					int version_client_has = data_to_send.right_neighbour_rank;
					if (destination != rank){
						data_to_send.left_neighbour_rank = destination;
						data_to_send.right_neighbour_rank = version_client_has;
						data_to_send.am_i_server = file_id;
						MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, UPDATE, MPI_COMM_WORLD);
					}else{
						int what_to_do = search_for_file_in_local_data_server_keeps(local_data_for_file_server_keeps,file_id);
						if (what_to_do == -42){ // i have no record of this file
							data_to_hold.file_id = file_id;
							data_to_hold.version = version_client_has+1;
							local_data_for_file_server_keeps.push_back(data_to_hold);
						}else{
							if( what_to_do <= version_client_has){
								change_version_of_file(local_data_for_file_server_keeps,file_id,version_client_has+1);
							}
						}
			            data_to_send.am_i_server = file_id;
            			MPI_Send(&data_to_send, 1, struct_with_three_integers,leader_id, UPDATE_ACK, MPI_COMM_WORLD);

					}
				}
			}
			if((status.MPI_TAG == SENT_UPDATE_FILE_TO_LEADER) ){
				//print_hashtable(hashtable);
				int client_version_of_file = data_to_send.left_neighbour_rank;
				if( (client_version_of_file == 0) || (hashtable.count(data_to_send.am_i_server) == 0) ){
					// This file is not in the hashtable OR client has no version of this file
					data_to_send.am_i_server = data_to_send.am_i_server;
					MPI_Send(&data_to_send, 1, struct_with_three_integers,status.MPI_SOURCE, UPDATE_FAILED, MPI_COMM_WORLD);
					hashtable.find(data_to_send.am_i_server)->second.pop();
				}else{
					// insert new node in hash table
					queue_node new_node = make_new_node_update(status.MPI_SOURCE,num_of_servers,client_version_of_file);
					(hashtable.find(data_to_send.am_i_server)->second).push(new_node);
					//cout<<green<<"I AM LEADER "<<rank<<" AND CLIENT "<<status.MPI_SOURCE<<" WANTS TO UPDATE FILE "<<data_to_send.am_i_server<<" WITH VERSION "<<client_version_of_file<<reset<<endl;
					// if (check_hashtable_for_update(hashtable,data_to_send.am_i_server)) 
					// 	cout<<green<<"I AM LEADER "<<rank<<" AND CLIENT "<<status.MPI_SOURCE<<" WANTS TO UPDATE FILE "<<data_to_send.am_i_server<<" WITH VERSION "<<client_version_of_file<<reset<<endl;
						//if(!(hashtable.find(data_to_send.am_i_server)->second).empty()){
							if (check_hashtable_for_update(hashtable,data_to_send.am_i_server)){
								cout<<green<<"I AM LEADER "<<rank<<" AND CLIENT "<<return_who_made_request(hashtable,data_to_send.am_i_server)<<" WANTS TO UPDATE FILE "<<data_to_send.am_i_server<<reset<<endl;
								//hashtable.find(data_to_send.am_i_server)->second.pop();
								execute_update_check(data_to_send.am_i_server,client_version_of_file,1,my_data.my_left_neighbour);
							}else
								execute_retrieve(hashtable,data_to_send.am_i_server,rank,servers_array_used_for_leader_election,who_are_connected_with_me,my_data.my_left_neighbour);
						//}//execute_retrieve(hashtable,data_to_send.am_i_server,rank,servers_array_used_for_leader_election,who_are_connected_with_me,my_data.my_left_neighbour);
				}
				//print_hashtable(hashtable);
								
				// data_to_send.am_i_server = data_to_send.am_i_server;
				// data_to_send.left_neighbour_rank = my_data_as_client.file_version;
				// MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data_as_client.leader_id, SENT_UPDATE_FILE_TO_LEADER, MPI_COMM_WORLD);
			}
			if((status.MPI_TAG == UPDATE_FAILED) ){
				cout << red <<"CLIENT "<<rank<<" UPDATE FAILED "<<data_to_send.am_i_server << reset <<endl;
			}
			if((status.MPI_TAG == VERSION_CHECK) ){
				if (rank == leader_id){
					int file_id = data_to_send.am_i_server;
					int version_client_has = data_to_send.right_neighbour_rank;
					int flag_bit = data_to_send.left_neighbour_rank;

					int who_made_request = return_who_made_request(hashtable,file_id);
					if (flag_bit == 0){
					    data_to_send.am_i_server = file_id;
					    MPI_Send(&data_to_send, 1, struct_with_three_integers,who_made_request, VERSION_OUTDATED, MPI_COMM_WORLD);
						// remove from queue 
						hashtable.find(file_id)->second.pop();
						if(!(hashtable.find(file_id)->second).empty()){
							if (check_hashtable_for_update(hashtable,file_id)){
								//cout<<yellow<<"I AM LEADER "<<rank<<" AND CLIENT "<<return_who_made_request(hashtable,data_to_send.am_i_server)<<" WANTS TO UPDATE FILE "<<data_to_send.am_i_server<<reset<<endl;
								//hashtable.find(file)->second.pop();
								//int version = ((hashtable.find(file_id)->second).front()).help_version;
								execute_update_check(file_id,version_client_has,1,my_data.my_left_neighbour);
								//execute_legit_update(hashtable,file_id,rank,servers_array_used_for_leader_election,who_are_connected_with_me,my_data.my_left_neighbour,version_client_has);
							}
							else
								execute_retrieve(hashtable,file_id,rank,servers_array_used_for_leader_election,who_are_connected_with_me,my_data.my_left_neighbour);
						}
						// CONTINUE TO THE NEXT REQUEST OF FILE ???????????????????????
						/*
						if(!(hashtable.find(data_to_send.am_i_server)->second).empty()){
							if (check_hashtable_for_update(hashtable,data_to_send.am_i_server)){
								cout<<green<<"I AM LEADER "<<rank<<" AND CLIENT "<<return_who_made_request(hashtable,data_to_send.am_i_server)<<" WANTS TO UPDATE FILE "<<data_to_send.am_i_server<<reset<<endl;
								//hashtable.find(data_to_send.am_i_server)->second.pop();
								execute_update_check(data_to_send.am_i_server,client_version_of_file,1,my_data.my_left_neighbour);
							}else
								execute_retrieve(hashtable,data_to_send.am_i_server,rank,servers_array_used_for_leader_election,who_are_connected_with_me,my_data.my_left_neighbour);
						}
						*/
					}else{
						cout<<yellow<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<reset<<endl;
						execute_legit_update(hashtable,file_id,rank,servers_array_used_for_leader_election,who_are_connected_with_me,my_data.my_left_neighbour,version_client_has,num_of_servers);
					}

				}else{
					int file_id = data_to_send.am_i_server;
					int version_client_has = data_to_send.right_neighbour_rank;
					int flag_bit = data_to_send.left_neighbour_rank;

					if (flag_bit){
						int version_i_have = search_for_file_in_local_data_server_keeps(local_data_for_file_server_keeps,file_id);
						if ((version_i_have == -42) || (version_i_have <= version_client_has)){
							// forward message to my left neigbour
							execute_update_check(file_id,version_client_has,1,my_data.my_left_neighbour);
						}else{
							cout<<red<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<reset<<endl;
						}
					}else{
						if(my_data.am_i_lucky){// i am connected to the server
							    data_to_send.am_i_server = file_id;
    data_to_send.right_neighbour_rank = version_client_has;
    data_to_send.left_neighbour_rank = 0;
    MPI_Send(&data_to_send, 1, struct_with_three_integers,leader_id, VERSION_CHECK, MPI_COMM_WORLD);
							//execute_update_check(file_id,version_client_has,0,leader_id);
						}else{//forward message in ring
														    data_to_send.am_i_server = file_id;
    data_to_send.right_neighbour_rank = version_client_has;
    data_to_send.left_neighbour_rank = 0;
    MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, VERSION_CHECK, MPI_COMM_WORLD);
							//execute_update_check(file_id,version_client_has,0,my_data.my_left_neighbour);
						}

					}
				}
				
			}
			if((status.MPI_TAG == VERSION_OUTDATED) ){
				cout<<yellow<<"CLIENT "<<rank<<" CANNOT UPDATE "<< data_to_send.am_i_server <<" WITHOUT MOST RECENT VERSION"<<reset<<endl;
			}
	      if((status.MPI_TAG == UPDATE_ACK)){
	        if(rank == leader_id){//i am leader
	          int file = data_to_send.am_i_server;
	          int what_to_do = 0;

	          ( (hashtable.find(file)->second).front() ).remaining_servers--;


	          if( ((hashtable.find(file)->second).front() ).remaining_servers == 0) what_to_do = ((hashtable.find(file)->second).front()).client_id_made_request;

	          if (what_to_do != 0){
	            // remaining servers are 0
	            data_to_send.am_i_server = file;
	            //cout<<"CLIENT WANTED TO UPLAOD FILE ----------------------------------------------->"<<what_to_do<<" "<<file<<endl;
	            //print_hashtable(hashtable);
	            MPI_Send(&data_to_send, 1, struct_with_three_integers,what_to_do, UPDATE_OK, MPI_COMM_WORLD);
	            //pop_front_from_queue(hashtable,file);
	            hashtable.find(file)->second.pop();
				//if(!(hashtable.find(file)->second).empty()){
					if (check_hashtable_for_update(hashtable,file)){
						//print_hashtable(hashtable);
					//cout<<yellow<<"I AM LEADER "<<rank<<" AND CLIENT "<<return_who_made_request(hashtable,data_to_send.am_i_server)<<" WANTS TO UPDATE FILE "<<data_to_send.am_i_server<<reset<<endl;
					//hashtable.find(file)->second.pop();
					//int version = ((hashtable.find(file_id)->second).front()).help_version;
					//int version = ((hashtable.find(file)->second).front()).help_version;
					//execute_update_check(file,version,1,my_data.my_left_neighbour);
					//execute_legit_update(hashtable,file,rank,servers_array_used_for_leader_election,who_are_connected_with_me,my_data.my_left_neighbour,version,num_of_servers);
					}
					else
						execute_retrieve(hashtable,file,rank,servers_array_used_for_leader_election,who_are_connected_with_me,my_data.my_left_neighbour);
				//}
	            /*if(!(hashtable.find(file)->second).empty()){
	              if (check_hashtable_for_update(hashtable,file)){
	                //cout<<green<<"I AM LEADER "<<rank<<" AND CLIENT "<<return_who_made_request(hashtable,data_to_send.am_i_server)<<" WANTS TO UPDATE FILE "<<data_to_send.am_i_server<<reset<<endl;
	                //hashtable.find(file)->second.pop();
	              }
	              else
	                execute_retrieve(hashtable,file,rank,servers_array_used_for_leader_election,who_are_connected_with_me,my_data.my_left_neighbour);
	            }*/

	            //print_hashtable(hashtable);

	          }
	        }else{
	          if(my_data.am_i_lucky)// I am directly connected to leader
	            MPI_Send(&data_to_send, 1, struct_with_three_integers,leader_id, UPDATE_ACK, MPI_COMM_WORLD);
	          else//forward message
	            MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, UPDATE_ACK, MPI_COMM_WORLD);
	        }
	        //cout << red <<"CLIENT "<<rank<<" FAILED TO RETRIEVE "<<data_to_send.am_i_server << reset <<endl;
	      }
	      if((status.MPI_TAG == UPDATE_OK)){
	      	//CLIENT<client_rank> UPDATED <FILE_ID>
	      	// for (int i = 0; i < local_data_for_file_client_keeps.size(); ++i)
	      	// {
	      	// 	if (local_data_for_file_client_keeps[i].file_id == data_to_send.am_i_server) local_data_for_file_client_keeps[i].version++;
	      	// }
	      	cout<<green<<"CLIENT "<<rank<<" UPDATED "<<data_to_send.am_i_server<<reset<<endl;
	      }


		}
		
	}
//}
	MPI_Finalize();
}


