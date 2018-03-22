
#include "headers.h"
#include "ipAddress.h"
#include "Date_Time.h"
#include "Config.h"
#include "DataTransfer.h"

#include<bits/stdc++.h>
#include <unistd.h>
#include <limits.h>
using namespace std;

void quit(int arg) {

		printf("\nS:Caught signal (%d). Mail server shutting down...\n\n", arg);
}

int main(int argc,char* argv[]) {
    ///FOR TERMINATION USING CTRL-C
    signal(SIGTERM, quit);

    ///SERVER & CLIENT SOCKET VARIABLES
	int server_sockfd, client_sockfd;
	socklen_t sin_size;
	struct sockaddr_in server_addr, client_addr;

	memset(&server_addr, 0, sizeof(server_addr));

	///CREATING SOCKET
	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket Create Error.\n");
		exit(1);
	}

	///set the socket's attributes
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(server_addr.sin_zero), 8);

	///create a link
	if (bind(server_sockfd, (struct sockaddr *) &server_addr,
			sizeof(struct sockaddr)) == -1) {
        perror("Socket Bind Error.\n");
        exit(1);
	}

	/// set to non-blocking to avoid lockout issue
	fcntl(server_sockfd, F_SETFL, fcntl(server_sockfd, F_GETFL, 0)|O_NONBLOCK);

	///listening requests from clients
	if (listen(server_sockfd, MAX_CLIENTS - 1) == -1) {
		printf("Socket Listen Error\n");
		exit(1);
	}

	///accept requests from clients,loop and wait.
	sin_size = sizeof(client_addr);
    gethostname(hostname,HOST_NAME_MAX);

    cout<<"\n\n\t\t\t=========================================="<<endl;
    cout<<"\t\t\t  SMTP mail server by [AMIT ROY] started"<<endl;
    cout<<"\t  HOSTNAME: "<<hostname<<"   IP Address: "<<ipAddress()<<"     PORT: "<<PORT<<endl;


	while (1) {

		if ((client_sockfd = accept(server_sockfd,
				(struct sockaddr *) &client_addr, &sin_size)) == -1) {
			sleep(1);
			continue;
		}
		cout << "\n\n\t\tS: Received a connection from "
			 << inet_ntoa(client_addr.sin_addr) << " at "
			 << find_Time()<< endl;

		mail_proc(client_sockfd);
	}

	close(client_sockfd);
	return 0;
}



