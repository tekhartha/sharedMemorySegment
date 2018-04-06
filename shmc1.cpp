/* shmc1.cpp */

#include "registration.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <memory.h>


using namespace std;


CLASS *class_ptr;
void *memptr;
char *pname;
int shmid, ret;
void rpterror(char *), srand(), perror(), sleep();
void sell_seats();

main(int argc, char* argv[])
{
	if (argc < 2) {
		fprintf (stderr, "Usage:, %s shmid\n", argv[0]);
		exit(1);
	}
	
	pname = argv[0];
	sscanf (argv[1], "%d", &shmid);
	memptr = shmat (shmid, (void *)0, 0);
	if (memptr == (char *)-1 ) {
		rpterror ("shmat failed");
		exit(2);
	}

	class_ptr = (struct CLASS *)memptr;

	sell_seats();

	ret = shmdt(memptr);
	exit(0);
}

void sell_seats()
{
	int all_out = 0;
	srand ( (unsigned) getpid() );
	while ( !all_out) { /* loop to sell all seats */
		if (class_ptr->seats_left > 0) {
			sleep ( (unsigned)rand()%5 + 1);
			class_ptr->seats_left--;
			sleep ( (unsigned)rand()%5 + 1);
			cout << pname << " SOLD SEAT -- " << class_ptr->seats_left << " left" << endl;
		}
		else {
			all_out++;
			cout << pname << " sees no seats left" << endl;
		}
		sleep ( (unsigned)rand()%10 + 1);
	}
}


void rpterror(char* string)
{
	char errline[50];

	sprintf (errline, "%s %s", string, pname);
	perror (errline);
}
