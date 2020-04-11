#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#define size 50

int main(int arguments, char* arv[])
{
	int file[2];
	pid_t child_id;
	char readBuff[size];
	pipe(file);
	if(arguments !=3)
	{
		printf("ERROR: Need exactly two parameters.\n");
		exit(1);
	}
	int fileOpen = open(arv[1], 0);
	int Targetfile = open(arv[2], O_RDWR|O_CREAT|O_APPEND, 0666);
	if(fileOpen == -1 || Targetfile == -1)
	{
		printf("File cannot be open\n");
		exit(1);
	}
	child_id = fork();
	if (child_id == 0)
	{
		close(file[1]);
		while(read(file[0], readBuff, sizeof(readBuff)) > 0)
		{
			write(Targetfile, readBuff, strlen(readBuff) - 1);
		}
		close(file[0]);
		close(Targetfile);
	}	
	else 
	{
		close(file[0]);
		while(read(fileOpen,readBuff, sizeof(readBuff)) > 0);
		{
			write(file[1], readBuff, sizeof(readBuff));
			memset(readBuff, 0, size);
		}
		close(file[1]);
		close(fileOpen);
		wait(NULL);
	}
}
