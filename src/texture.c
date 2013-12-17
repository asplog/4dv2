//
//  texture.c
//  P4
//
//  Created by Kota Nakano on 16/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//

#include<unistd.h>
#include<stdio.h>
#include"texture.h"
#include"gl.h"
int texture_init2d(texture_t*self,unsigned int name,unsigned int w,unsigned int h,void*data,int dump)
{
	char buff[16];
	self->target=name;
	glGenTextures(1,&self->texture);
	glBindTexture(GL_TEXTURE_2D,self->texture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	return 0<dump?write(dump,buff,sprintf(buff,"%d",glGetError())):0;
}
int tx_blank2d(tx_t*self,unsigned int name,unsigned int w,unsigned int h,float r,float g,float b,float a,int dump)
{
	char buff[16];
	float rgba[]={r,g,b,a};
	self->target=name;
	glActiveTexture(GL_TEXTURE0+self->target);
	glGenTextures(1,&self->texture);
	glBindTexture(GL_TEXTURE_2D,self->texture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,1,1,0,GL_RGBA,GL_FLOAT,rgba);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	return 0<dump?write(dump,buff,sprintf(buff,"%d",glGetError())):0;
}