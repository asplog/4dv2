//
//  line.c
//  P4
//
//  Created by Kota Nakano on 17/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//
#include<memory.h>
#include"gl.h"
#include"sprite.h"
#include"line.h"
int sprite_attrib_line(sprite_t*self,int dump)
{
	return 0;
}
int sprite_init_line(sprite_t*self,int dump)
{
	register unsigned int result=0;
	struct
	{
		float vertex[12][4];
		float normal[12][4];
		float ambient[12][4];
		float diffuse[12][4];
	}
	v=
	{
		{0},
		{
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
		},
		{
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},
			{1,1,1,1},	
		},
	};
	struct
	{
		unsigned char v;
		unsigned char d;
	}
	i[]=
	{
		{0x0,0x1},{0x1,0x2},{0x2,0x0},
		{0x3,0x4},{0x4,0x5},{0x5,0x3},
		{0x6,0x7},{0x7,0x8},{0x8,0x6},
		{0x9,0xa},{0xa,0xb},{0xb,0x9},
		
		{0x0,0x3},{0x3,0x6},{0x6,0x9},{0x9,0x0},
		{0x1,0x4},{0x4,0x7},{0x7,0xa},{0xa,0x1},
		{0x2,0x5},{0x5,0x8},{0x8,0xb},{0xb,0x2},
	};
	result+=sprite_init(self,dump);
	
	glBindBuffer(GL_ARRAY_BUFFER,self->buffer[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(v),&v,GL_DYNAMIC_DRAW);
	
	self->vertex.stride=sizeof(float)*4,self->vertex.offset=sizeof(float)*12*0x0;
	self->normal.stride=sizeof(float)*4,self->normal.offset=sizeof(float)*12*0x4;
	self->ambient.stride=sizeof(float)*4,self->ambient.offset=sizeof(float)*12*0x8;
	self->diffuse.stride=sizeof(float)*4,self->diffuse.offset=sizeof(float)*12*0xc;
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->buffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(i),&i,GL_STATIC_DRAW);
	
	self->length=sizeof(i);
	self->stride=GL_UNSIGNED_BYTE;
	
	return result;
}
