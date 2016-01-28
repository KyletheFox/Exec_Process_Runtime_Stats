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
	struct timeval end;
	gettimeofday(&end, NULL);
	FILE *fp;

	if ((fp=fopen(argv[1], "a"))== NULL) {
		fprintf(stderr, "ERROR opening %s\n", argv[1]);
	}
	
	fprintf(fp, "%d\n", end.tv_usec);
	fclose(fp);

	// --------------------------------


	int i, randRecNum;
	char records[10][121]; 
	char *randRec;
	FILE *fs;

	if ((fs=fopen("Records.txt", "w+"))== NULL) {
		fprintf(stderr, "ERROR opening %s\n", argv[1]);
	}

	// Change rand() algorithm
	srand(time(NULL));

	// Populate File
	recordGen(fs, records);

	// Pick a random record (0-9)
	randRecNum = rand() % 10;
	randRec = records[randRecNum];

	// printf("Record Number: %d\n", randRecNum);
	//printf("Record to search for: %s\n", randRec);
	
	// Rewind File
	rewind(fs);

	// Read File and Search for Record
	searchFile(fs, randRec);

	remove("Records.txt");
	fclose(fs);
	return 0;
}

void recordGen(FILE *fp, char records[10][121]) {
	char string[121];
	int i, j;

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

	fputs(string, fp);
	fputc('\n', fp);

	}

}

void searchFile(FILE *fp, char *record) {
	char found = 0;
	char str[121];

	while (fgets(str, 121, fp) && !found) {
		if (strcmp(str, record) == 0) {
			found = 1;
		}
	}

	if (str == EOF)	{
		printf("Record Not Found.\n");
	}
}