#include "header.h"

void create_map_with_commands(int index,string str){ 
    // Create new command
	  commands_t new_command;

    // Used to split string around spaces. 
    istringstream ss(str); 

    int tmp = 0;
  
    // Read the first word -> Type of command
    string word; 
    ss >> word; 
		new_command.type = word;
    //cout << "TYPE = " << new_command.type <<endl;

		if(((new_command.type).compare("SERVER")) == 0){

			ss >> word; 
			stringstream to_int(word); 
			to_int >> tmp; 
			new_command.first = tmp;
			
			ss >> word; 
			stringstream to_int2(word); 
			to_int2 >> tmp; 
			new_command.second = tmp;
			
			ss >> word; 
			stringstream to_int3(word); 
			to_int3 >> tmp; 
			new_command.third = tmp;

      
      //you_are_server_msg data_to_send;
      data_to_send.am_i_server = 1;
      data_to_send.left_neighbour_rank = new_command.second;
      data_to_send.right_neighbour_rank = new_command.third;


      total_servers.push_back(new_command.first);
      MPI_Send(&data_to_send, 1, struct_with_three_integers,new_command.first, YOU_ARE_SERVER, MPI_COMM_WORLD);
      flag =1;
			
		}
		if(((new_command.type).compare("START_LEADER_ELECTION")) == 0){
      // cout<<"I AM IN LEADER ELECTION"<<endl;
      // cout <<"All servers are->";
     //  for (int i = 0; i < total_servers.size()-1; i++) {
     //    cout <<" "<< total_servers[i] << " ";              
      
     //    //cout << endl;
     //    third_stage = 1;
     //    //second_stage=0;
  			// // new_command.first = -1;
  			// // new_command.second = -1;
  			// // new_command.third = -1;
     //    data_to_send.am_i_server = 0;
     //    data_to_send.left_neighbour_rank = -1;
     //    data_to_send.right_neighbour_rank = -1;
     //    cout << "000000000000000000000000000000000000000"<<endl;
     //    MPI_Send(&data_to_send, 1, struct_with_three_integers,total_servers[i], TAG, MPI_COMM_WORLD);
     //    MPI_Recv(&ack, 1, ack_res, MPI_ANY_SOURCE, TAG, MPI_COMM_WORLD, &status);
     //    cout << endl << "I received ack from "<< status.MPI_SOURCE << endl<< endl;
     //  }
		}
		// if((((new_command.type).compare("UPLOAD")) == 0) || (((new_command.type).compare("RETRIEVE")) == 0) || (((new_command.type).compare("UPDATE")) == 0)){
			
  //     ss >> word; 
		// 	stringstream to_int(word); 
		// 	to_int >> tmp; 
		// 	new_command.first = tmp;
			
		// 	ss >> word; 
		// 	stringstream to_int2(word); 
		// 	to_int2 >> tmp; 
		// 	new_command.second = tmp;
		// 	new_command.third = -1;
		// 	cout<<new_command.type<<" "<<new_command.first<<" "<<new_command.second<<endl;
		// }
		if((((new_command.type).compare("UPLOAD")) == 0) ){
			
      		ss >> word; 
			stringstream to_int(word); 
			to_int >> tmp; 
			new_command.first = tmp;
			
			ss >> word; 
			stringstream to_int2(word); 
			to_int2 >> tmp; 
			new_command.second = tmp;
			new_command.third = -1;
			//cout<<new_command.type<<" "<<new_command.first<<" "<<new_command.second<<endl;
			
		    send_upload_data_to_client.file_id_to_upload=new_command.second;
		    //new_command.first
			MPI_Send(&send_upload_data_to_client, 1, struct_with_three_integers,new_command.first, UPLOAD, MPI_COMM_WORLD);
		}


		if((((new_command.type).compare("RETRIEVE")) == 0) ){
			
      		ss >> word; 
			stringstream to_int(word); 
			to_int >> tmp; 
			new_command.first = tmp;
			
			ss >> word; 
			stringstream to_int2(word); 
			to_int2 >> tmp; 
			new_command.second = tmp;
			new_command.third = -1;
			
			send_retrieve_data_to_client.file_id = new_command.second;
			MPI_Send(&send_retrieve_data_to_client, 1, struct_with_three_integers,new_command.first, RETRIEVE, MPI_COMM_WORLD);
			//cout<<new_command.type<<" "<<new_command.first<<" "<<new_command.second<<endl;
		}
		
		if((((new_command.type).compare("UPDATE")) == 0) ){
			
      		ss >> word; 
			stringstream to_int(word); 
			to_int >> tmp; 
			new_command.first = tmp;
			
			ss >> word; 
			stringstream to_int2(word); 
			to_int2 >> tmp; 
			new_command.second = tmp;
			new_command.third = -1;
			send_update_data_to_client.file_id = new_command.second;
			MPI_Send(&send_update_data_to_client, 1, struct_with_three_integers,new_command.first, UPDATE, MPI_COMM_WORLD);
			//cout<<new_command.type<<" "<<new_command.first<<" "<<new_command.second<<endl;
		}

		if(((new_command.type).compare("LEAVE")) == 0){
			ss >> word; 
			stringstream to_int(word); 
			to_int >> tmp; 
			new_command.first = tmp;

			new_command.second = -1;
			new_command.third = -1;
		}
    
    // insert command in map commands
		commands.insert(pair<int, commands_t>(index, new_command));
} 

