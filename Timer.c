/*

Timer.c
@author Kyle O'Neill

The purpose of this program is to measure the time it takes for an application to 
start. These times will come from the launching of ./write which is the binary that
is created from the ReadWrite.c program. This program needs to be run for several 
minutes so there are 100,000 executions of this program which allows us to analyze the
times required to start this particular program.

To tackle this problem the first thing I needed was to generate files for the timestamps
to be writen to for later analyze. The best way to do this was to put each time into the
file on a new line. I decided to have a total of 4 different files to collect all the times.
Two of the files would contain the start times before the process starts. One for the first
run of the application and another for the second one. The two other files would hold the 
results but this files are created inside the ReadWrite by being passed as an argument. 
These files would hold the time stamps after the program starts.

The second part of the problem was launching the applications themselves. To do this, I first got the timestamp
of the program and printed it to the startTime1.txt. Then the program calls the fork method
which copies the current process. The program then checks to see if itself is a fork or not. 
If the program is a fork then it runs the program using the execl function. Inside this
function I also pass resultTime1.txt to record the program starts time.
Else, the program gets another time stamp and places it into startTime2.txt. The program then
calls the fork() function again and checks to see if the program is a child or not. If it is,
it uses execl with the parameter file name resultTime2.txt. This entire process happens in a 
loop. 

This process is shortened when the program needs only to run the program once per loop. To 
Create the data for that test, the internal conditional statement is commented out and
the program is then recomplied. Now the program runs only once

I also opened and closed the resultTime1.txt and resultTime2.txt files in write mode to
clear the files if there was any previous data left in them. The ReadWrite program only 
appends to the file so the old data needs to be cleaned before they can be used again.  

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	// ----------  Some Quick Housecleaning -----------
	// Clearing out old Files
	fclose(fopen("resultTime1.txt", "w"));
	fclose(fopen("resultTime2.txt", "w"));

	// -------------------------------------------------


	// ----------- Variables ------------------
    struct timeval sp, sd;		// Timestamp struct (from <sys/time.h>)
    FILE *fs, *fe;			// File Pointers to StartTime text files
    int i;					// Loop counter
  

    // Open Files
    if ((fs=fopen("startTime1.txt", "w+"))== NULL) {
		fprintf(stderr, "ERROR opening %s\n", "RecordOutput");
	}

	if ((fe=fopen("startTime2.txt", "w+"))== NULL) {
		fprintf(stderr, "ERROR opening %s\n", "RecordOutput");
	}
	
	// Get and print time stamp
	gettimeofday(&sp, NULL);
	fprintf(fs, "%d\n", sp.tv_usec);
	
	// Fisrt Fork call (!fork() == child)
	if (!fork()) {		
		execl("./write", "./write", "resultTime1.txt", (char *)0);
	}
	else {
		
		wait(NULL);

		// Second Fork
		if (!fork())	{
			// Get and print second time stamp
			gettimeofday(&sd, NULL);
			//fprintf(fe, "%d\n", sd.tv_usec);
			//printf("%d\n", sd.tv_usec);
			execl("./write", "./write", "resultTime2.txt", (char *)0);
		}
	}

	wait(NULL);

	// Print out program is finished to user
	printf("\nFinished!\n\n");
	fclose(fs);
	fclose(fe);
	

	return 0;
}