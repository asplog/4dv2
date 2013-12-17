//
//  bill.c
//  P4
//
//  Created by Kota Nakano on 18/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//

#include<stdio.h>
#include<memory.h>
#include"bill.h"
#include"gl.h"
int sprite_bill_init(sprite_t*self,int dump)
{
	register int result=0;
	struct
	{
		vec4_t vertex;
		vec4_t normal;
		vec4_t ambient;
		vec4_t diffuse;
	}
	buff[4]=
	{
		{{-1,-1, 1,1},{0,0,-1,1},{1,1,1,1},{0,0,0,0}},
		{{ 1,-1, 1,1},{0,0,-1,1},{1,1,1,1},{0,0,0,0}},
		{{-1, 1, 1,1},{0,0,-1,1},{1,1,1,1},{0,0,0,0}},
		{{ 1, 1, 1,1},{0,0,-1,1},{1,1,1,1},{0,0,0,0}},
	};
	glGenBuffers(2,self->buffer);
	glBindBuffer(GL_ARRAY_BUFFER,self->buffer[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(buff),&buff,GL_STATIC_DRAW);
	self->vertex.stride=sizeof(vec4_t)*4,self->vertex.offset=sizeof(vec4_t)*0;
	self->normal.stride=sizeof(vec4_t)*4,self->normal.offset=sizeof(vec4_t)*1;
	self->ambient.stride=sizeof(vec4_t)*4,self->ambient.offset=sizeof(vec4_t)*2;
	self->diffuse.stride=sizeof(vec4_t)*4,self->diffuse.offset=sizeof(vec4_t)*3;
	
	unsigned char i[]={0,1,2,3,2,1};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->buffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(i),i,GL_STATIC_DRAW);
	self->length=sizeof(i);
	self->stride=(void*)GL_UNSIGNED_BYTE;
	return result;
}
