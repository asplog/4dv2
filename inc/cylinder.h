//
//  poly.h
//  P4
//
//  Created by Kota Nakano on 17/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//
#ifndef P4_cylinderxyz_h
#define P4_cylinderxyz_h
#include"sprite.h"
int sprite_cylinder_init_x(sprite_t*,float,float,float,int,int);
int sprite_cylinder_init_y(sprite_t*,float,float,float,int,int);
int sprite_cylinder_init_z(sprite_t*,float,float,float,int,int);
int sprite_cylinder_init_any(sprite_t*,float vx,float vy,float vz,float dx,float dy,float dz,int,int);
#endif
