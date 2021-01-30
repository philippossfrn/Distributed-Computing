#include "functions.h"
int search_in_hashtable(map<int, queue<queue_node> > hashtable,int file){
  if ( hashtable.find(file) == hashtable.end() ) {
    return 0;
  } else {
    return 1;
  }
}

queue_node make_new_node_upload(int who_made_request,int num_of_servers){
  queue_node new_request;
  new_request.client_id_made_request = who_made_request;
  new_request.remaining_servers = ((num_of_servers-1)/2) + 1;
  new_request.type_of_request = IS_UPLOAD;
  new_request.help_version = 1;

  return new_request;

}

queue_node make_new_node_retrieve(int who_made_request,int num_of_servers){
  queue_node new_request;
  new_request.client_id_made_request = who_made_request;
  new_request.remaining_servers = ((num_of_servers-1)/2) + 1;
  new_request.type_of_request = IS_RETRIEVE;
  new_request.help_version = 1;
  new_request.deleted = 0;

  return new_request;

}

queue_node make_new_node_update(int who_made_request,int num_of_servers,int version){
  queue_node new_request;
  new_request.client_id_made_request = who_made_request;
  new_request.remaining_servers = ((num_of_servers-1)/2) + 1;
  new_request.type_of_request = IS_UPDATE;
  new_request.help_version = version;
  new_request.deleted = 0;

  return new_request;

}



void print_queue(queue<queue_node> q)
{
  while (!q.empty())
  {
    if((q.front()).type_of_request == IS_UPLOAD)
      std::cout << (q.front()).client_id_made_request << " "<< (q.front()).remaining_servers << " "<< "IS_UPLOAD" << '\t';
    if((q.front()).type_of_request == IS_RETRIEVE)
      std::cout << (q.front()).client_id_made_request << " "<< (q.front()).remaining_servers << " "<< "IS_RETRIEVE" << '\t';
    if((q.front()).type_of_request == IS_UPDATE)
      std::cout << (q.front()).client_id_made_request << " "<< (q.front()).remaining_servers << " "<< "IS_UPDATE" << '\t';
    q.pop();
  }
  std::cout << std::endl;
}

void print_hashtable(map<int, queue<queue_node> > hashtable){
  map<int, queue<queue_node> >::iterator itr; 

  cout<<"HASHTABLE"<<endl;
  for (itr = hashtable.begin(); itr != hashtable.end(); ++itr) { 
    cout << '\t' <<"QUEUE FOR FILE "<< itr->first << '\t'; 
    print_queue(itr->second);
  }

}

int dicrease_remaining_servers(map<int, queue<queue_node> > hashtable,int file_id){
  queue<queue_node> file_queue = hashtable.find(file_id)->second;
  print_queue(file_queue);
  // int temp = (file_queue.front()).remaining_servers;
  // temp--;
  
  (file_queue.front()).remaining_servers--;
  cout<<"?????????????????????????????????????????? "<<file_id<<"-----"<<(file_queue.front()).remaining_servers<<endl;
  
  if((file_queue.front()).remaining_servers == 0) return file_queue.front().client_id_made_request;
  return 0;
}

void pop_front_from_queue(map<int, queue<queue_node> > hashtable,int file_id){
  // queue<queue_node> file_queue = 
hashtable.find(file_id)->second.pop();
   //file_queue.pop();
  // hashtable.find(file_id)->second.pop_front();
 // hashtable[file_id].pop();
//hashtable.find(file_id)->second.front().deleted = 1;
}


int check_hashtable(map<int, queue<queue_node> > hashtable,int file_id){
  queue<queue_node> q = hashtable.find(file_id)->second;
  //print_queue(hashtable.find(file_id)->second);
  if(!q.empty() && ((q.front()).type_of_request == IS_RETRIEVE)){
    //cout<<yellow<<"I MUST ECEXUTE RETRIEVE FOR FILE "<<file_id<<" MADE BY "<<(q.front()).client_id_made_request<<reset<<endl;
    //print_hashtable(hashtable);
    return 1;
  }
  return 0;

  //   while (!q.empty())
  // {
  //   if((q.front()).type_of_request == IS_UPLOAD)
  //     std::cout << (q.front()).client_id_made_request << " "<< (q.front()).remaining_servers << " "<< "IS_UPLOAD" << '\t';
  //   if((q.front()).type_of_request == IS_RETRIEVE)
  //     std::cout << (q.front()).client_id_made_request << " "<< (q.front()).remaining_servers << " "<< "IS_RETRIEVE" << '\t';
  //   q.pop();
  // }
  // std::cout << std::endl;
}

