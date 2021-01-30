#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 
#include <stddef.h> 
#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <iterator> 
#include <map> 
#include <sstream> 
#include <cassert>
#include <vector>
#include <algorithm>
#include <time.h>
#include <utility>
#include <queue>



#define ACK 0
#define YOU_ARE_SERVER 1
#define YOU_ARE_NOT_A_SERVER 2
#define START_LEADER_ELECTION 3
#define CANDIDATE_ID 4
#define CONNECT 5
#define CONNECTED_ACK 6
#define LEADER_ELECTION_DONE 7
#define CLIENT 8
#define UPLOAD 9
#define SENT_FILE_TO_LEADER 10
#define UPLOAD_FAILED 11
#define UPLOAD_ACK 12
#define UPLOAD_OK 13
#define RETRIEVE 14
#define SENT_RETRIEVE_FILE_TO_LEADER 15
#define RETRIEVE_FAILED 16
#define RETRIEVE_ACK 17
#define RETRIEVE_OK 18
#define UPDATE 19
#define SENT_UPDATE_FILE_TO_LEADER 20
#define UPDATE_FAILED 21
#define VERSION_CHECK 22
#define VERSION_OUTDATED 23
#define UPDATE_ACK 24
#define UPDATE_OK 25

#define IS_UPLOAD 100
#define IS_UPDATE 101
#define IS_RETRIEVE 110

const std::string green("\033[0;32m");
const std::string red("\033[0;31m");
const std::string yellow("\033[0;33m");
const std::string blue("\033[0;30m");
const std::string reset("\033[0m");

/*
         foreground background
black        30         40
red          31         41
green        32         42
yellow       33         43
blue         34         44
magenta      35         45
cyan         36         46
white        37         47 


reset             0  (everything back to normal)
bold/bright       1  (often a brighter shade of the same colour)
underline         4
inverse           7  (swap foreground and background colours)
bold/bright off  21
underline off    24
inverse off      27

*/

using namespace std; 

//#define TAG 0 

struct commands_t {
  string type;
  int first;
  int second;
  int third;
} ;

struct you_are_server_msg{
  int am_i_server;
  int left_neighbour_rank;
  int right_neighbour_rank;
} ;

struct server_data{
  int am_i_server;
  int my_rank;
  int my_left_neighbour;
  int my_right_neighbour;
  int am_i_lucky;
} ;

struct upload_msg{
  int you_are_client;
  int file_id_to_upload;
  int nothing;
} ;

struct client_data{
  int am_i_client;
  int my_rank;
  int file_id_to_upload;
  int file_version;
  int leader_id;
} ;

struct queue_node{
  int client_id_made_request;
  int remaining_servers;
  int type_of_request;
  int help_version;
  int deleted;
} ;


struct local_data_for_file{
  int file_id;
  int version;
};

struct local_data_for_file_client{
  int file_id;
  int version;
};

struct retrieve_msg{
  int file_id;
  int nothing;
  int nothing2;
} ;

struct update_msg{
  int file_id;
  int nothing;
  int nothing2;
} ;


int stop_leader_election = 0;
int loop=1;
int first_stage=0;
int wait_for_ack_first_stage=0;
int flag=0;
int second_stage=1;
int first_stage_count=0;
int third_stage = 0;
string previous_line;
//-----
int start_leader=0;
int num_of_servers,world_size;

string check_for_leader_election;

//map<int, string> commands;
map<int, commands_t> commands; 

you_are_server_msg data_to_send;
upload_msg data_to_send_to_client;
upload_msg send_upload_data_to_client;
retrieve_msg send_retrieve_data_to_client;
update_msg send_update_data_to_client;

MPI_Status status;
MPI_Datatype struct_with_three_integers; 

MPI_Datatype ack_res; 
MPI_Datatype ack_res2; 

vector<int> total_servers; 

vector<int> total_np; 

char* ack;
server_data my_data;
client_data my_data_as_client;

int client_knows_leader;



vector<int> who_are_connected_with_me;

local_data_for_file data_to_hold;
local_data_for_file_client data_to_hold_client;

vector<local_data_for_file> local_data_for_file_server_keeps;
vector<local_data_for_file_client> local_data_for_file_client_keeps;

//queue<queue_node> file_queue;

map<int, queue<queue_node> > hashtable; 
    // MPI_Type_contiguous(3, MPI_BYTE, &ack_res);
    // MPI_Type_commit(&ack_res);

int total_fails;

int test=0;

int event=0;

int total_retrieved = 0;
int total_retrieved_failed = 0;


  
