//
//  lcd.h
//  P4
//
//  Created by Kota Nakano on 17/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//

#ifndef P4_lcd_h
#define P4_lcd_h
#include"sprite.h"
int sprite_lcd_init(sprite_t*,int);
int sprite_lcd_light(sprite_t*,unsigned char,float*a0,float*d0,float*a1,float*d1,int);
int sprite_lcd_term(sprite_t*,int);
#endif