void execute_retrieve(map<int, queue<queue_node> > hashtable,int file_id,int rank,int *servers_array_used_for_leader_election,vector<int> who_are_connected_with_me,int my_left_neighbour){
    vector<int> random_servers;
    random_servers = choose_random_servers(rank,servers_array_used_for_leader_election,num_of_servers);

    int shortest_path;
    for(int i=0;i<random_servers.size();i++){
                //cout<<yellow<<random_servers[i]<<reset<<endl;
      //cout<<" "<<random_servers[i]<<" ";
      data_to_send.am_i_server = file_id;
      data_to_send.left_neighbour_rank = random_servers[i]; //destination server
    if(std::count(who_are_connected_with_me.begin(), who_are_connected_with_me.end(), random_servers[i])){
      //cout<<"@@@@@@@@@@ I AM "<<rank<<" and i want to send RETRIEVE msg to "<<random_servers[i]<<" and it's directly connected with me for file"<<file_id<<endl;
      //send directly to server here
      //data_to_send.am_i_server = file_to_upload;
      //data_to_send.left_neighbour_rank = random_servers[i];
      MPI_Send(&data_to_send, 1, struct_with_three_integers,random_servers[i], RETRIEVE, MPI_COMM_WORLD);
    }else{
          if(random_servers[i] == my_left_neighbour){
        //cout<<"@@@@@@@@@@ I AM "<<rank<<" and i want to send RETRIEVE msg to "<<random_servers[i]<<" and i will NOT need help from so i will forward to "<<my_data.my_left_neighbour<<" file "<<file_id<<endl;
        // data_to_send.am_i_server = file_to_upload;
        // data_to_send.left_neighbour_rank = random_servers[i];
        MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, RETRIEVE, MPI_COMM_WORLD);
          }else{

        // find_shortest_path will return rank of leader to tell him to use the ring or the id of server to use
        shortest_path=find_shortest_path(rank,random_servers[i],num_of_servers,servers_array_used_for_leader_election,who_are_connected_with_me);
        if(shortest_path == rank){
          //cout<<"@@@@@@@@@@ I AM "<<rank<<" and i want to send RETRIEVE msg to "<<random_servers[i]<<" for RETRIEVE and i will NOT need help from so i will forward to "<<my_data.my_left_neighbour<<" for file "<<file_id<<endl;
          // data_to_send.am_i_server = file_to_upload;
          // data_to_send.left_neighbour_rank = random_servers[i];
          MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, RETRIEVE, MPI_COMM_WORLD);
        }else{
          //cout<<"@@@@@@@@@@ I AM "<<rank<<" and i want to send RETRIEVE msg to "<<random_servers[i]<<" for RETRIEVE and i will need help from "<<shortest_path<<" for file "<<file_id<<endl;
          // data_to_send.am_i_server = file_to_upload;
          // data_to_send.left_neighbour_rank = random_servers[i];
          MPI_Send(&data_to_send, 1, struct_with_three_integers,shortest_path, RETRIEVE, MPI_COMM_WORLD);
        }

      }
          
        }

    }

    //  find_shortest_path(rank,random_servers[i],num_of_servers,servers_array_used_for_leader_election,who_are_connected_with_me);
    //  data_to_send.am_i_server = file_to_upload;
    //  MPI_Send(&data_to_send, 1, struct_with_three_integers,status.MPI_SOURCE, UPLOAD_FAILED, MPI_COMM_WORLD);
    

}

int check_hashtable_for_update(map<int, queue<queue_node> > hashtable,int file_id){
  queue<queue_node> q = hashtable.find(file_id)->second;
  //print_queue(hashtable.find(file_id)->second);
  if(!q.empty() && ((q.front()).type_of_request == IS_UPDATE)){
    //cout<<yellow<<"I MUST ECEXUTE RETRIEVE FOR FILE "<<file_id<<" MADE BY "<<(q.front()).client_id_made_request<<reset<<endl;
    //print_hashtable(hashtable);
    return 1;
  }
  return 0;
}

