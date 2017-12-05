#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "observer.h"

#define M_PI 3.14159265

struct mammal {
	char mammal_type; /* 'P' for porpoise 'D' for dolphin */
	struct location loc;
};

float deg_to_rad(float degrees) {
	return (degrees * M_PI) / 180.0;
}

int main() {
	printf("Enter file location for observer locations: ");
	char filename[30] = {};
	scanf("%30s", filename);
	
	struct observer * observers;
	struct date observers_date;
	
	int num_observers = read_observer_data(filename, &observers, &observers_date);
	
	printf("\n%d year, %d month, %d day, %d hour, %d min, %d sec\n\n", observers_date.year, observers_date.month, observers_date.day, observers_date.hour, observers_date.min, observers_date.sec);
	
	print_observers(num_observers, &observers);
	
	printf("\nEnter file location for locations of sightings: ");
	scanf("%30s", filename);
	printf("\n");
	
	int total_sightings = read_sighting_data(filename, num_observers, &observers);
	
	print_observers(num_observers, &observers);
	
	/* calculate locations */
	
	/* Array of mammals */
	struct mammal * mammals = malloc(total_sightings * sizeof(struct mammal));
	
	int curr_mammal = 0;
	for (int ob = 0; ob < num_observers; ob ++)
	{
		for (int st = 0; st < observers[ob].num_sightings; st++)
		{
			mammals[curr_mammal].mammal_type = observers[ob].sightings[st].mammal_type;
			
			/* CMLAT = OLAT + (RANGE * cos(BGR)) / 60.0; */
			mammals[curr_mammal].loc.lat = observers[ob].loc.lat + (observers[ob].sightings[st].range * cos(deg_to_rad(observers[ob].sightings[st].bearing))) / 60.0;
			
			/* CMLONG = OLONG + (RANGE * sin(BGR) / cos(OLATR)) / 60.0; */
			mammals[curr_mammal].loc.lon = observers[ob].loc.lon + ((observers[ob].sightings[st].range * sin(deg_to_rad(observers[ob].sightings[st].bearing))) / cos(deg_to_rad(observers[ob].loc.lat))) / 60.0;
			
			/* If mammal location is outside of the box specified then set its mammal type to N to indicate null*/
			if (mammals[curr_mammal].loc.lat < 52.000 || mammals[curr_mammal].loc.lat > 52.833 || mammals[curr_mammal].loc.lon < -5.500 || mammals[curr_mammal].loc.lon > -4.000)
			{
				mammals[curr_mammal].mammal_type = 'N';
			}
			
			curr_mammal++;
		}
	}
	
	printf("\n");
	
	for (int i = 0; i < total_sightings; i++) {
		if (mammals[i].mammal_type != 'N') {
			printf("mammal type: %c, latitude: %f, longtitude: %f\n", mammals[i].mammal_type, mammals[i].loc.lat, mammals[i].loc.lon);
		}
	}
	
	printf("\n");
	
	printf("Enter file location to save mammal locations: ");
	scanf("%30s", filename);
	
	{
		FILE * file = fopen(filename, "w");
		
		for (int i = 0; i < total_sightings; i++) {
			if (mammals[i].mammal_type != 'N') {
				fprintf(file, "%c %f %f\n", mammals[i].mammal_type, mammals[i].loc.lat, mammals[i].loc.lon);
			}
		}
		fclose(file);
	}
	
	return EXIT_SUCCESS;
}