void start_reading_test_file(char *file){
			 string line;
			 int index=0;
		  ifstream myfile (file);
		  if (myfile.is_open()){
		    while ( getline (myfile,line) )
		    {
		      create_map_with_commands(index,line);
		      index++;
		    }
		    myfile.close();
		  }else cout << "Unable to open file";
}

void print_commands(map<int, commands_t> commands){
	map<int, commands_t>::iterator itr; 
	
	for (itr = commands.begin(); itr != commands.end(); ++itr) { 
    	if(((itr->second.type).compare("SERVER")) == 0)
    		cout << '\t' << itr->first << '\t' << itr->second.type << '\t' << itr->second.first<< '\t' << itr->second.second<< '\t' << itr->second.third<< '\n'; 
		else if(((itr->second.type).compare("START_LEADER_ELECTION")) == 0)
			cout << '\t' << itr->first << '\t' << itr->second.type << endl;
		else if ((((itr->second.type).compare("UPLOAD")) == 0) || (((itr->second.type).compare("RETRIEVE")) == 0) || (((itr->second.type).compare("UPDATE")) == 0))
			cout << '\t' << itr->first << '\t' << itr->second.type << '\t' << itr->second.first<< '\t' << itr->second.second<< '\n';
		else if(((itr->second.type).compare("LEAVE")) == 0)
			cout << '\t' << itr->first << '\t' << itr->second.type << '\t' << itr->second.first<< endl;
    	

	}
}

vector<int> choose_lucky_servers(vector<int> to_choose_from,int how_many){ 


	vector<int> return_vector;

	int random;
	for (int i = 0; i < how_many; i++){
		random = (rand() % (to_choose_from.size()));
		return_vector.push_back(to_choose_from[random]);
		to_choose_from.erase(std::remove(to_choose_from.begin(), to_choose_from.end(), to_choose_from[random]), to_choose_from.end());
	}
	return return_vector;
}


bool unique(vector<int> arr){ 
    int n = arr.size(); 
  
    // Put all array elements in a map 

    map<int, int> check;
    for (int i = 0; i < n; i++) { 
        check.insert(pair<int, int>(arr[i], arr[i]));
    } 
    map<int, int>::iterator itr; 
	
	// cout << "MAP CHECK----------------------------------------"<<endl;
	// for (itr = check.begin(); itr != check.end(); ++itr) { 
	// 		cout << '\t' << itr->first << '\t' << itr->second << endl;
    	

	// }
    
  
    // If all elements are distinct, size of check
    // should be same array. 
    return (check.size() == arr.size()); 
} 

int contains(std::vector<int> v,int key){
	if (std::binary_search(v.begin(), v.end(), key))
		return 1;
	else
		return 0;
}

vector<int> choose_random_servers(int rank,int *arr,int size){

	vector<int> return_vector;
	vector<int> choose_random_servers;
	for(int i=0;i<size;i++){
		if(arr[i]!= rank)
			choose_random_servers.push_back(arr[i]);
	}

	int random;
	for (int i = 0; i < ((num_of_servers-1)/2) + 1; i++){
		random = (rand() % (choose_random_servers.size()));
		return_vector.push_back(choose_random_servers[random]);
		choose_random_servers.erase(std::remove(choose_random_servers.begin(), choose_random_servers.end(), choose_random_servers[random]), choose_random_servers.end());
	}
	return return_vector;
}

