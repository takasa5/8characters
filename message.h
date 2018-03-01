
#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>

#define DATA_SIZE 512
#define CONTINUE 0
#define ACK 1
#define END 2
#define PORT 12343


struct Message {
	char now[DATA_SIZE];
	int status;
	int score;
};

int check_input(char *buf)
{
	//不正な場合not0、正しい場合0
	strtok(buf, "\n\0");
	if (strlen(buf) != 8) {
		printf("8文字入力してください.\n");
		return -1;
	}
	int i;
	for (i=0; i < 8; i++) {
		if (buf[i] < 'a' || buf[i] > 'z') {
			printf("小文字のアルファベットで入力してください.\n");
			return -1;
		}
	}
	return 0;
}

void calc_score(char* mine, char* yours, int* score)
{
	int i;
	char tmp[8], *p;
	strncpy(tmp, yours, 8);
	for (i=0; i < 8; i++) {
		p = strchr(tmp, mine[i]);
		if (p != NULL) {
			(*score)++;
			tmp[p-tmp] = '1';
		}
	}
}


#endif /* _MESSAGE_H_ */
