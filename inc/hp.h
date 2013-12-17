//
//  hs.h
//  P4
//
//  Created by Kota Nakano on 16/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//

#ifndef P4_hp_h
#define P4_hp_h
#include"sprite.h"
typedef enum
{
	hp_DEPTH_CLEAR=1<<0x0,
	
	hp_DEPTH_MODE=1<<0x1,
	hp_BLEND_MODE=1<<0x2,
	
	hp_DRAW_AXIS=1<<0x04,
	hp_DRAW_LABEL=1<<0x05,
	hp_DRAW_TICK=1<<0x06

}
hd_draw_flag;
typedef struct
{
	unsigned int name;
	unsigned int flag;
	unsigned int frame;
	sprite_t bill,cylinder[3],string,sphere;
	short	root[2];
	float	back[4];
	float	view[4];
	struct
	{
		float	matrix[4][4];
		float	from[3];
		float	to[3];
		float	top[3];
		float	dummy[7];
	}
	pinhole;
	struct
	{
		float	matrix[4][4];
		float	lr[2];
		float	tb[2];
		float	nf[2];
		float	dummy[2];
	}
	project;
	struct
	{
		float	ambient[4];
		float	diffuse[4];
		union
		{
			float	width;
			float	scale;
		};
	}
	axis[3],object[1];
	struct
	{
		struct
		{
			float	ambient[4];
			float	diffuse[4];
		}n,p;
		float	scale;
		char	label[60];
	}
	label[3];
	struct
	{
		float	from[4];
		float	to[4];
		float	fovy;
		union
		{
			struct
			{
				float	up[3];
				float	aspect,zmin,zmax;
			};
			struct
			{
				float	colour[4];			
			};
		};
	}
	camera,light;	
	struct
	{
		float	decay;
		float	accel;
		float	shift;

		float	dst;
		float	cur;
		float	vel;
	
		float	max;
		float	min;
	}
	value[3];
}
hp_t;
int hp_init(hp_t*,unsigned int,short,short,int);
int hp_root(hp_t*,short,short,int);
int hp_draw(hp_t*,int);
int hp_perspective(hp_t*,float,float,float,float,int);
int hp_lookat(hp_t*,float,float,float,float,float,float,float,float,float,int);
int hp_light(hp_t*,float,float,float,int);
int hp_reshape(hp_t*,int);
int hp_update(hp_t*,int);
int hp_back(hp_t*,float,float,float,float,int);
int hp_view(hp_t*,float,float,float,float,int);
int hp_term(hp_t*,int);

int hp_handler(hp_t*,const char*,unsigned int,int);
int hp_handler_help();
#endif