int return_who_made_request(map<int, queue<queue_node> > hashtable,int file_id){
  queue<queue_node> q = hashtable.find(file_id)->second;
  //print_queue(hashtable.find(file_id)->second);
  if(!q.empty() && ((q.front()).type_of_request == IS_UPDATE)){
    //cout<<yellow<<"I MUST ECEXUTE RETRIEVE FOR FILE "<<file_id<<" MADE BY "<<(q.front()).client_id_made_request<<reset<<endl;
    //print_hashtable(hashtable);
    return (q.front()).client_id_made_request;
  }
  return 0;
}

void execute_update_check(int file_id,int client_version_of_file,int flag_bit,int my_left_neighbour){
    data_to_send.am_i_server = file_id;
    data_to_send.right_neighbour_rank = client_version_of_file;
    data_to_send.left_neighbour_rank = flag_bit;
    MPI_Send(&data_to_send, 1, struct_with_three_integers,my_left_neighbour, VERSION_CHECK, MPI_COMM_WORLD);
}

void execute_legit_update(map<int, queue<queue_node> > hashtable,int file_id,int rank,int *servers_array_used_for_leader_election,vector<int> who_are_connected_with_me,int my_left_neighbour,int version_client_has,int num_of_servers){
    vector<int> random_servers;
    random_servers = choose_random_servers(rank,servers_array_used_for_leader_election,num_of_servers);

    int shortest_path;
    for(int i=0;i<random_servers.size();i++){
      cout<<green<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<random_servers[i]<<reset<<endl;
        data_to_send.am_i_server = file_id;
        data_to_send.left_neighbour_rank = random_servers[i]; //destination server
        data_to_send.right_neighbour_rank = version_client_has; //version client has

      if(std::count(who_are_connected_with_me.begin(), who_are_connected_with_me.end(), random_servers[i])){
        //cout<<"@@@@@@@@@@ I AM "<<rank<<" and i want to send RETRIEVE msg to "<<random_servers[i]<<" and it's directly connected with me for file"<<file_id<<endl;
        //send directly to server here
        //data_to_send.am_i_server = file_to_upload;
        //data_to_send.left_neighbour_rank = random_servers[i];
        MPI_Send(&data_to_send, 1, struct_with_three_integers,random_servers[i], UPDATE, MPI_COMM_WORLD);
      }else{
            if(random_servers[i] == my_left_neighbour){
          //cout<<"@@@@@@@@@@ I AM "<<rank<<" and i want to send RETRIEVE msg to "<<random_servers[i]<<" and i will NOT need help from so i will forward to "<<my_data.my_left_neighbour<<" file "<<file_id<<endl;
          // data_to_send.am_i_server = file_to_upload;
          // data_to_send.left_neighbour_rank = random_servers[i];
          MPI_Send(&data_to_send, 1, struct_with_three_integers,my_left_neighbour, UPDATE, MPI_COMM_WORLD);
            }else{

          // find_shortest_path will return rank of leader to tell him to use the ring or the id of server to use
          shortest_path=find_shortest_path(rank,random_servers[i],num_of_servers,servers_array_used_for_leader_election,who_are_connected_with_me);
          if(shortest_path == rank){
            //cout<<"@@@@@@@@@@ I AM "<<rank<<" and i want to send RETRIEVE msg to "<<random_servers[i]<<" for RETRIEVE and i will NOT need help from so i will forward to "<<my_data.my_left_neighbour<<" for file "<<file_id<<endl;
            // data_to_send.am_i_server = file_to_upload;
            // data_to_send.left_neighbour_rank = random_servers[i];
            MPI_Send(&data_to_send, 1, struct_with_three_integers,my_left_neighbour, UPDATE, MPI_COMM_WORLD);
          }else{
            //cout<<"@@@@@@@@@@ I AM "<<rank<<" and i want to send RETRIEVE msg to "<<random_servers[i]<<" for RETRIEVE and i will need help from "<<shortest_path<<" for file "<<file_id<<endl;
            // data_to_send.am_i_server = file_to_upload;
            // data_to_send.left_neighbour_rank = random_servers[i];
            MPI_Send(&data_to_send, 1, struct_with_three_integers,shortest_path, UPDATE, MPI_COMM_WORLD);
          }

        } 
      }
    }
}

