/** final phase 2 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>
#define MAX 1000
#define BIGMAX 10000
#define MAX1 2000
#define CMAX 30
#define PORT 12345
#define SA struct sockaddr
#include "cJSON.h"
#include "cJSON.c"
/**
* all the function between {} are in connect with client directly
{ */
/** these function are investigating message from client
and are classified in 3 groups
    { */
int  m_analays_r_l(const char * buffer);
int  m_analays_ref_lout_leave(const char * buffer);
int m_analays_channel(const char* buffer);
/**
    * } end of classification of message */
void members_c(int index);
void refresh(int index);
void regster(char * tmpname,char* tmppassword);
void login(char *tmpname,char *tmppassword);
void successful();
void recieve_m(const char * buffer);
void input_error();
/**
* } end of server connecting to client functions */
void creat_chanell(char * name, int index, char * tmp);
void join_chanell(char * name, int index, char*tmp);
int tokencheking(char * tocken);
void leave(/*char * authtouken,*/int index);
void logout(/*char * authtouken,*/int index);
void set_in_file(char * address,char * sms,char *sender);
int number_of_members = 0;
int server_socket;
int client_socket;
struct sockaddr_in server, client;
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
int main()
{
    /**
    * this for initialize the items in struct  */
    for(int i = 0 ; i < CMAX ; i++)
    {
        members[i].name = malloc(MAX);
        members[i].chanell_name= malloc(MAX);
        members[i].AUTHTOCKEN = malloc(31);
        members[i].position = -1;
        members[i].id = -1;
        members[i].refresh = 0;
        memset(members[i].chanell_name,0,MAX);
        memset(members[i].AUTHTOCKEN,0,31);
        memset(members[i].name,0,MAX);
    }
    /**
    * the buffer is what is received from client */
    char *buffer = malloc(MAX);
    /**
    * modes are for the if are in while down to underestand that what command was
    */
    int mode1,mode2,mode3;
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    // Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 0 ;
    }

    // Create and verify socket
    server_socket = socket(AF_INET, SOCK_STREAM, 6);
    if (server_socket == INVALID_SOCKET)
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
    else
        printf("Socket successfully created..\n");

    // Assign IP and port
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

                // Bind newly created socket to given IP and verify
    if (((bind(server_socket, (SA *)&server, sizeof(server))) != 0))
    {
        printf("Socket binding failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully bound..\n");
    /**
    * this while kept the listening of server for each command
    */
    while(true)
    {
    // Now server is ready to listen and verify
    if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
        int len = sizeof(client);
        client_socket = accept(server_socket, (SA *)&client, &len);
        if (client_socket < 0)
        {
        printf("Server accceptance failed...\n");
        exit(0);
        }
        else
        printf("Server acccepted the client..\n");
        /**
        * the buffer is initialized with 0 and the recv gets the command from client
        */
        memset(buffer, 0, MAX);
        recv(client_socket, buffer, MAX, 0);
        //gets(buffer);
       // printf("the buffer is in main :%s\n",buffer);
        if (!strncmp(buffer, "send",4))
            recieve_m( buffer+5);
        else if(!strncmp(buffer,"channel",7)){
            char *tok = calloc(1,31);
            sscanf(buffer,"%*s %*s %s",tok);
            printf("tok is : %s",tok);
            int index = tokencheking(tok);
            members_c(index);
            free(tok);
            }
        else
            {
            mode1 = m_analays_r_l(buffer);
             if (!mode1)
                mode2 = m_analays_ref_lout_leave(buffer);
             if (!mode2)
                mode3=m_analays_channel(buffer);
            if (!mode3){
                /**
                * if no match command found the input_error function
                 will send and error type and content unknown request pattern */
                input_error();
            }
        }
    }
    free(buffer);
    return 0;
}
/**
* this function will analyze the command which in my classification are
(command just one ) (tocken)
it gets the const char named buffer and then return 1 if command was refresh or log out
or leave and return 0 or not it will get index of memeber and pass it by tocken to neede
function
*/
int m_analays_ref_lout_leave(const char *buffer)
{
    char *command = malloc(MAX);
    char * touken = malloc(MAX);
    memset(touken,0,MAX);
    memset(command,0,MAX);
    int index = -1;
    sscanf(buffer,"%s %s",command,touken);
   // printf("the buffer in leave is %s\nthe command in 2 command : %s\n tocken is in leave :%s\n",buffer,command,touken);
    puts(buffer);
    if((strncmp(buffer,"refresh",7))&&(strncmp(buffer,"leave",5))&&(strncmp(buffer,"logout",6)))
        return 0;
    /**
    * the tockencheking is explained down */
    index = tokencheking(touken);
    if(index<0)
        return 0;
    //printf("\nbuffer in refresh is :%s\n",buffer);
    if (!strncmp(buffer,"refresh",7) || !strncmp(buffer, "leave",5) || !strncmp(buffer, "logout",6))
    {
        if (!strncmp(buffer,"leave", 5))
        {
            leave(/*touken,*/index);
            free(touken);
            free(command);
            return 1;
        }
        if (!strncmp(buffer,"logout",6))
        {
            logout(/* touken,*/index);
            free(touken);
            free(command);
            return 1;
        }
        if (!strncmp(buffer,"refresh",7))
        {
            refresh(index);
            free(touken);
            free(command);
            return 1;
        }
    }
    free(touken);
    free(command);
    return 0;
}
/**
*this will analyze the command which are register/login username, password
and cut the each part and send to needed function and return 1 if any of those
command are found and 1 if not return 0
*/
int m_analays_r_l(const char * buffer)
{
    char temp[MAX],tmpname[MAX],tmppassword[MAX];
    sscanf(buffer,"%s %s %s",temp,tmpname,tmppassword);
    if(!strncmp(temp,"register",8)||!strncmp(temp,"login",5))
    {
        if (!strcmp(tmpname,"") || !strcmp(tmppassword,"") || !(*(tmpname+(strlen(tmpname) - 1)) == ','))
        {
            /**
            * if name or password is empty it wont work for server */
            input_error();
            return 0 ;
        }
        *(tmpname+strlen(tmpname)-1)='\0';
        if(!strncmp(temp,"register",8))
        {
            regster(tmpname,tmppassword);
            return 1;
        }
        if(!strncmp(temp,"login",5))
        {
            login(tmpname,tmppassword);
            return 1;
        }
    }
    return 0;
}
/**
* this function will analyze the (command) (command) (name), tocken
command in my classification of command function that get the command from client
*/
int m_analays_channel(const char* buffer)
{
    char *command= malloc(MAX);
    char *command2 = malloc(MAX);
    char *touken = malloc(31);
    char *name = malloc(MAX);
    memset(touken,0,31);
    memset(name,0,MAX);
    memset(command,0,MAX);
    memset(command2,0,MAX);
    sscanf(buffer,"%s %s %s %s",command,command2,name,touken);
   // printf("buffer is in channel : %s\nfirst  : %s\n seconf : %s\n tocken is : %s\n",buffer,command,command2,touken);
    *(name+strlen(name)-1)='\0';
    int index = tokencheking(touken);
    if(index < 0)
        return 0;
    if(!strncmp(command,"create",6))
    {
        /**
        * here we create directory string that we want to create the channel file in it
        */
        char* tmp = calloc(1,MAX+25);
        strcpy(tmp,"resources\\channels\\");
        strcat(tmp,name);
        strcat(tmp,".txt");
        FILE* r=fopen(tmp,"r");
        /**
        * if checks if the channel is created before or not
        and if it is the json will send that represent you cant create this channel
        */
        if(r)
        {
            /** this is going to create a cJSON object */
            cJSON *root1 = cJSON_CreateObject();
            /** the string here is made and become json part */
            cJSON *str = cJSON_CreateString("this channel have already created");
            /** Error type and it content is added to root and then this json will be send */
            cJSON_AddItemToObject(root1,"type",cJSON_CreateString("Error"));
            cJSON_AddItemToObject(root1,"content",str);
            char* out1 ;
            /** cjson here is become an string and then is send to the client */
            out1 = cJSON_PrintUnformatted(root1);
            printf("%s\n",out1);
            send(client_socket,out1,strlen(out1),0);
            fclose(r);
            closesocket(client_socket);
            free(tmp);
            return 0 ;
        }
        fclose(r);
        creat_chanell(name,index,tmp);
        return 1 ;
    }
    else if(!strncmp(command,"join",4))
    {
        /** explained just above */
        char* tmp = calloc(1,MAX+25);
        strcpy(tmp,"resources\\channels\\");
        strcat(tmp,name);
        strcat(tmp,".txt");
        FILE* r=fopen(tmp,"r");
        /**
        * now if the file doesnt exist it will be an available one to join */
        if(!r)
        {
            cJSON *root1 = cJSON_CreateObject();
            cJSON *str = cJSON_CreateString("this channel isn't available");
            cJSON_AddItemToObject(root1, "type", cJSON_CreateString("Error"));
            cJSON_AddItemToObject(root1, "content",str );
            char *out1 ;
            out1 = cJSON_PrintUnformatted(root1);
            printf("%s\n",out1);
            send(client_socket, out1, strlen(out1), 0);
            fclose(r);
            closesocket(client_socket);
            free(tmp);
            return 0;
        }
        fclose(r);
        join_chanell(name,index,tmp);
        return 1 ;
    }
    return 0;
}
/**
* the function register gets name and password as arguments and then create the txt
file in the directory and save the user name and password
*/
void regster(char * tmpname,char* tmppassword)
{
    char* tmp = calloc(1,MAX+25);
    strcpy(tmp,"resources\\users\\");
    strcat(tmp,tmpname);
    strcat(tmp,".txt");
    FILE* r=fopen(tmp,"r");
    /**
    * if checks if the user name have already registered or no and if so
    the Error type will be send */
    if(r)
    {
        cJSON *root1 = cJSON_CreateObject();
        cJSON *str = cJSON_CreateString("this user have already created");
        cJSON_AddItemToObject(root1,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(root1,"content",str);
        char* out1 ;
        out1 = cJSON_PrintUnformatted(root1);
        send(client_socket,out1,strlen(out1),0);
        printf("%s",out1);
        fclose(r);
        closesocket(client_socket);
        free(tmp);
        return;
    }
    fclose(r);
    successful();
    /** here the json is created to be sent */
    cJSON *root = cJSON_CreateObject();
    cJSON *n_str = cJSON_CreateString(tmpname);
    cJSON *p_str = cJSON_CreateString(tmppassword);
    cJSON_AddItemToObject(root,"username",n_str);
    cJSON_AddItemToObject(root,"password",p_str);
    char* out ;
    out = cJSON_PrintUnformatted(root);
    printf("%s\n",out);
    /** the file is now created here
    and then the user name and password is added */
    FILE *rgister=fopen(tmp,"w");
    fprintf(rgister,"%s",out);
    fclose(rgister);
    free(tmp);
    return;
}
/**
* the login will check the client is already registerd and send a tocken and save it
in struct of client to have it and make the id in that struct 1
gets name and password as argument
 */
void login(char *tmpname,char *tmppassword)
{
    char* tmp = calloc(1,MAX+25);
    strcpy(tmp,"resources\\users\\");
    strcat(tmp,tmpname);
    strcat(tmp,".txt");
    FILE* logn=fopen(tmp,"r");
    /**
    * this will check the that it is registered or not */
    if(!logn)
    {
        cJSON *root1 = cJSON_CreateObject();
        cJSON *e_str = cJSON_CreateString("Error");
        cJSON *m_str = cJSON_CreateString("this user is not registered");
        cJSON_AddItemToObject(root1,"type",e_str);
        cJSON_AddItemToObject(root1,"content",m_str);
        char* out1 ;
        out1 = cJSON_PrintUnformatted(root1);
        printf("%s\n",out1);
        send(client_socket,out1,strlen(out1),0);
        closesocket(client_socket);
        fclose(logn);
        free(tmp);
        return;
    }
    /**
    * here we check if the user is already logged in or not */
    for(int i = 0 ; i < number_of_members; i ++)
    {
        if((strcmp(members[i].name,tmpname)== 0)&& members[i].id == 1)
        {
            cJSON *root = cJSON_CreateObject();
            cJSON *e_str = cJSON_CreateString("Error");
            cJSON *m_str = cJSON_CreateString("this user is already logged in");
            cJSON_AddItemToObject(root,"type",e_str);
            cJSON_AddItemToObject(root,"content",m_str);
            char* out ;
            out = cJSON_PrintUnformatted(root);
            printf("%s\n",out);
            send(client_socket,out,strlen(out),0);
            closesocket(client_socket);
            free(tmp);
            return ;
        }
    }
    fseek(logn, 0, SEEK_SET);
    char tmp1[MAX1];
    int count = 0;
    while (true)
    {
        tmp1[count] = fgetc(logn);
        if (feof(logn))
            break;
        count++;
    }
    /**
    * we have read the file and down we will parse it then get the password item
    and if the password isnt correct the error type will be sent
    */
    cJSON *mycontent =NULL;
    cJSON *monitor_json = cJSON_Parse(tmp1);
    mycontent = cJSON_GetObjectItemCaseSensitive(monitor_json, "password");
    memset(tmp1,0,strlen(tmp1));
    strcpy(tmp1,mycontent->valuestring);
    if(strcmp(tmppassword,tmp1))
    {
        cJSON *root1 = cJSON_CreateObject();
        cJSON *e_str = cJSON_CreateString("Error");
        cJSON *m_str = cJSON_CreateString("wrong password");
        cJSON_AddItemToObject(root1,"type",e_str);
        cJSON_AddItemToObject(root1,"content",m_str);
        char* out1;
        out1 = cJSON_PrintUnformatted(root1);
        printf("%s\n",out1);
        send(client_socket,out1,strlen(out1),0);
        closesocket(client_socket);
        fclose(logn);
        free(tmp);
        return;
    }
    /**
    *the random tocken is down created for sending to client and also save it in the
    struct
    the tockne is 10_alpha 10_int _ 10_alpha
     */
    srand(time(0));
    bool same_check = true;
    char tmptocken[30];
    while(same_check)
    {
        for (int i = 0; i < 30; i++)
        {
            if (i < 10)
            {
                tmptocken[i]=(char)(((123 * rand())%26)+65);
            }
            else if (i < 20)
            {
                tmptocken[i]=(char)(((rand()*rand())%10)+'0');
            }
            else
            {
                tmptocken[i]=(char)(((321 * rand())%26)+97);
            }
        }
        tmptocken[30]='\0';
        /**
        * function down is checking if the tocken is already created for any user
        before or not
        */
        same_check = false;
        /*for(int i = 0; i < number_of_members ; i ++)
        {
            if(!strcmp(tmptocken,members[i].AUTHTOCKEN))
            {
                same_check = true;
                break;
            }
        }*/
    }
    memset(members[number_of_members].name,0,MAX);
    strcpy(members[number_of_members].name,tmpname);
    strcpy(members[number_of_members].AUTHTOCKEN,tmptocken);
    members[number_of_members].id = 1;
    cJSON *root = cJSON_CreateObject();
    cJSON *tocken = cJSON_CreateString(tmptocken);
    cJSON *AUTH = cJSON_CreateString("AuthToken");
    cJSON_AddItemToObject(root,"type",AUTH);
    cJSON_AddItemToObject(root,"content",tocken);
    char *out ;
    out = cJSON_PrintUnformatted(root);
    printf("%s\n",out);
    send(client_socket,out,strlen(out),0);
    closesocket(client_socket);
    number_of_members = number_of_members +1 ;
    free(tmp);
    return;
}
/**
* log out function gets  the number of the client in struct
and then delete the tocken and name and then delete the client by
replace to the previous one
*/
void logout(/*char * authtouken,*/int index)
{
    members[index].id = -1;
    memset(members[index].name,0,strlen(members[index].name));
    memset(members[index].AUTHTOCKEN,0,strlen(members[index].AUTHTOCKEN));
    successful(client_socket);
    for(int j = index ; j < number_of_members -1 ; j++)
    {

        members[j].refresh = members[j+1].refresh;
        members[j].position = members[ j + 1].position;
        members[j].id = members[ j + 1].id;
        memset(members[j].AUTHTOCKEN,0,strlen(members[j].AUTHTOCKEN));
        memset(members[j].name,0,strlen(members[j].name));
        memset(members[j].chanell_name,0,strlen(members[j].chanell_name));
        strcpy(members[j].AUTHTOCKEN,members[ j + 1].AUTHTOCKEN);
        strcpy(members[j].name,members[ j + 1].name);
        strcpy(members[j].chanell_name,members[ j + 1].chanell_name);
    }
    number_of_members = number_of_members -1 ;
    return ;
}
/**
*the create channel gets the name of channel and the index of client and the address
as tmp all as argument then create the directory
*/
void creat_chanell(char * name, int index,char * tmp)
{
    FILE * create = fopen(tmp,"w");
    fclose(create);
    members[index].position = 1;
    memset(members[index].chanell_name,0,strlen(members[index].chanell_name));
    strcpy(members[index].chanell_name,name);
    char * sms = calloc(1,MAX);
    strcat(sms,members[index].name);
    strcat(sms," created and joined");
    set_in_file(tmp,sms,"server");
    successful();
    return ;
}
/**
* the join will set the name of channel to struct of that member
and also print in the file by passing to set in file
*/
void join_chanell(char * name, int index,char *tmp)
{
    successful(client_socket);
    memset(members[index].chanell_name,0,strlen(members[index].chanell_name));
    strcpy(members[index].chanell_name,name);
    members[index].position = 1;
    char sms[MAX]="";
    strcat(sms,members[index].name);
    strcat(sms," joined");
    set_in_file(tmp,sms,"server");
    return ;
}
/**
*leave will undo the position and delete the name of channel in channel
and gets the index of client as argument
*/
void leave(/*char * authtouken,*/int index)
{
    char address[MAX+25] = "resources\\channels\\";
    strcat(address,members[index].chanell_name);
    strcat(address,".txt");
    char sms[MAX]="";
    strcat(sms,members[index].name);
    strcat(sms," leaved");
    set_in_file(address,sms,"server");
    successful();
    memset(members[index].chanell_name,0,strlen(members[index].chanell_name));
    members[index].position = -1;
    return ;
}
/**
* function below is for the messages which the client send to be written in channel
it will get the messges and the pass it for set in file function
*/
void recieve_m(const char * buffer)
{
    char token[30];
    char sms[BIGMAX] = "";
    int n = 0;
    while((*(buffer+n))!=',')
    {
        sms[n]=*(buffer + n);
        n++;
    }
    sms[n]='\0';
    int j = 0;
    n+=2;
    while(j < 30 ) {
        token[j] = *(buffer+n);
        n++;
        j++;
    }
    int index = tokencheking(token);
    if(index < 0)
        return ;
    char address[MAX+25] = "resources\\channels\\";
    strcat(address,members[index].chanell_name);
    strcat(address,".txt");
    set_in_file(address,sms,members[index].name);
    successful();
    return ;
}
/**
* the refresh will gets the lines from file then parse it and sent it to the client
just index is argument of refresh */
void refresh(int index)//
{
    char * tmp = malloc(MAX+25);
    memset(tmp,0,MAX+25);
    strcpy(tmp,"resources\\channels\\");
    strcat(tmp,members[index].chanell_name);
    strcat(tmp,".txt");
    char *file = calloc(1,MAX) ;
    FILE * ref  = fopen(tmp,"r");
    char *ftmp = calloc(1,MAX);
    memset(file,0,MAX);
    /**
    * will go until unseen messages */
    for (int i = 0; i < members[index].refresh;i ++){
        fgets(ftmp,MAX,ref);
        memset(file,0,MAX);
    }
    cJSON *tree = cJSON_CreateArray();
    int num = 0;
    memset(file,0,MAX);
    /**
    * the loop down is for getting lines from the file and then parse it and add it
    to array of json  for sending to client
    */
    for(num = members[index].refresh -1; fgets(file,MAX,ref)!=NULL ; num++){
    cJSON *monitor_json = cJSON_Parse(file);
    cJSON *root = cJSON_CreateObject();
    cJSON *snd = cJSON_GetObjectItem(monitor_json,"sender");
    //cJSON *cont = cJSON_GetObjectItem(monitor_json,"content");//
    /**
    * this will add the item which include the sender of message to root as
    "sender" */
    cJSON_AddItemToObject(root,"sender",snd);
    //cJSON_AddItemToObject(root,"content",cont);//
    puts(cJSON_PrintUnformatted(root));
    cJSON_AddItemToArray(tree,root);
    memset(file,0,MAX);
    }
    fclose(ref);
    members[index].refresh = num -1;
    cJSON * pack = cJSON_CreateObject();
    cJSON_AddItemToObject(pack,"type",cJSON_CreateString("List"));
    cJSON_AddItemToObject(pack,"content",tree);
    char *out ;
    out = cJSON_PrintUnformatted(pack);
    send(client_socket,out,strlen(out),0);
    printf("\nunseen sent are:\n%s\n",out);
    closesocket(client_socket);
    free(tmp);
    return ;
}
/**
* it will open a file get address of it and the message you want to put and who is sender also
as argument */
void set_in_file(char * address,char * sms,char *sender)
{
    char file[BIGMAX];
    FILE * add  = fopen(address,"r");
    fseek(add,0,SEEK_SET);
    int count = 0;
    while (!feof(add))
    {
        file[count] = fgetc(add);
        count++;
    }
    fclose(add);
    //printf("%s\n",file);
    cJSON *monitor_json = cJSON_Parse(file);
    cJSON *root = cJSON_CreateObject();
    cJSON *sender_f, *content;
    sender_f = cJSON_CreateString(sender);
    content = cJSON_CreateString(sms);
    cJSON_AddItemToObject(root, "sender",sender_f);
    cJSON_AddItemToObject(root, "content", content);
    cJSON_AddItemToArray(monitor_json,root);
    char *out ;
    out = cJSON_PrintUnformatted(root);
    add = fopen(address,"a");
    fprintf(add,"%s\n",out);
    printf("%s\n",out);
    fclose(add);
    return ;
}
/**
* the member_c will get the index of the client and then search the members
that are in the sane channel then make a json and send it to client
*/
void members_c(int index)
{
    cJSON *answer = cJSON_CreateObject();
    cJSON *content = cJSON_CreateArray();
    cJSON *lis = cJSON_CreateString("List");
    cJSON_AddItemToObject(answer, "type",lis );
    int i = 0;
    while( i < number_of_members){
        if(!strcmp(members[index].chanell_name,members[i].chanell_name)&& (members[i].position==1))
        {
            cJSON *member = cJSON_CreateString(members[i].name);

            cJSON_AddItemToArray(content,member);
        }
        i++;
    }
    char *out1 ;
    cJSON_AddItemToObject(answer, "content", content);
    //cJSON_AddArrayToObject(answer,content);
    out1 = cJSON_PrintUnformatted(answer);
    printf("%s\n",out1);
    send(client_socket, out1, strlen(out1),0);
    closesocket(client_socket);
    return ;
}
/**
* the input_error is for request that are in the wrong way */
void input_error()
{
    cJSON *root_error = cJSON_CreateObject();
    cJSON_AddItemToObject(root_error,"type",cJSON_CreateString("Error"));
    cJSON_AddItemToObject(root_error,"content",cJSON_CreateString("unknown request pattern"));
    char *error ;
    error = cJSON_PrintUnformatted(root_error);
    send(client_socket,strlen(error),MAX,0);
    closesocket(client_socket);
    return;
}
/**
will send the successful type to client */
void successful()
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root,"type",cJSON_CreateString("Successful"));
    cJSON_AddItemToObject(root,"content",cJSON_CreateString(""));
    char *out ;
    out = cJSON_PrintUnformatted(root);
    printf("%s\n",out);
    send(client_socket,out,strlen(out),0);
    closesocket(client_socket);
    return;
}
/**
* tocken is now checked and if its valid the index of the sender is returned */
int tokencheking(char * tocken)
{
     *(tocken+30)='\0';
    int index = -1;
    for(int i = 0; i < number_of_members ; i++)
    {
        *(members[i].AUTHTOCKEN+30)='\0';
        if(strncmp(members[i].AUTHTOCKEN,tocken,30) == 0)
        {
            index = i;
            return index;
        }
    }
    if(index == -1)
    {
        cJSON *root1 = cJSON_CreateObject();
        cJSON_AddItemToObject(root1,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(root1,"content",cJSON_CreateString("tocken authirizing failed"));
        char* out1;
        out1 = cJSON_PrintUnformatted(root1);
        printf("%s",out1);
        send(client_socket,out1,strlen(out1),0);
        closesocket(server_socket);
        return -1;
    }
    return index;
}
