
****/** first there is little explanation about json's function */
{
    */** this function will print the string of json and return the address
    it will gets the item which is cJSON * as an argument */
    char *cJSON_PrintUnformatted(const cJSON * item )
    */** this function will gets the string for the naming the item which is going to be added to object
    like "sender":"ali" the ali is an item which is add to object by taged as sender   */
    void cJSON_AddItemToObject(cJSON *object, const char *,cJSON * item)
    */** this will add the item to cjson array*/
    void cJSON_AddItemToArray(cJSON* array ,cJSON * item )
    */** this will create an object of json which  we can add items to it it return the
    address of struct of json */
    cJSON * cJSON_CreateObject();
    */** this will create the array of object and return the first element of the first object */
    cJSON *cJSON_CreateArray();
    */** this will make the string and json like "(string)" */
    cJSON *cJSON_CreateString(const char *);
    */** it will parse the json and translate to string  */
    cJSON *cJSON_Parse(const char *);
    */** this will get the object tagged by that name which is given to function as as string argument(char *) and then the
    item tagged by that string in that object will be returned   */
    cJSON *cJSON_GetObjectItem(cJSON *object , const char *);

}

/**     ******************phase 2*****************



* all the function between {} are in connect with client directly
{ */
/** these function are investigating message from client
and are classified in 3 groups
    { */
*/**
*this will analyze the command which are register/login username, password
and cut the each part and send to needed function and return 1 if any of those
command are found and 1 if not return 0
*/
int  m_analays_r_l(const char * buffer);
*/**
* this function will analyze the command which in my classification are
(command just one ) (tocken)
it gets the const char named buffer and then return 1 if command was refresh or log out
or leave and return 0 or not it will get index of memeber and pass it by tocken to neede
function
*/
*
int  m_analays_ref_lout_leave(const char * buffer);
*/**
* this function will analyze the (command) (command) (name), tocken
command in my classification of command function that get the command from client
*/
int m_analays_channel(const char* buffer);
/**
    * } end of classification of message */
*/**
* the member_c will get the index of the client and then search the members
that are in the sane channel then make a json and send it to client
*/
void members_c(int index);
*/**
* the refresh will gets the lines from file then parse it and sent it to the client
just index is argument of refresh */
void refresh(int index);
*/**
* the function register gets name and password as arguments and then create the txt
file in the directory and save the user name and password
*/
void regster(char * tmpname,char* tmppassword);
*/**
* the login will check the client is already registerd and send a tocken and save it
in struct of client to have it and make the id in that struct 1
gets name and password as argument
 */
void login(char *tmpname,char *tmppassword);
*/**
will send the successful type to client */
void successful();
*/**
* function below is for the messages which the client send to be written in channel
it will get the messges and the pass it for set in file function
*/
void recieve_m(const char * buffer);
*/**
* the input_error is for request that are in the wrong way */
void input_error();
/**
* } end of server connecting to client functions */


**/** function that arn't connect directly to server */
*/**
*the create channel gets the name of channel and the index of client and the address
as tmp all as argument then create the directory
*/
void creat_chanell(char * name, int index, char * tmp);
*/**
* the join will set the name of channel to struct of that member
and also print in the file by passing to set in file
*/
void join_chanell(char * name, int index, char*tmp);
*/**
* tocken is now checked and if its valid the index of the sender is returned and if the tocken isnt found it will send an Error
and return -1 */
int tokencheking(char * tocken);
*/**
*leave will undo the position and delete the name of channel in channel
and gets the index of client as argument
*/
void leave(/*char * authtouken,*/int index);
*/**
* log out function gets  the number of the client in struct
and then delete the tocken and name and then delete the client by
replace to the previous one
*/
void logout(/*char * authtouken,*/int index);
*/**
* it will open a file get address of it and the message you want to put and who is sender also
as argument */
void set_in_file(char * address,char * sms,char *sender);
/**
* the struct clients will hold the tocken and name
the id represent that the user is logged in or not 1 for logged in and -1 for logged out
the channel name holds the name of channel and the position holds that its
joined the channel or not
the refresh save the number of message in file of channel that user has seen
*/
struct clients
{
    char * AUTHTOCKEN;
    char *name;
    int id;
    char * chanell_name;
    int position;
    int refresh;
} members[CMAX];

****
****/**          *******phase 1*************** */


/** final phase one */
**/** the functions that are totally connected with the server is just */
*/**
* this function takes the mymessage as the thing which is going to be sent to server and it create the socket for
client and then recieve a messege from srever and then copy it to
global array named tmpbuffer then it is returned and close the clinet socket before
*/
int socket_creating(char *mymessage); :
**/**the function which are designing the menu are
  {:*/
*/**
*the function start is my first page and it explain the commands which are needed to start chatting
it is the function which is going to relate to menu
the command help will show the tips over the screen
*/
void start();
*/**
*the function menu have two option made of login and register
 and with command exit the program will close
 it is not connected to server its just the client function
*/
void my_menu();
*/**
* the login function is somehow in connect with server and it will make the string
and pass it to socket creating it will
*/
void my_login();
*/**
* the function my_client_chat will be the menu of the chat in channel
*/
void my_client_chat();
/** } end of menu designer */
**/** the function down til } are the functions that parse and print the jsons from server
  {*/
*/**
* the function refresh is the function which will give a json from server( not as argument ) and then
pass it for  parsing then will print the messeges the function of json will explain next to them
*/
void myrefresh();
*/**
*this my mem will receive an json from server  not as argument and then parse it to show the members
*/
void mymem();
/** } end of json printers */
*/**
* this function will send the message you want to tell in the group and will create related string and send it to server
then it is connected to server by function socket_creating in it
*/
void mysend();
*/**
*the c_json function is the function that will parse the json from server and investigate that is it Error type or
successful and if the Error occurs then the content of Error is printed
it will also get the tocken which server gives us and then copy it to global array named auth_tocken
*/
int c_json();
**/** these two make the string needed to be sent to the
   {*/
*/**
*this is for register and login that is for command like register (name), (password)
*/
void stringcat(char *userid,char * userpassword);
*/**
*this function will create the command to srever with this format crate channel (name), (tocken) and send (), (tocken)
and first can be create channel , join channel , channel members , send , request is the messege or command remain
*/
void stringcserver(char *first, char request[]);
/** }end of string makers*/
/**
* my program classify command and it will then passes the command needed to function
the function return and it again circle in the loop for other command
*/
int main()
{
    while(option)
    {
    if(option==1)
        start();
    if(option==2)
        my_menu();
    if(option == 3)
        my_login();
    if(option == 4)
        my_client_chat();
    }
}



