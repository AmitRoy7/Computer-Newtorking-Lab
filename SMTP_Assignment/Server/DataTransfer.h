#ifndef DATATRANSFER_H_
#define DATATRANSFER_H_

#include "headers.h"
#include "Date_Time.h"
#include "Config.h"
#include "messages.h"


///SEND DATA BY SOCKET
void send_data(int sockfd, const char* data)
{
    if (data != NULL)
    {

        int n = write(sockfd, data, strlen(data));
    }
}



///FIND IF USER EXITS
bool check_user(string st)
{

    FILE* fp;
    char data[60];

    fp = fopen("userinfo.txt", "r");
    while (fgets(data, sizeof(data), fp) > 0)
    {
        if (strncmp(st.c_str(), data, st.length()) == 0) /// valid user
        {
            return true;
        }
    }
    fclose(fp);
    return false;
}

bool check_format(string st)
{
    for(int i=0;st[i];i++)
    {
        if(st[i]=='@')
            return true;
    }
    return false;
}

/// RECEIVE, PRINT & STORE MAIL CONTENTS
void mail_data(int sockfd)
{
    sleep(1);
    char buf[BUF_SIZE];
    memset(buf, 0, sizeof(buf));
    recv(sockfd, buf, sizeof(buf), 0); ///RECEIVEING

    cout << "\n\n\t\tMail Content: \n\n\t\t" << endl;

    ///PRINTING
    int l = strlen(buf);
    bool pflag = false;
    for(int i=0; i<l; i++)
    {
        if(i<(l-1) && (buf[i]==92 && buf[i+1]=='n'))    ///92 = \

        {
            printf("\n\t\t");
            i = i+2;
            pflag = true;
            continue;
        }
        if(pflag)
        {
            printf("\t\t%c",buf[i]);
            pflag = false;
        }
        else
            printf("%c",buf[i]);
    }

    ///STORING MAIL CONTENT

    int tm = time(NULL), i;
    char file[80], tp[20];

    gethostname(hostname,HOST_NAME_MAX);
    data_dir = "/home/" + (string)hostname + "/Server/Mailbox";
    mkdir(data_dir.c_str(),0777);
    string directory = data_dir+"/"+(string)from_user;
    mkdir(directory.c_str(),0777);
    directory += "/";
    directory += from_user;
    directory += ".txt";
    FILE* fp = fopen(directory.c_str(), "a");
    if (fp != NULL)
    {
        fprintf(fp,"\n\n");
        fprintf(fp,"==================================================\n\n");
        for(int i=0; buf[i]; i++)
        {
            if(i<(l-1) && (buf[i]==92 && buf[i+1]=='n'))    ///92 = \

            {
                fprintf(fp,"\n");
                i = i+2;
                continue;
            }
            fprintf(fp,"%c",buf[i]);
        }
        fprintf(fp,"==================================================");
        fclose(fp);

    }
    else
    {
        cout << "\n\n\t\tFile open error!" << endl;
    }
    cout << "\n\n\t\tS: " << reply_code[6];
    send_data(sockfd, reply_code[6]);
}


