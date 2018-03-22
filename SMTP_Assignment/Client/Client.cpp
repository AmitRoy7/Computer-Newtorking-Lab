#include<bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "Date_Time.h"

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

string refine(string temp)
{
    string ret;
    for(int i=0; i<temp.size(); i++)
    {
        ret += temp[i];
        if(temp[i]=='.')
            break;
    }
    return ret;
}

///argv[0] = "./client";
///argv[1] = "hostname";       ///MAIL FROM
///argv[2] = "portnumber";
///argv[3] = "username";       ///RCPT TO
///argv[4] = "SUBJECT";        ///
///argv[5] = "filename";

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (argc < 3)
    {
        fprintf(stderr,"\n\n\t\tusage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        error("404 ERROR opening socket");

    server = gethostbyname(argv[1]);

    if (server == NULL)
    {
        fprintf(stderr,"\n\n\t\t404 ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("\n\n\t\t404 Server not found");

    int mail_state = 0;
    string buffer;
    string user_name = argv[3];
    string host_name = argv[1];
    string subject = argv[4];
    for(int i=5; i<argc-1; i++)
    {
        subject += " ";
        subject += argv[i];
    }
    string mailbody = "";

    char temp[50000];
    FILE*   fp = fopen(argv[argc-1],"r");

    while (fgets(temp, sizeof(temp), fp) > 0)
    {
        mailbody += (string)temp;
    }

    while(true)
    {
        n = read(sockfd, temp, 255);
        buffer=(string) temp;

        buffer = refine(buffer);

        if (n < 0)
            error("Error reading from socket");

        cout<<"\n\t\tS: "<<buffer<<endl;

        if(buffer[0]!='2' && buffer[0]!='3')
        {
            printf("\n\n\t\tError Occurred. Client Terminating\n");
            printf("\n\t\t====================================\n\n");
            //close(sockfd);return 0;
            break;
        }
        cout<<"\n\n\t\tC: ";
        getline(cin,buffer);


        if (n < 0)
            error("\n\n\t\tError writing to socket");


        if(mail_state==0 && buffer=="HELO")
        {
            mail_state  = 1;
        }
        else if(mail_state==1 && (strncmp(buffer.c_str(), "MAIL FROM", 9) == 0))
        {
            mail_state = 2;
            user_name = buffer.substr(9);
            string un = "";
            bool flag = false;
            for(int i=0; i<user_name.size(); i++)
            {
                if(user_name[i]=='<')
                {
                    flag = true;
                    continue;
                }
                else if(user_name[i]=='>')
                {
                    flag = false;
                    break;
                }
                if(flag)
                {
                    un += user_name[i];
                }

            }
            user_name = (string)un;
        }
        else if(mail_state==2 && strncmp(buffer.c_str(), "RCPT TO", 7) == 0)
        {
            mail_state = 3;
            host_name = buffer.substr(7);
            string hn = "";
            bool flag = false;
            for(int i=0; i<host_name.size(); i++)
            {
                if(host_name[i]=='<')
                {
                    flag = true;
                    continue;
                }
                else if(host_name[i]=='>')
                {
                    flag = false;
                    break;
                }
                if(flag)
                {
                    hn += host_name[i];
                }

            }
            host_name = (string)hn;
        }
        else if(mail_state==3 && buffer=="DATA")
        {
            write(sockfd, "DATA", buffer.length());
            read(sockfd, temp, 255);
            buffer = (string)temp;
            buffer = refine(buffer);
            cout<<"\n\t\tS: "<<buffer<<endl;
            cout<<"\n\t\tMAIL CONTENT:";

            buffer = "To: ";
            buffer += host_name;
            buffer += "\n";
            buffer += "From: ";
            buffer += user_name;
            buffer += "\n";
            buffer += "Subject: ";
            buffer += subject;
            buffer += "\n";
            buffer += "Date: ";
            buffer += find_Date();
            buffer += "\nTime: ";
            buffer += find_Time();

            buffer += "\n\n";

            buffer += mailbody;
            buffer += "\n";

            int l = buffer.size();
            cout<<"\n\n";
            bool pflag = false;
            for(int i=0; i<l; i++)
            {
                if(i<(l-1) && (buffer[i]==92 && buffer[i+1]=='n'))    ///92 = \

                {
                    printf("\n\n\t\t");
                    i = i+2;
                    pflag = true;
                    continue;
                }

                if(pflag)
                {
                    printf("\t\t%c",buffer[i]);
                    pflag = false;
                }
                else
                    printf("%c",buffer[i]);
            }
            mail_state = 4;
            string dot;
            cin>>dot;
            cin.ignore();
        }
        else if(mail_state==4 && buffer=="QUIT")
        {
            mail_state = 5;
            n = write(sockfd, buffer.c_str(), buffer.length());
            cout << "\n\n\t\tS: 221 Bye.";
            printf("\n\t\t====================================\n\n");
            break;
        }

        n = write(sockfd, buffer.c_str(), buffer.length());

    }
    close(sockfd);
    return 0;
}