// void execute_retrieve(map<int, queue<queue_node> > hashtable,int file_id,int rank,int *servers_array_used_for_leader_election,vector<int> who_are_connected_with_me){
//             vector<int> random_servers;
//           random_servers = choose_random_servers(rank,servers_array_used_for_leader_election,num_of_servers);

//           int shortest_path;
//                     for(int i=0;i<random_servers.size();i++){
//                       //cout<<yellow<<random_servers[i]<<reset<<endl;
//             //cout<<" "<<random_servers[i]<<" ";
//             // data_to_send.am_i_server = file_to_upload;
//             // data_to_send.left_neighbour_rank = random_servers[i];

//               if(contains(who_are_connected_with_me,random_servers[i])){
//                 //cout<<"@@@@@@@@@@ I AM "<<rank<<" and i want to send msg to "<<random_servers[i]<<" for RETRIEVE and it's directly connected with me"<<endl;
//                 //send directly to server here
//                 //data_to_send.am_i_server = file_to_upload;
//                 //data_to_send.left_neighbour_rank = random_servers[i];
//                 //MPI_Send(&data_to_send, 1, struct_with_three_integers,random_servers[i], UPLOAD, MPI_COMM_WORLD);
//               }else{

//                 // find_shortest_path will return rank of leader to tell him to use the ring or the id of server to use
//                 shortest_path=find_shortest_path(rank,random_servers[i],num_of_servers,servers_array_used_for_leader_election,who_are_connected_with_me);
//                 if(shortest_path == rank){
//                   //cout<<"@@@@@@@@@@ I AM "<<rank<<" and i want to send msg to "<<random_servers[i]<<" for RETRIEVE and i will NOT need help from so i will forward to "<<my_data.my_left_neighbour<<endl;
//                   // data_to_send.am_i_server = file_to_upload;
//                   // data_to_send.left_neighbour_rank = random_servers[i];
//                   //MPI_Send(&data_to_send, 1, struct_with_three_integers,my_data.my_left_neighbour, UPLOAD, MPI_COMM_WORLD);
//                 }else{
//                   //cout<<"@@@@@@@@@@ I AM "<<rank<<" and i want to send msg to "<<random_servers[i]<<" for RETRIEVE and i will need help from "<<shortest_path<<endl;
//                   // data_to_send.am_i_server = file_to_upload;
//                   // data_to_send.left_neighbour_rank = random_servers[i];
//                   //MPI_Send(&data_to_send, 1, struct_with_three_integers,shortest_path, UPLOAD, MPI_COMM_WORLD);
//                 }
                
//               }

//           //  find_shortest_path(rank,random_servers[i],num_of_servers,servers_array_used_for_leader_election,who_are_connected_with_me);
//           //  data_to_send.am_i_server = file_to_upload;
//           //  MPI_Send(&data_to_send, 1, struct_with_three_integers,status.MPI_SOURCE, UPLOAD_FAILED, MPI_COMM_WORLD);
//           }

// }





















// void print_queue(queue<queue_node> q)
// {
//   while (!q.empty())
//   {
//     if((q.front()).type_of_request == IS_UPLOAD)
//       std::cout << (q.front()).client_id_made_request << " "<< (q.front()).remaining_servers << " "<< "IS_UPLOAD" << '\t';
//     if((q.front()).type_of_request == IS_RETRIEVE)
//       std::cout << (q.front()).client_id_made_request << " "<< (q.front()).remaining_servers << " "<< "IS_RETRIEVE" << '\t';
//     q.pop();
//   }
//   std::cout << std::endl;
// }

// class Hash 
// { 
//     int BUCKET;    // No. of buckets 
  
//     // Pointer to an array containing buckets 
//     list<int> *table; 
// public: 
//     Hash(int V);  // Constructor 
  
//     // inserts a key into hash table 
//     void insertItem(int x); 
  
//     // deletes a key from hash table 
//     void deleteItem(int key); 
  
//     // hash function to map values to key 
//     int hashFunction(int x) { 
//         return (x % BUCKET); 
//     } 
  
