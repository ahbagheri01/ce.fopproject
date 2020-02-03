/** final phase one */
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "cJSON.c"
#include"cJSON.h"
#include<string.h>
#include <stdbool.h>
#define MAX 80
#define MYMAX 20000
#define PORT 12345
#define SA struct sockaddr
void start();
void my_menu();
void my_login();
void my_client_chat();
void mysend();
void myrefresh();
void mymem();
void stringcat(char *userid,char * userpassword);
void stringcserver(char *first, char request[]);
int c_json();
int socket_creating(char *mymessage);
char auth_tocken[MYMAX];
char servercontent[MYMAX];
char tmpbuffer[MYMAX];
char serverrespond[MYMAX];
int option=1;
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
/**
*the function start is my first page and it explain the commands which are needed to start chatting
it is the function which is going to relate to menu
the command help will show the tips over the screen
*/
void start()
{
    system("color E0"); system("cls");
    printf("%s","welcome to this chat application\n\ntype start to start your own chat \n\ntype help to have more information\n");
    char *massage=malloc(10*sizeof(char));
    fflush(stdin);
    gets(massage);
    /**
    * the message is now analized and passed for needed function to send the server needed string
    */
    if(!strcmp(massage,"start"))
        option = 2;
    else if(!strcmp(massage,"help"))
    {
        printf("%s","1)just type the command which is shown to start doing that command \n2)type back to go to the previous command list if you were not crating some thing\n3)please type start to start the chat\n4)type exit here to end\n");
        fflush(stdin);
        getchar();
        option = 2;
    }
    else if(!strcmp(massage,"exit"))
    {
        system("cls"); option = 0;
    }
    else
    {
        puts("please type correct command");
        option = 1;
    }
    free(massage); return ;
}
/**
*the function menu have two option made of login and register
 and with command exit the program will close
 it is not connected to server its just the client function
*/
void my_menu()
{
    system("color A0"); system("cls");
    memset(auth_tocken,0,MYMAX);
    char *mymassage=calloc(MYMAX,sizeof(char));
    char *user_id=calloc(MYMAX,sizeof(char));
    char *user_password=calloc(MYMAX,sizeof(char));
    printf("%s","Menu:\n\n1)register\n2)login\n");
    char mymassage1[MYMAX]="register ",mymassage2[MYMAX]="login ";
    fflush(stdin);
    gets(mymassage);
    if(!strcmp(mymassage,"exit"))
        option = 0;
    else if(!strcmp(mymassage,"1"))
    {
        system("cls");
        printf("%s","please enter your user name(note that should not start with numbers)\n");
        fflush(stdin);
        gets(user_id);
        printf("%s","please enter your password\n");
        fflush(stdin);
        gets(user_password);
        stringcat(user_id,user_password);
        strcat(mymassage1,user_id);
        if (!socket_creating(mymassage1))
            return ;
        if(c_json())
        {
            printf("%s","registered successfully\n");
            option = 2;
        }
        else
        {
        puts("failed to register the user name is reserved before \nback to menu\n");
        fflush(stdin);
        gets(mymassage);
        if(!strcmp(mymassage,"back"))
            option=2;
        }
    }
    else if(!strcmp(mymassage,"2"))
    {
        system("cls");
        printf("%s","please enter your user name\n");
        fflush(stdin);
        gets(user_id);
        printf("%s","please enter your user password\n");
        fflush(stdin);
        gets(user_password);
        stringcat(user_id,user_password);
        strcat(mymassage2,user_id);
        if(!socket_creating(mymassage2))
            return;
        if(!c_json())
        {
            fflush(stdin);
            gets(mymassage);
            if(!strcmp(mymassage,"back"))
                option = 2;
        }
        else
        {
            printf("%s","logged in Successful\n");
            strcat(auth_tocken,"\n");
            option = 3;
        }
    }
    else
    {
        puts("please type correct command");
        option = 2;
    }
    free(mymassage); free(user_id); free(user_password); return;
}
/**
* the login function is somehow in connect with server and it will make the string
and pass it to socket creating it will
*/
void my_login()
{
    system("color B0");
    system("cls");
    printf("%s","1)create a channel\n2)join a channel\n3)logout\n");
    char *mymassage=calloc(MYMAX,sizeof(char));
    char *channel_name=calloc(MYMAX,sizeof(char));
    fflush(stdin);
    gets(mymassage);
    if(!strcmp(mymassage,"1"))
    {
        /**
        * this will create the string and pass it to socket which will send it to function
        */
        system("cls");
        printf("%s","please enter your channel name\n");
        fflush(stdin);
        gets(channel_name);
        char request[MYMAX]="create channel ";
        stringcserver(channel_name,request);
        socket_creating(channel_name);
        /**
        * the function c_json will parse the string came from server and then investigate that it is an error type
        or not if its not it will be one and then the option will be 4 which means that it should go to channel
        menu
        */
        if(c_json())
        {
            system("cls");
            option = 4;
        }
        else
        {
            puts("failed\ntry another name\n");
            fflush(stdin);
            getchar();
            option = 3;
        }
    }
    else if(!strcmp(mymassage,"2"))
    {
        system("cls");
        printf("%s","please enter your channel name\n");
        fflush(stdin);
        gets(channel_name);
        char request[MYMAX]="join channel ";
        stringcserver(channel_name,request);
        socket_creating(channel_name);
        /**
        c_json is explained up and will be totally explain down
         */
        if(c_json())
        {
            puts("joined\n");
            system("cls");
            option = 4;
        }
        else
        {
            puts("failed\ntry another name\n");
            fflush(stdin);
            getchar();
            option = 3;
        }
    }
    else if(!strcmp(mymassage,"3"))
    {
        system("cls");
        char lgot[MYMAX]="logout ";
        strcat(lgot,auth_tocken);
        socket_creating(lgot);
        if(c_json())
        {
            puts("logged out\n");
            system("cls");
            option = 2;
        }
        else
        {
            puts("failed\n");
            fflush(stdin);
            getchar();
            option = 3;
        }
    }
    else
    {
        puts("please type correct command");
        option = 3;
    }
    free(mymassage); free(channel_name); return;
}
/**
* the function my_client_chat will be the menu of the chat in channel
*/
void my_client_chat()
{
    char *mymassage=calloc(MYMAX,sizeof(char));
    char *mychat=calloc(MYMAX,sizeof(char));
    memset(mymassage,0,MYMAX);memset(mychat,0,MYMAX);
    system("color 60");
    printf("%s","1)Send a message\n2)Refresh\n3)Channel Members\n4)Leave a channel\n");
    fflush(stdin);
    gets(mymassage);
    if(!strcmp(mymassage,"1"))
        mysend();
    else if(!strcmp(mymassage,"2"))
        myrefresh();
    else if(!strcmp(mymassage,"3"))
        mymem();
    else if(!strcmp(mymassage,"4"))
    {
        /** there is no function for leave and it will be in few code down there
        */
        char mychat[MYMAX]="leave ";
        strcat(mychat,auth_tocken);
        socket_creating(mychat);
        option = 3;
        system("cls");
    }
    else
    {
        puts("type correct command\n");
        option = 4;
    }
    free(mymassage);
    free(mychat);
    return;
}
/**
* this function will send the message you want to tell in the group and will create related string and send it to server
then it is connected to server by function socket_creating in it
*/
void mysend()
{
    system("cls");
    char *memassage=malloc(MYMAX*sizeof(char));
    memset(memassage,0,MYMAX);
    char mychat[MYMAX]="send ";
    puts("write your message\n");
    gets(memassage);
    stringcserver(memassage,mychat);
    socket_creating(memassage);
    free(memassage);
    system("cls");
    return;
}
/**
* the function refresh is the function which will give a json from server( not as argument ) and then
pass it for  parsing then will print the messeges the function of json will explain next to them
*/
void myrefresh()
{
    system("cls");
    char mychat[MYMAX]="refresh ";
    strcat(mychat,auth_tocken);
    socket_creating(mychat);
    cJSON* items=NULL;
    /**
    * parse the json and return is cJSON * which include the senders and content
    */
    cJSON* rpage_json = cJSON_Parse(tmpbuffer);
    cJSON* mycontent = cJSON_GetObjectItemCaseSensitive(rpage_json, "content");/** will get the object of content */
    cJSON_ArrayForEach(items,mycontent)
    {
        /** will print the json by it functon and the valuestring which is in it*/
        printf("%s : %s\n",cJSON_GetObjectItemCaseSensitive(items,"sender")->valuestring,cJSON_GetObjectItemCaseSensitive(items,"content")->valuestring);
    }
    /**
    this will free buffer of keyboard */
    fflush(stdin);
    getchar();
    system("cls");
    return;
}
/**
*this my mem will receive an json from server  not as argument and then parse it to show the members
*/
void mymem()
{
    system("cls");
    char mychat[MYMAX]="channel members ";
    strcat(mychat,auth_tocken);
    socket_creating(mychat);
    /** this will parse the cjson */
    cJSON *monitor_json = cJSON_Parse(tmpbuffer);
    cJSON *contents = cJSON_GetObjectItemCaseSensitive(monitor_json, "content");
    cJSON *content = NULL;
    cJSON_ArrayForEach(content, contents)
    {
        printf("%s\n", content->valuestring);
    }
    fflush(stdin);
    getchar();
    system("cls");
    return;
}
/**
*this is for register and login that is for command like register (name), (password)
*/
void stringcat(char *userid,char * userpassword)
{
    strcat(userid,", ");
    strcat(userid,userpassword);
    strcat(userid,"\n");
    return;
}
/**
*this function will create the command to srever with this format crate channel (name), (tocken) and send (), (tocken)
and first can be create channel , join channel , channel members , send , request is the messege or command remain
*/
void stringcserver(char *first, char request[])
{
    strcat(request,first);
    strcat(request,", ");
    strcat(request,auth_tocken);
    strcpy(first,request);
    strcat(first,"\n");
    return;
}
/**
*the c_json function is the function that will parse the json from server and investigate that is it Error type or
successful and if the Error occurs then the content of Error is printed
it will also get the tocken which server gives us and then copy it to global array named auth_tocken
*/
int c_json()
{
    cJSON *mytype =NULL;
    cJSON *mycontent =NULL;
    cJSON *monitor_json = cJSON_Parse(tmpbuffer);
    mycontent = cJSON_GetObjectItemCaseSensitive(monitor_json, "content");
    mytype = cJSON_GetObjectItemCaseSensitive(monitor_json, "type");
    memset(serverrespond,0,sizeof(serverrespond));
    strcpy(servercontent,mycontent->valuestring);
    strcpy(serverrespond,mytype->valuestring);
    if(!strcmp(serverrespond,"AuthToken"))
        strcpy(auth_tocken,mycontent->valuestring);
    if(!strcmp(serverrespond,"Successful"))
        return 1;
    if(!strcmp(serverrespond,"AuthToken"))
        return 1;
    if(!strcmp(serverrespond,"Error"))
        printf("%s\n",servercontent);
    return 0;
}
/**
* this function takes the mymessage as the thing which is going to be sent to server and it create the socket for
client and then recieve a messege from srever and then copy it to
global array named tmpbuffer then it is returned and close the clinet socket before
*/
int socket_creating(char *mymessage)
{
    char *myerror=calloc(20*sizeof(char),1);
    int client_socket;//, server_socket;
    struct sockaddr_in servaddr;//, cli;
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
        option = 1;

    }
    // Create and verify socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        printf("Socket creation failed...\nplease try again\nif you want to rebuild type rebuild otherwise press enter\n");
        memset(myerror,0,20);
        gets(myerror);
        if(!strcmp(myerror,"rebuild"))
        {
            option = 1;
        }
    }
    else
    // Assign IP and port
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    // Connect the client socket to server socket
    if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("it seems there is a problem we are trying to solve it \nConnection to the server failed... \nplease try again\nif you want to reconnect type reconnect otherwise type back\n");

        gets(myerror);
        if(!strcmp(myerror,"reconnect"))
        {
            option = 2;free(myerror);
            return 0;
        }
        else
            exit(0);
    }
    send(client_socket,mymessage,MYMAX,0);
    char buffer[MYMAX];
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket,buffer,MYMAX,0);
    memset(tmpbuffer, 0,MYMAX);
    for(int i = 0; i < MYMAX; i++)
        tmpbuffer[i] = buffer[i];
    strcat(tmpbuffer,"\0");
    // Close the socket
    closesocket(client_socket);
    free(myerror);
    return 1 ;
}


