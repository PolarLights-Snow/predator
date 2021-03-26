#ifndef __HX711_H
#define __HX711_H

#include "main.h"

#define HX711_SCK PBout(0)// PB0
#define HX711_DOUT PBin(1)// PB1


extern void Init_HX711pin(void);
extern int HX711_Read(void);
extern void Get_Maopi(void);
extern void Get_Weight(void);

extern int HX711_Buffer;
extern int Weight_Maopi;
extern int Weight_Shiwu;
extern int Flag_Error;
extern int get_HX711data(void);

#endif