//     void displayHash(); 

//     bool search_Hash(int key);
// }; 
  
// Hash::Hash(int b) 
// { 
//     this->BUCKET = b; 
//     table = new list<int>[BUCKET]; 
// } 
  
// void Hash::insertItem(int key) 
// { 
//     int index = hashFunction(key); 
//     table[index].push_back(key);  
// } 
  
// void Hash::deleteItem(int key) 
// { 
//   // get the hash index of key 
//   int index = hashFunction(key); 
  
//   // find the key in (inex)th list 
//   list <int> :: iterator i; 
//   for (i = table[index].begin(); 
//            i != table[index].end(); i++) { 
//     if (*i == key) 
//       break; 
//   } 
  
//   // if key is found in hash table, remove it 
//   if (i != table[index].end()) 
//     table[index].erase(i); 
// } 
  
// // function to display hash table 
// void Hash::displayHash() { 
//   for (int i = 0; i < BUCKET; i++) { 
//     cout <<"BUCKET"<<"["<< i<<"]"; 
//     // for (auto int& x : table[i]) 
//     //   cout << " --> " << x; 
//     for (list<int>::iterator itr =  table[i].begin(); itr !=  table[i].end(); ++itr)
//       cout << " --> " << *itr ;
//     cout << endl; 
//   } 
// } 

// bool Hash::search_Hash(int key) { 
//   for (int i = 0; i < BUCKET; i++) { 
    
//     // for (auto int& x : table[i]) 
//     //   cout << " --> " << x; 
//     for (list<int>::iterator itr =  table[i].begin(); itr !=  table[i].end(); ++itr)
//       if(key == (*itr))
//         return true;
    
//   } 
//   return false;
// } 

// /***********************************************
// ------------> How to use this                  *
// //Hash h(num_of_servers);                      *
// // h.search_Hash(file_to_upload)               *
// //h.insertItem(file_to_upload);                *
// //h.deleteItem(28);                            *
// // h.displayHash();                            *
//                                                *
// /***********************************************




 // for (list<int>::iterator itr = lst.begin(); itr != lst.end(); ++itr)
 //    cout << " --> " << *itr << endl;

// void main2(){
//     // array that contains keys to be mapped 
//   int a[] = {15, 11, 27, 8, 12}; 
//   int n = sizeof(a)/sizeof(a[0]); 
  
//   // insert the keys into the hash table 
//     // 7 is count of buckets in 
//                // hash table 

//   for (int i = 0; i < n; i++)  
//     h.insertItem(a[i]);   
  
//   // delete 12 from hash table 
//   h.deleteItem(12); 
  
//   // display the Hash table 
//   h.displayHash(); 
// }


// class Hashtable{
//   private:
//     static const int buckets = 10;
//     list<pair<int key,int value>> table[10];

//   public:
//       bool isempty();
//       int hashfunction(int key);
//       void insert_item(int key,int value);
//       void remove_item(int key);
//       int search(int key);
//       void print_table();
// }; 

// bool Hashtable::isempty(){
//   int sum = 0;
//   for(int i=0;i<buckets;i++){
//     sum+=table[i].size();
//   }

//   if(!sum){
//     return true;
//   }

//   return false;
// }

// int Hashtable::hashfunction(int key){
//   return key % buckets;
// }

// void Hashtable::insert_item(int key,int value){
//   int hash = hashfunction(key);
//   auto& cell = table[hash];
//   auto itr = begin(cell);
//   bool exists = false;
//   for(;itr != end(cell);itr++){
//     if(itr->first == key){
//       exists = true;
//       itr->second = value;
//       cout<<"KEY EXISTS VALUE CHANGED"<<endl;
//     }
//   }

//   if(!exists)
//     cell.emplace_back(key,value);
// }

// void Hashtable::remove_item(int key){
//   int hash = hashfunction(key);
//   auto& cell = table[hash];
//   auto itr = begin(cell);
//   bool exists = false;
//   for(;itr != end(cell);itr++){
//     if(itr->first == key){
//       exists = true;
//       itr = cell.erase(itr);
//       cout<<"KEY REMOVED"<<endl;
//     }
//   }

//   if(!exists)
//     cout<<"KEY NOT FOUND"<<endl;
// }
