#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_LENGTH 50
#define MAX_LINE_LENGTH 100
#define MAX_ENTRIES 100
#define MAX_LINE_LENGTH_ERROR 40

struct Flight 
{
	char source[20];
	char destination[20];
	int fare;
};

/*
  Function name  : parseLine()
  Description    : This function parses a line of flight data, extracting source, destination, and fare.
  Parameters     : char* source is a pointer to store the source extracted from the line
				   char* destination is a pointer to store the destination extracted from the line
				   int* fare is a pointer to store the fare extracted from the line
				   char* line is the line of flight data to be parsed
  Returns        : If parsing is successful, it returns 1. Otherwise, it returns 0.
*/
int parseLine(char* source, char* destination, int* fare, char* line)
{
	char* token;
	char* next_token = NULL;
	token = strtok_s(line, ",", &next_token);
	if (token == NULL)
	{
		//display error if comma is missing
		printf("Error: Missing comma in line\n");
		return 0;
	}

	strcpy_s(source, 20, token);

	token = strtok_s(NULL, ",", &next_token);
	if (token == NULL)
	{
		//display error if missing fare in line
		printf("Error: Missing fare in line\n");
		return 0;
	}

	//converting fare to integar
	*fare = atoi(token);

	token = strtok_s(NULL, ",",&next_token);
	if (token == NULL)
	{
		//display error if missing destination in line
		printf("Error: Missing destination in line\n");
		return 0;
	}

	//copying destination into destination array
	strcpy_s(destination, 20, token);
	return 1;
}

/*
  Function name  : processFlight()
  Description    : This function processes a flight data file, extracting flight information and storing it in the flights array.
  Parameters     : char* filename is the name of the flight data file to be processed
				   struct Flight* flights is an array to store the parsed flight information
				   int* count is a pointer to track the number of flights processed
  Returns        : If file opening is successful, it returns 1. Otherwise, it returns 0.
*/
int processFlight(char* filename, struct Flight* flights, int* count)
{
	FILE* file;
	if(fopen_s(&file, filename, "r") != 0)
	{
		printf("Error: Unable to open file %s\n", filename);
		return 0;
	}

	char line[MAX_LINE_LENGTH];
	while (fgets(line, sizeof(line), file) != NULL && *count < MAX_ENTRIES)
	{
		char source[20], destination[20];
		int fare;
		if (strlen(line) > MAX_LINE_LENGTH_ERROR)
		{
			printf("Error: Line exceeds maximum length of %d characters\n", MAX_LINE_LENGTH_ERROR);
			continue;
		}
		if (parseLine(source, destination, &fare, line))
		{
			strncpy_s(flights[*count].source, source, sizeof(flights[*count].source) - 1);
			flights[*count].source[sizeof(flights[*count].source) - 1] = '\0';
			strncpy_s(flights[*count].destination, destination, sizeof(flights[*count].destination) - 1);
			flights[*count].destination[sizeof(flights[*count].destination) - 1] = '\0';
			flights[*count].fare = fare;
			(*count)++;
			if (*count >= MAX_ENTRIES)
			{
				printf("Warning: Maximum number of entries reached. Skipping remaining flights\n");
				break;
			}
		}
	}
	fclose(file);
	return 1;
}

/*
  Function name  : displayLeastFareDetails()
  Description    : This function displays the flight with the least fare from the flights array.
  Parameters     : struct Flight* flights is an array containing the flight information
				   int count is the number of flights in the array
  Returns        : None
*/
void displayLeastFareDetails(struct Flight* flights, int count)
{
	if (count == 0)
	{
		printf("No fligt information avaliable\n");
		return;
	}
	int min_fare = flights[0].fare;
	int min_index = 0;
	for (int i = 1; i < count; i++)
	{
		if (flights[i].fare < min_fare)
		{
			min_fare = flights[i].fare;
			min_index = i;
		}
	}
	//displa
	printf("Lowest fare: %s: %s to %s, Fare $%d\n", flights[min_index].source, flights[min_index].source, flights[min_index].destination, flights[min_index].fare);
}

int main()
{
	const char* filenames[] = {"flights.txt"};

	struct Flight flights[MAX_ENTRIES];
	int count = 0;

	int i = 0;
	for (i = 0; i < sizeof(filenames) / sizeof(filenames[0]); i++)
	{
		FILE* flights_file;
		//open file in read mode
		if (fopen_s(&flights_file, filenames[i], "r") != 0)
		{
			printf("Error: Unable to open %s\n", filenames[i]);
			//return 1 indicating failure to open file
			return 1;
		}

		char filename[MAX_FILENAME_LENGTH];

		//read each line from the file
		while (fgets(filename, sizeof(filename), flights_file) != NULL)
		{
			filename[strcspn(filename, "\n")] = 0;
			// skip this filename and continue to next one
			if (strlen(filename) > MAX_FILENAME_LENGTH)
			{
				printf("Error: Filename exceeds maximum length of %d characters\n", MAX_FILENAME_LENGTH);
				continue;
			}

			//check if filenmae is empty
			if (filename[0] == '\0')
				//skip empty filename
				continue;

			if (processFlight(filename, flights, &count) == 0)
			{
				printf("Error processing file: %s\n", filename);
			}
		}

		//close the file
		fclose(flights_file);
	}

	//display flight with least fare
		displayLeastFareDetails(flights, count);

		return 0;
}