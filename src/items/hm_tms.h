#ifndef HM_TMS_H
#define HM_TMS_H

#include "stdbool.h"

//All key item ID's
typedef enum { HM_EMPTY=240, HM_CUT } HM_TM;

char * get_hm_tm_name(int id);

//Returns false if player already has the key item
bool add_hm_tm(HM_TM id);

void teach_hm_tm(HM_TM id);

#endif // HM_TMS_H