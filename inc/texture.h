//
//  texture.h
//  P4
//
//  Created by Kota Nakano on 16/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//

#ifndef P4_texture_h
#define P4_texture_h
typedef struct
{
	unsigned int texture;
	unsigned int target;
	unsigned int width;
	unsigned int height;
	unsigned int depth;
	unsigned int*cache;
}
texture_t;
typedef struct
{
	unsigned int texture;
	unsigned int target;
}
tx_t;
int texture_init2d(texture_t*,unsigned int,unsigned int,unsigned int,void*,int);
int tx_blank2d(tx_t*,unsigned int,unsigned int,unsigned int,float,float,float,float,int);
#endif
