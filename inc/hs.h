//
//  hs.h
//  P4
//
//  Created by Kota Nakano on 16/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//

#ifndef P4_hs_h
#define P4_hs_h
#include"sprite.h"
typedef enum
{
	hs_DEPTH_CLEAR=1<<0x0,
	
	hs_DEPTH_MODE=1<<0x1,
	hs_BLEND_MODE=1<<0x2,
	
	hs_DRAW_EDGE=1<<0x6,
	hs_DRAW_FILL=1<<0x7,
	hs_DRAW_LINE=1<<0x8,
	hs_DRAW_NAME=1<<0x9,
	hs_DRAW_DOTS=1<<0xa,

	hs_ROTATION_UNWRAP=1<<20,

}
hs_draw_flag;
typedef struct
{
	unsigned int name;
	unsigned int flag;
	unsigned int frame;
	sprite_t bill,poly[4],line,text,dots;
	float	proj[4][4];
	float	pers[4][4];
	short	root[2];
	float	back[4];	
	float	view[4];	
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
	unsigned char dict[12];
	struct
	{
		float	decay;
		float	accel;
		float	shift;
		float	any;

		float	rad;
		float	dst;
		float	cur;
		float	vel;
	}
	alpha,theta;
	struct
	{
		float	modelview[4][4];
		float	reshape[4][4];
		union
		{
			struct
			{
				float	ambient[4];
				float	diffuse[4];
			}
			fill;
			struct
			{
				float	ambient[4];
				float	diffuse[4];
			}
			light;
		};
		union
		{
			struct
			{
				float	ambient[4];
				float	diffuse[4];
			}
			edge;
			struct
			{
				float	ambient[4];
				float	diffuse[4];
			}
			unlit;
		};
		float width;
		unsigned int flag;
		unsigned int refs;
	}
	shape[12],label[12];
	struct
	{
		float	ambient[4];
		float	diffuse[4];
		float	width;
	}
	lines;
	struct
	{
		unsigned int flags;
		unsigned int reserv[3];
		struct
		{
			unsigned int resol;
			unsigned int reserv[3];
			float	scale[4];
			float	ambient[4];
			float	diffuse[4];
		}
		attrib[8];
	}
	geodes;
}
hs_t;
int hs_init(hs_t*,unsigned int,short,short,int);
int hs_root(hs_t*,short,short,int);
int hs_draw(hs_t*,int);
int hs_perspective(hs_t*,float,float,float,float,int);
int hs_lookat(hs_t*,float,float,float,float,float,float,float,float,float,int);
int hs_light(hs_t*,float,float,float,int);
int hs_update(hs_t*,int);
int hs_back(hs_t*,float,float,float,float,int);
int hs_view(hs_t*,float,float,float,float,int);
int hs_term(hs_t*,int);

int hs_handler(hs_t*,const char*,unsigned int,int);
int hs_handler_help();
#endif