int find_shortest_path(int my_rank,int destination,int num_of_servers,int *leaders_server_array,vector<int> directly_connected){

	int hops_without_help=1000;
	int hops_with_help=1000;
	int help_from_who=0;
	int dest_pos=10000;
	int direct_server_pos;

	// for(int i = 0; i<directly_connected.size(); i++)
	// 	cout<<red<<directly_connected[i]<<" "<<reset;
	// cout<<endl;
	// Search in leaders array and num_of_servers-(i+1) is the number 
	// of hops leader will need using ring
	for(int i=0;i<num_of_servers;i++){
		if(leaders_server_array[i] == destination){
			hops_without_help = num_of_servers-(i+1);
			break;
		}
	}

	// Save position of destination server
	// When you find the nearest server who is in directly_connected vector
	// save his position and his value
	for(int i=0;i<num_of_servers;i++){
		//cout<<yellow<<leaders_server_array[i]<<" "<<reset;
		if(destination == leaders_server_array[i]){
			dest_pos = i;
			if(std::count(directly_connected.begin(), directly_connected.end(), leaders_server_array[dest_pos+1])){
			//if(contains(directly_connected,leaders_server_array[dest_pos+1])){
				direct_server_pos=dest_pos+1;
				help_from_who = leaders_server_array[dest_pos+1];
				break;
			}
		}
		if (std::count(directly_connected.begin(), directly_connected.end(), leaders_server_array[i])){
		//if(contains(directly_connected,leaders_server_array[i])){
			direct_server_pos=i;
			help_from_who = leaders_server_array[i];
			//if (leaders_server_array[dest_pos] == leaders_server_array[direct_server_pos-1]) break;
			if (i >= dest_pos) break;

		}
		
	}
	
	// Calculate hops will need from the nearest connected  server 
	// to the destination
	hops_with_help = (direct_server_pos - dest_pos)+1;

	// If hops_with_help <= 0 means that destination server is right neighbour(or after) of connected server 
	// so it's fastest to use the ring
	// Only if hops_with_help < hops_without_help use the connected server
	if((hops_with_help < 0) || (!hops_with_help)){
		return my_rank;
	}else{
		if(hops_with_help < hops_without_help)
			return help_from_who;
		else
			return my_rank;
	}

	// if(hops_with_help < hops_without_help && hops_with_help != 0)
	// 	return help_from_who;
	
	return my_rank;
	// Return rank of leader to tell him to use the ring or the id of server to use
}

int search_for_file_in_local_data_server_keeps(vector<local_data_for_file> local_data_for_file_server_keeps,int file_id){
	for (int i = 0; i < local_data_for_file_server_keeps.size(); ++i){
		if(local_data_for_file_server_keeps[i].file_id == file_id) return local_data_for_file_server_keeps[i].version;
	}

	return -42;
}

int search_for_file_in_local_data_client_keeps(vector<local_data_for_file_client> local_data_for_file_client_keeps,int file_id){
	for (int i = 0; i < local_data_for_file_client_keeps.size(); ++i){
		if(local_data_for_file_client_keeps[i].file_id == file_id) return local_data_for_file_client_keeps[i].version;
	}

	return -42;
}

void icrease_file_version(vector<local_data_for_file_client> local_data_for_file_client_keeps,int file_id){
	for (int i = 0; i < local_data_for_file_client_keeps.size(); ++i){
		if(local_data_for_file_client_keeps[i].file_id == file_id) local_data_for_file_client_keeps[i].version++;
	}

	//return -42;
}

void change_version_of_file(vector<local_data_for_file> local_data_for_file_server_keeps,int file_id,int new_version){
  for (int i = 0; i < local_data_for_file_server_keeps.size(); ++i){
    if(local_data_for_file_server_keeps[i].file_id == file_id) local_data_for_file_server_keeps[i].version = new_version;
  }

 
}

// int return file_version_client_keeps(vector<local_data_for_file_client> local_data_for_file_client_keeps,int file_id){
// 	for (int i = 0; i < local_data_for_file_client_keeps.size(); ++i){
// 		if(local_data_for_file_client_keeps[i].file_id == file_id) return local_data_for_file_client_keeps[i].version++;
// 	}
// }


// int test2(map<int, queue<queue_node> > hashtable,int file){
// 	int what_to_do = 0
// 						((hashtable.find(file)->second).front()).remaining_servers--;
// 					if(((hashtable.find(file)->second).front()).remaining_servers == 0) what_to_do = ((hashtable.find(file)->second).front()).client_id_made_request;
// 					return 0;
// }



