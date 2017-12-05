#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "observer.h"

/*
* Takes a FILE as an input, then reads and returns the number of lines in that file.
*/
int read_num_lines(FILE * file) {
	/* flag to check for end of file */
	int flag;
	/* count number of lines */
	int num_lines = 0;
	while (flag != EOF) {
		char c;
		flag = fscanf(file, "%c", &c);
		if (c == '\n') num_lines++;
	}
	rewind(file); /* return to start of file so data can be read */
	return num_lines-1; /* do not count extra newline at tend of file */
}

/*
* Takes a filename and a pointer to a pointer that will be an array to hold all of the observers read in the file.
* Returns the number of observers to be used for iterating through the array
*/
int read_observer_data(char * filename, struct observer ** observers_arr, struct date * observer_date) {
	FILE * file = fopen(filename, "r");
	
	int num_observers = read_num_lines(file) - 1; /* -1 to account for date and time not being a line for observer data */
	
	fscanf(file, "%d %d %d %d %d %d", &observer_date->day, &observer_date->month, &observer_date->year, &observer_date->hour, &observer_date->min, &observer_date->sec);
	
	/* pointer to array of observer structures and allocate enough memory for all observers in file */
	*observers_arr = malloc(sizeof(struct observer) * num_observers);
	
	for (int i = 0; i < num_observers; i++) {
		fscanf(file, "%5s %f %f\n", (*observers_arr)[i].id, &(*observers_arr)[i].loc.lat, &(*observers_arr)[i].loc.lon);
		(*observers_arr)[i].num_sightings = 0; /* initialise value to 0 ready for reading sightings data */
	}
	
	fclose(file);
	
	return num_observers;
}

/* 
* Reads in sighting data and stores it in the observers_arr array similar to read_observer_data()
* Must already have used read_observer_data() to initialise and allocate array.
* Returns the total number of sightings
*/
int read_sighting_data(char * filename, int num_observers, struct observer ** observers_arr) {
	FILE * file = fopen(filename, "r");
	
	int total_sightings = read_num_lines(file);
	
	/* iterate through and calculate how many sightings there are for each observer */
	for (int i = 0; i < total_sightings; i++) {
		char curr_id[5];
		fscanf(file, "%5s %*c %*f %*f\n", curr_id);
		
		for (int ob = 0; ob < num_observers; ob++) {
			if (strcmp((*observers_arr)[ob].id, curr_id) == 0) {
				(*observers_arr)[ob].num_sightings++;
			}
		}
	}
	/* allocate enough memory to each observers sightings array and reset their number
	   of sightings value to help iterate through them */
	for (int ob = 0; ob < num_observers; ob++) {
		(*observers_arr)[ob].sightings = malloc((*observers_arr)[ob].num_sightings * sizeof(struct sighting));
		(*observers_arr)[ob].num_sightings = 0;
	}
	
	/* Now go back and iterate through sightings again this time reading the data into a sighting struct
	   then putting that data into each observers sightings array */
	rewind(file);
	for (int i = 0; i < total_sightings; i++) {
		char curr_id[5];
		struct sighting curr_sighting;
		fscanf(file, "%5s %c %f %f\n", curr_id, &curr_sighting.mammal_type, &curr_sighting.bearing, &curr_sighting.range);
		
		for (int ob = 0; ob < num_observers; ob++) {
			if (strcmp((*observers_arr)[ob].id, curr_id) == 0) {
				/* Num_sightings was reset to 0 before so it could be used here to access the right elemets in the sightings array */
				(*observers_arr)[ob].sightings[(*observers_arr)[ob].num_sightings] = curr_sighting;
				(*observers_arr)[ob].num_sightings++; 
			}
		}
	}
	
	fclose(file);
	
	return total_sightings;
}

/* 
* Prints an observer array and all of each observers sightings (if any)
*/
void print_observers(int num_observers, struct observer ** observers_arr) {
	for (int i = 0; i < num_observers; i++) {
		printf("id: %s, latitude: %f, longtitude %f\n", (*observers_arr)[i].id, (*observers_arr)[i].loc.lat, (*observers_arr)[i].loc.lon);
		for (int j = 0; j < (*observers_arr)[i].num_sightings; j++) {
			printf(" - sighting: type %c, bearing %f, range %f\n", (*observers_arr)[i].sightings[j].mammal_type, (*observers_arr)[i].sightings[j].bearing, (*observers_arr)[i].sightings[j].range);
		}
	}
}
