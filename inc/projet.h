//
//  projet.h
//  P4
//
//  Created by Kota Nakano on 18/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//

#ifndef P4_projet_h
#define P4_projet_h
#include"matrix.h"
mat4_t mat4_ortho(float,float,float,float,float,float);
mat4_t mat4_frustum(float,float,float,float,float,float);
mat4_t mat4_perspective(float,float,float,float);
mat4_t mat4_look(float,float,float,float,float,float,float,float,float);

#endif
