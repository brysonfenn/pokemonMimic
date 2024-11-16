#ifndef SAFARI_ZONE_H
#define SAFARI_ZONE_H

struct Pokemon;

//Begin a Battle with a pokemon in the safari zone
int safari_zone_encounter(struct Pokemon * enemyPoke);

// Begin Safari Zone - give Safari balls etc.
void setup_safari_zone();

#endif // SAFARI_ZONE_H