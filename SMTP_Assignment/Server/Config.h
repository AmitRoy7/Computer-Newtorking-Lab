
#ifndef CONFIG_H_
#define CONFIG_H_
#include <limits.h>

/// Mail Server configurations
#define PORT 20001
#define MAX_CLIENTS 32
#define MAX_RCPT_USR 50
#define BUF_SIZE 1024

/// User information saving configurations
char hostname[50];
string data_dir;
char userinfo[] = "userinfo";

 int mail_stat;
 char from_user[30];
 char rcpt_user[30];


#endif
