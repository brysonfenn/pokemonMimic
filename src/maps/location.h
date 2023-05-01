#ifndef LOCATION_H
#define LOCATION_H

typedef struct Location {
    char x;
    char y;
    short action; //Should be -1 if this is a door
    int map; // # map that this location is a part of
    struct Location * next_loc; //Where a door leads (if applicable)
} Location;



#endif //LOCATION_H