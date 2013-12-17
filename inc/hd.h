//
//  head.h
//  P4
//
//  Created by Kota Nakano on 16/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//

#ifndef P4_head_h
#define P4_head_h
#include"sprite.h"
#include"texture.h"
enum
{
	hd_DEPTH_CLEAR=1<<0,
	hd_DEPTH_MODE=1<<1,
	hd_BLEND_MODE=1<<2,
};
typedef struct
{
	unsigned int name;
	unsigned int flag;
	unsigned int frame;
	sprite_t bill,head,face;
	float pers[4][4];
	float proj[4][4];
	short root[2];
	float back[4];
	float view[4];	
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
		float min;
		float max;
	}
	xrange,zrange;
	struct
	{
		unsigned int engine;
		unsigned int buffer;
		unsigned int w;
		unsigned int h;
		float*cache;
	}
	texture;
	struct
	{
		struct
		{
			char name[4];
			float x;
			float y;
			float z;
		}
		dict[256];
		unsigned char dictLength;
		unsigned char itemLength;
		unsigned char order[256];
		float	gain;
		float	bias;
	}
	eeg;
}
hd_t;
int hd_init(hd_t*,unsigned int,short,short,const char*,const char*,int);
int hd_update(hd_t*,int);
int hd_draw(hd_t*,int);
int hd_term(hd_t*,int);
int hd_back(hd_t*,float,float,float,float,int);
int hd_view(hd_t*,float,float,float,float,int);
int hd_root(hd_t*,short,short,int);
int hd_light(hd_t*,float,float,float,int);
int hd_ortho(hd_t*,float,float,float,float,float,float,int);
int hd_lookat(hd_t*,float,float,float,float,float,float,float,float,float,int);
int hd_eeg_dict(hd_t*,const char*,int);
int hd_eeg_order(hd_t*,const char*,int);

int hd_handler(hd_t*,const char*,unsigned int,int);
int hd_handler_help();
#endif