/// respond to request from the client
void respond(int client_sockfd, char* request)
{
    char output[1024];
    memset(output, 0, sizeof(output));

    ///smtp
    if (strncmp(request, "HELO", 4) == 0)
    {
        if (mail_stat == 1)
        {
            cout << "\n\n\t\tS: " << reply_code[6];
            send_data(client_sockfd, reply_code[6]);            ///send READY
            mail_stat = 2;
        }
        else
        {
            cout << "\n\n\t\tS: " << reply_code[16];
            send_data(client_sockfd, reply_code[16]);           ///send ERROR
        }
    }

    else if (strncmp(request, "MAIL FROM", 9) == 0)
    {
        if (mail_stat == 2)
        {

            bool stringStart = false;
            string temp = "";
            for(int i=0;request[i];i++)
            {
                if(request[i]=='<')
                    stringStart = true;
                else if(request[i]=='>')
                    stringStart = false;
                else if(stringStart)
                    temp += request[i];
            }
            if(temp=="")
            {
                cout << "\n\n\t\tS: " << reply_code[13];
                send_data(client_sockfd, reply_code[13]);           ///send ERROR
                return;
            }
            strcpy(from_user,temp.c_str());
            if(!check_format(from_user))
            {
                cout << "\n\n\t\tS: " << reply_code[14];
                send_data(client_sockfd, reply_code[14]);           ///send ERROR
                return;
            }

            cout << "\n\n\t\tS: " << reply_code[6];
            send_data(client_sockfd, reply_code[6]);                ///send OK
            mail_stat = 3;

        }
        else
        {
            cout << "\n\n\t\tS: " << reply_code[16];
            send_data(client_sockfd, reply_code[16]);           ///send ERROR
        }
    }

    else if (strncmp(request, "RCPT TO", 7) == 0)
    {
        if (mail_stat == 3 )
        {
            bool stringStart = false;
            string temp = "";
            for(int i=0;request[i];i++)
            {
                if(request[i]=='<')
                    stringStart = true;
                else if(request[i]=='>')
                    stringStart = false;
                else if(stringStart)
                    temp += request[i];
            }
            if(temp=="")
            {
                cout << "\n\n\t\tS: " << reply_code[13];
                send_data(client_sockfd, reply_code[13]);           ///send ERROR
                return;
            }
            strcpy(rcpt_user,temp.c_str());
            if(!check_format(rcpt_user))
            {
                cout << "\n\n\t\tS: " << reply_code[14];
                send_data(client_sockfd, reply_code[14]);           ///send ERROR
                return;
            }
            if(!check_user(rcpt_user))
            {
                cout << "\n\n\t\tS: " << reply_code[10];
                send_data(client_sockfd, reply_code[10]);           ///send ERROR
                return;
            }

            cout << "\n\n\t\tS: " << reply_code[6];
            send_data(client_sockfd, reply_code[6]);                    ///send OK
            mail_stat = 4;
        }
        else
        {
            cout << "\n\n\t\tS: " << reply_code[16];
            send_data(client_sockfd, reply_code[16]);                   ///send ERROR
        }
    }

    else if (strncmp(request, "DATA", 4) == 0)
    {
        if (mail_stat == 4)
        {
            cout << "\n\n\t\tS: " << reply_code[8];
            send_data(client_sockfd, reply_code[8]);                    ///ask for DATA
            mail_data(client_sockfd);                                   ///receive,print & save DATA
            mail_stat = 5;
        }
        else
        {
            cout << "\n\n\t\tS: " << reply_code[16];
            send_data(client_sockfd, reply_code[16]);                   ///send ERROR
        }
    }

    else if (strncmp(request, "QUIT", 4) == 0)
    {
        mail_stat = 0;
        return;
    }

    else
    {
        cout << "\n\n\t\tS: " << reply_code[15];
        send_data(client_sockfd, reply_code[15]);
        return;
    }
}



/// MAIL PROCESSING FUNCTION

void  mail_proc(int client_socket)
{
    int client_sockfd, len;
    char buf[BUF_SIZE];

    memset(buf, 0, sizeof(buf));
    client_sockfd =  client_socket;

    send_data(client_sockfd, reply_code[4]);                    ///send 220
    cout << "\n\n\t\tS: " << reply_code[4];
    mail_stat = 1;

    while (1)
    {
        memset(buf, 0, sizeof(buf));
        len = read(client_sockfd, buf, sizeof(buf));
        if(strcmp(buf,"QUIT")==0)
        {
            cout << "\n\n\t\tC: " << buf;
            cout << "\n\n\t\tS: " << reply_code[5];             ///send BYE
            break;
        }
        if (len > 0)
        {
            cout << "\n\n\t\tC: " << buf;
            respond(client_sockfd, buf);
        }
        else
        {
            break;
        }
    }
    cout << "\n\n\t\tSocket is closed by client." << endl;
    cout << "\t============================================================\n\n";
    return;
}

#endif // DATATRANSFER_H_
