#ifndef __KEY_H
#define __KEY_H

#include "common.h"

#define  KEY_I_LEFT     PAin(19)
#define  KEY_I_RIGHT    PAin(29)

#define  KEY_5_UP  		PAin(28)
#define  KEY_5_DOWN  	PAin(26)
#define  KEY_5_LEFT  	PAin(25)
#define  KEY_5_RIGHT  	PAin(27)
#define  KEY_5_MIDDLE   PAin(24)

void key_init(void);
int  key_scan(void);

#endif

