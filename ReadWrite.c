/*

ReadWrite.c
@author Kyle O'Neill

The main purpose of this program is to read and write records to a text file inside the 
the directory. The secondary function of this program is to write a timestamp to a file
of when this program starts.

To appoarch this problem I divided this program into two parts. The first part was to 
read and write records into a file and to write a time stamp to another file.

For the first part of my program, I analysed the problem and decide to divide it into three parts.
The three parts of my program include a record generatior, a record reader, and a controler. 
These part help naturally divided the program into the three tasks I need
to meet one of the requirements of this lab.

The first part of the programm is the recordGen(). This function takes two parameters
a file pointer and an array of strings. The file pointer is the file which the reorcdGenerator()
is to write the records to and the array of strings is an array of all the records that are
written to the file. The function uses a nested for loop to generate the records. The inside 
loop, loops 120 times, placing a randonly generated chartacter into a temporary string. After
this loop is finished, that string has the EOF charter added, written to the array of strings
passed into the function and then written to the file. The outside loop repeates all of this to 
generate 10 seperate records inside the file pointer passed into the function.

The output of recordGen is the 10 records written to the file pointer that was passed in as 
an argument. The records contain 120 randonmly generated characters with ASCII values 65 - 122
plus one EOF charater. These records can contain any of the different possible combinations
of these particular characters.

The second part of the code is the searchFile(). This function has two parameters passed to it.
The two parameters are a FILE pointer and a string which contains one of the randomly generated
records that recordGen() created. This function then uses a for loop to linearly search the 
file for a record that matches the randomly generated one passed to the function. Inside the loop, 
I scaned the first record from the file and compared it to record that was passed into the function.
This loop will continue until a match is found or the loop has read all the records in the file. 
If the record was not found, a message would be print to the screen.

The third part of the program is the controller which is the main() in this program. The main 
calls the functions, opens and rewinds the file pointer and generates a random record for
recordGen() to search for. 

The other purpose of this code is to create a time stamp of when this program starts. To 
accomplish this, I used the gettimeofday function from the <sys/time.h> library. I used this
function to return a timeval struct, which is also apart of that same library. One of the 
fields of this struct holds a timestamp in mircoseconds. A file is opened up and thios time 
stamp is writen to the file. 

This program is designed to read and write to the files for several minutes. To accomplish this
I have both the recordGen(), searchFile() and some other logic inside a loop. The loop runs for 
a specified amount of iterations. The number of iterations is around 55000 to get this program 
to run for several minutes. This is tested on my own personal computer.

Everytime this process goes through the loop it opens, deletes and closes the file that the process
uses. The reason for this is if you don't clear the file out and reopen it, the file just continues
to grow. Prior to me figuring this out, my record.txt file contained millions of different records
created by the recordGen(). This has a significant impact on the runtime of this application. 

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

void recordGen(FILE *, char[10][121]);
void searchFile(FILE *fp, char *record);

int main(int argc, char const *argv[])
{	
	// ---- Get End Time ---------
	struct timeval end;			// Timestamp Struct
	FILE *fp;					// File pointer for timestamp output

	// Gets the timestamp
	gettimeofday(&end, NULL);

	// Open file to output timestamp too
	if ((fp=fopen(argv[1], "a"))== NULL) {
		fprintf(stderr, "ERROR opening %s\n", argv[1]);
	}
	
	// Print timestamp to file
	fprintf(fp, "%d\n", end.tv_usec); 
	fclose(fp);

	// --------------------------------

	// ------ Variables ----------------
	int randRecNum, i;				// Placeholder for rand number
	char records[10][121]; 		// Array of Strings to hold all the records generated
	char *randRec;				// Placeholder for the randomly generated record
	FILE *fs;					// File pointer to the text file where the records are writen.

	// Change rand() algorithm
	srand(time(NULL));

	for (i = 0; i < 55000; i++) {

		// Open file to put write/read records from
		if ((fs=fopen("Records.txt", "w+"))== NULL) {
			fprintf(stderr, "ERROR opening %s\n", argv[1]);
		}

		// Populate File
		recordGen(fs, records);

		// Pick a random record (0-9)
		randRecNum = rand() % 10;
		randRec = records[randRecNum];
		
		// Rewind File
		rewind(fs);

		// Read File and Search for Record
		searchFile(fs, randRec);

		remove("Records.txt"); 		// Deletes file as per spec
		fclose(fs);

	}
	
	return 0;
}

/* 
This function generates and writes records to file and array that 
is passed in as arguments.
*/
void recordGen(FILE *fp, char records[10][121]) {
	// --------- Input -----------
	//	*fp - File to write records too
	//  records - Array to hold all the records that are generated

	char string[121];	// Temporary place holder for generated records
	int i, j;			// Loop Counters

	for (j = 0; j < 10; j++) {
	
		for (i = 0; i < 120; i++) {

			// Generate random number for a letter
			// ---- USe ASCII numbers 65-122 -----

			string[i] = 65 + (rand() % (122-65));
			records[j][i] = string[i];
		}

	// EOS Char
	string[120] = '\0';
	records[j][120] = '\0';

	// Writes record and new line character to file
	fputs(string, fp);
	fputc('\n', fp);

	}

}

/*	
This function searches the file that records were writen too and compares
them to a randomly generated record.
*/
void searchFile(FILE *fp, char *record) {
	// --------------- Input -----------
	//	*fp - file that contains the records
	//	*record - a randonly selected record that was created by recordGen()

	char found = 0;		// Flag to break loop if found
	char str[121];		// Temporary placeholder for strings that are read from file


	// Loop to search through file - Linear Search
	while (fgets(str, 121, fp) && !found) {
		if (strcmp(str, record) == 0) {
			found = 1;
		}
	}

	// Print out statement if no matching record was found
	if (str == (char *)EOF)	{
		printf("Record Not Found.\n");
	}
}