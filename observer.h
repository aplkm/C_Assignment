#ifndef OBSERVER_H
#define OBSERVER_H

#include <stdlib.h>
#include <stdio.h>

struct location {
	float lat; /* latitude in degrees */
	float lon; /* longtitude in degrees */
};

struct sighting {
	char mammal_type; /* 'D' for dolphin and 'P' for porpoise */
	float bearing; /* bearing of observer */
	float range; /* range from observer to mammal in nautical miles */
};

struct observer {
	char id[5]; /* ID for observer */
	struct location loc;
	int num_sightings;
	struct sighting * sightings; /* array of sightings */
};

struct date {
	int day, month, year, hour, min, sec;
};

int read_num_lines(FILE * file);
int read_observer_data(char * filename, struct observer ** observers_arr, struct date * observer_date);
int read_sighting_data(char * filename, int num_observers, struct observer ** observers_arr);
void print_observers(int num_observers, struct observer ** observers_arr);

#endif
