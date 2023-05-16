#ifndef LOCATION_H
#define LOCATION_H

typedef struct Location {
    char x;
    char y;
    short action; //Should be -1 if this is a door to another map
    int map; // # map that this location is a part of
    char next_x;
    char next_y;
    char next_map;
} Location;



#endif //LOCATION_H