// int find_shortest_path(int my_rank,int destination,int num_of_servers,int *leaders_server_array,vector<int> directly_connected){
// 					cout<<"I AM "<<my_rank<<" AND I WANT TO FIND SHORTEST ROOT FOR SERVER "<<destination<<endl;
// 					cout<<endl;
// 					int hops_without_help=1000;
// 					int hops_with_help=1000;
// 					int help_from_who=0;
// 					int dest_pos=0;;
// 					int direct_server_pos;
// 					int start_count=0;
// 					cout<<"ARRAY SERVERS FOR SERVER "<<my_rank<< "-->";
// 					for(int i=0;i<num_of_servers;i++)
// 						cout<<" "<<leaders_server_array[i]<<" ";
// 					for(int i=0;i<num_of_servers;i++){
// 						//cout<<" "<<leaders_server_array[i]<<" ";
// 						if(leaders_server_array[i] == destination){
// 							hops_without_help = num_of_servers-(i+1);
// 							break;
// 						}

// 					}
// 					// for(int i=0;i<num_of_servers;i++){
// 					// 	//cout<<" "<<leaders_server_array[i]<<" ";
// 					// 	if(leaders_server_array[i] == destination){
// 					// 		hops_without_help = num_of_servers-(i+1);
// 					// 		break;
// 					// 	}

// 					// }
// 					cout<<endl;

// 					cout<<"THESE SERVERS ARE DIRECTLY CONNECTED TO ME "<<my_rank<< "-->";
// 					for (int i = 0; i < directly_connected.size(); ++i)
// 					{
// 						cout<<" "<<directly_connected[i]<<" ";
// 					}
// 					cout<<endl;
// 					for(int i=0;i<num_of_servers;i++){
// 						if(destination == leaders_server_array[i]){
// 							dest_pos = i;
// 						}
// 						if(contains(directly_connected,leaders_server_array[i])){
// 							direct_server_pos=i;
// 							help_from_who = leaders_server_array[i];
// 						}
						
// 					}
// 					hops_with_help = (direct_server_pos - dest_pos)+1;
// 					cout<<"I AM "<<my_rank<<" AND I WILL NEED " <<hops_without_help<<" HOPS WITHOUT HELP FOR SERVER "<<destination<<endl;
// 					cout<<endl;
// 					cout<<"I AM "<<my_rank<<" AND I WILL NEED " <<hops_with_help<<" HOPS WITH HELP FOR SERVER "<<destination<<" THROUGH SERVER "<<help_from_who<<endl;
// 					cout<<endl;

// 					if((hops_with_help < 0) || (!hops_with_help)){
// 						cout<<"@@@@@@@@@@ SO FOR SERVER "<<destination<<" I WILL NOT NEED ANY HELP AND I WILL DO "<<hops_without_help<<" HOPS"<<endl;
// 					}else{
// 						if(hops_with_help < hops_without_help)
// 							cout<<"@@@@@@@@@@ SO FOR SERVER "<<destination<<" I WILL NEED HELP AND I WILL DO "<<hops_with_help<<" HOPS WITH HELP FROM "<<help_from_who<<endl;
// 						else
// 							cout<<"@@@@@@@@@@ SO FOR SERVER "<<destination<<" I WILL NOT NEED ANY HELP AND I WILL DO "<<hops_without_help<<" HOPS"<<endl;
// 					}

						
// 					return 0;

// }


					// 						int shortest_path=0;
					// for(int i=0;i<num_of_servers;i++){
					// 	//if(leaders_server_array[i] == my_rank) //return 0;
					// 	if(destination == leaders_server_array[i]){
					// 		shortest_path = 1;
					// 		dest_pos = i;
					// 	}
					// 	if(shortest_path){

					// 		if(contains(directly_connected,leaders_server_array[i])){
					// 			direct_server_pos=i;
					// 			help_from_who = leaders_server_array[i];
					// 			//return leaders_server_array[i];
					// 			// shortest_path = leaders_server_array[i];
					// 			// break;
					// 		}
					// 	}
					// }


// vector<int> choose_random_servers(int rank,int *arr,int size){
// 	vector<int> return_vector;
// 	vector<int> choose_random_servers;
// 	for(int i=1;i<world_size;i++){
// 		if(i != rank)
// 			choose_random_servers.push_back(i);
// 	}

// 	int random;
// 	for (int i = 0; i < (num_of_servers/2) + 1; i++){

// 		random = (rand() % (choose_random_servers.size()));
// 		cout<<"EMPIKAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA "<<i<<" "<<random<<endl;
// 		return_vector.push_back(choose_random_servers[random]);
// 		choose_random_servers.erase(std::remove(choose_random_servers.begin(), choose_random_servers.end(), choose_random_servers[random]), choose_random_servers.end());
// 	}
// 	cout<<"EMPIKAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA "<<return_vector.size()<<endl;
// 	return return_vector;
// }