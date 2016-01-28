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


	// Variables
    struct timeval sp;
    FILE *fs, *fe;
    int i;

    // Open Files
    if ((fs=fopen("startTime1.txt", "w+"))== NULL) {
		fprintf(stderr, "ERROR opening %s\n", "RecordOutput");
	}

	if ((fe=fopen("startTime2.txt", "w+"))== NULL) {
		fprintf(stderr, "ERROR opening %s\n", "RecordOutput");
	}
	
	// Loop through forks
	for (i = 0; i < 1000; ++i)	{
		
	gettimeofday(&sp, NULL);
	fprintf(fs, "%d\n", sp.tv_usec);

		if (fork()) {
			execl("./write", "./write", "resultTime1.txt", (char *)0);
		}
		else {
			
			gettimeofday(&sp, NULL);
			fprintf(fe, "%d\n", sp.tv_usec);

			if (fork())	{
				execl("./write", "./write", "resultTime2.txt", (char *)0);
			}
		}
	}

	printf("Finished!\n");
	fclose(fs);
	fclose(fe);

	return 0;
}