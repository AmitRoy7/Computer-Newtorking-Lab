#ifndef MESSAGES_H_
#define MESSAGES_H_


///a header file containing RFC for SMTP

const char reply_code[][100]={
	{" "},  //0
	{"200 Mail OK."},   //1
	{"211 System status, or system help reply."},  //2
	{"214 Help message."},   //3
	{"220 Ready."},  //4
	{"221 Bye."},  //5
	{"250 OK."},  //6
	{"251 User not local; will forward to %s<forward-path>."},  //7

	{"354 End data with <CR><LF>.<CR><LF>."},  //8
	{"421 service not available, closing transmission channel."},  //9
	{"450 Requested mail action not taken: mailbox unavailable."},  //10
	{"451 Requested action aborted: local error in processing."},   //11
	{"452 Requested action not taken: insufficient system storage."}, //12
	{"500 Syntax error, command incomplete."},  //13
	{"501 Syntax error in parameters or arguments."},  //14
	{"502 Error: command not implemented."},  //15
	{"503 Error: Bad sequence of commands."}, //16
	{"504 Error: Command parameter not implemented."},  //17
	{"521 <domain>%s does not accept mail (see rfc1846)."},  //18
	{"530 Access denied (???a Sendmailism)."},  //19
	{"550 Requested action not taken: mailbox unavailable."},  //20
	{"551 User not registered."},  //21
	{"552 Requested mail action aborted: exceeded storage allocation."},  //22
	{"553 authentication is required."},  //23

	{"250-mail 250-PIPELINING 250-AUTH LOGIN PLAIN250-AUTH=LOGIN PLAIN250 8BITMIME."},  //24
	{"334 dXNlcm5hbWU6."},  //25, "dXNlcm5hbWU6" is "username:"'s base64 code
	{"334 UGFzc3dvcmQ6."},  //26, "UGFzc3dvcmQ6" is "Password:"'s base64 code
	{"235 Authentication successful."} //27
};

#endif // MESSAGES_H_
