//
//  geometry.c
//  P4
//
//  Created by Kota Nakano on 16/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//
#include<stdio.h>
#include<math.h>
#include"vector.h"

vec1_t vec1_pack(const float x){vec1_t v;v.x=x;return v;}
vec2_t vec2_pack(const float x,const float y){vec2_t v;v.x=x;v.y=y;return v;}
vec3_t vec3_pack(const float x,const float y,const float z){vec3_t v;v.x=x;v.y=y;v.z=z;return v;}
vec4_t vec4_pack(const float x,const float y,const float z,const float w){vec4_t v;v.x=x;v.y=y;v.z=z;v.w=w;return v;}

vec1_t vec1_copy(const float*p){return*(vec1_t*)p;}
vec2_t vec2_copy(const float*p){return*(vec2_t*)p;}
vec3_t vec3_copy(const float*p){return*(vec3_t*)p;}
vec4_t vec4_copy(const float*p){return*(vec4_t*)p;}

vec1_t vec1_shift(const vec1_t v,int s){float r[1];s=s<0?1-((-s)%1):s%1;r[(s+0)%1]=v.x;return*(vec1_t*)r;}
vec2_t vec2_shift(const vec2_t v,int s){float r[2];s=s<0?2-((-s)%2):s%2;r[(s+0)%2]=v.x;r[(s+1)%2]=v.y;return*(vec2_t*)r;}
vec3_t vec3_shift(const vec3_t v,int s){float r[3];s=s<0?3-((-s)%3):s%3;r[(s+0)%3]=v.x;r[(s+1)%3]=v.y;r[(s+2)%3]=v.z;return*(vec3_t*)r;}
vec4_t vec4_shift(const vec4_t v,int s){float r[4];s=s<0?4-((-s)%4):s%4;r[(s+0)%4]=v.x;r[(s+1)%4]=v.y;r[(s+2)%4]=v.z;r[(s+3)%4]=v.w;return*(vec4_t*)r;}

vec1_t vec1_vec1(const vec1_t v){return vec1_pack(v.x);}
vec1_t vec1_vec2(const vec2_t v){return vec1_pack(v.x);}
vec1_t vec1_vec3(const vec3_t v){return vec1_pack(v.x);}
vec1_t vec1_vec4(const vec4_t v){return vec1_pack(v.x);}

vec2_t vec2_vec1(const vec1_t v,const float y){return vec2_pack(v.x,y);}
vec2_t vec2_vec2(const vec2_t v){return vec2_pack(v.x,v.y);}
vec2_t vec2_vec3(const vec3_t v){return vec2_pack(v.x,v.y);}
vec2_t vec2_vec4(const vec4_t v){return vec2_pack(v.x,v.y);}

vec3_t vec3_vec1(const vec1_t v,const float y,const float z){return vec3_pack(v.x,y,z);}
vec3_t vec3_vec2(const vec2_t v,const float z){return vec3_pack(v.x,v.y,z);}
vec3_t vec3_vec3(const vec3_t v){return vec3_pack(v.x,v.y,v.z);}
vec3_t vec3_vec4(const vec4_t v){return vec3_pack(v.x,v.y,v.z);}

vec4_t vec4_vec1(const vec1_t v,const float y,const float z,const float w){return vec4_pack(v.x,y,z,w);}
vec4_t vec4_vec2(const vec2_t v,const float z,const float w){return vec4_pack(v.x,v.y,z,w);}
vec4_t vec4_vec3(const vec3_t v,const float w){return vec4_pack(v.x,v.y,v.z,w);}
vec4_t vec4_vec4(const vec4_t v){return vec4_pack(v.x,v.y,v.z,v.w);}

vec1_t vec1_add_scalar(const vec1_t v,const float s){float r[1];r[0]=v.x+s;return*(vec1_t*)r;}
vec2_t vec2_add_scalar(const vec2_t v,const float s){float r[2];r[0]=v.x+s;r[1]=v.y+s;return*(vec2_t*)r;}
vec3_t vec3_add_scalar(const vec3_t v,const float s){float r[3];r[0]=v.x+s;r[1]=v.y+s;r[2]=v.z+s;return*(vec3_t*)r;}
vec4_t vec4_add_scalar(const vec4_t v,const float s){float r[4];r[0]=v.x+s;r[1]=v.y+s;r[2]=v.z+s;r[3]=v.w+s;return*(vec4_t*)r;}

vec1_t scalar_add_vec1(const float s,const vec1_t v){float r[1];r[0]=s+v.x;return*(vec1_t*)r;}
vec2_t scalar_add_vec2(const float s,const vec2_t v){float r[2];r[0]=s+v.x;r[1]=s+v.y;return*(vec2_t*)r;}
vec3_t scalar_add_vec3(const float s,const vec3_t v){float r[3];r[0]=s+v.x;r[1]=s+v.y;r[2]=s+v.z;return*(vec3_t*)r;}
vec4_t scalar_add_vec4(const float s,const vec4_t v){float r[4];r[0]=s+v.x;r[1]=s+v.y;r[2]=s+v.z;r[3]=s+v.w;return*(vec4_t*)r;}

vec1_t vec1_mul_scalar(const vec1_t v,const float s){float r[1];r[0]=v.x*s;return*(vec1_t*)r;}
vec2_t vec2_mul_scalar(const vec2_t v,const float s){float r[2];r[0]=v.x*s;r[1]=v.y*s;return*(vec2_t*)r;}
vec3_t vec3_mul_scalar(const vec3_t v,const float s){float r[3];r[0]=v.x*s;r[1]=v.y*s;r[2]=v.z*s;return*(vec3_t*)r;}
vec4_t vec4_mul_scalar(const vec4_t v,const float s){float r[4];r[0]=v.x*s;r[1]=v.y*s;r[2]=v.z*s;r[3]=v.w*s;return*(vec4_t*)r;}

vec1_t scalar_mul_vec1(const float s,const vec1_t v){float r[1];r[0]=s*v.x;return*(vec1_t*)r;}
vec2_t scalar_mul_vec2(const float s,const vec2_t v){float r[2];r[0]=s*v.x;r[1]=s*v.y;return*(vec2_t*)r;}
vec3_t scalar_mul_vec3(const float s,const vec3_t v){float r[3];r[0]=s*v.x;r[1]=s*v.y;r[2]=s*v.z;return*(vec3_t*)r;}
vec4_t scalar_mul_vec4(const float s,const vec4_t v){float r[4];r[0]=s*v.x;r[1]=s*v.y;r[2]=s*v.z;r[3]=s*v.w;return*(vec4_t*)r;}

vec1_t vec1_add_vec1(const vec1_t a,const vec1_t b){register vec1_t r;r.x=a.x+b.x;return r;}
vec2_t vec2_add_vec2(const vec2_t a,const vec2_t b){register vec2_t r;r.x=a.x+b.x;r.y=a.y+b.y;return r;}
vec3_t vec3_add_vec3(const vec3_t a,const vec3_t b){register vec3_t r;r.x=a.x+b.x;r.y=a.y+b.y;r.z=a.z+b.z;return r;}
vec4_t vec4_add_vec4(const vec4_t a,const vec4_t b){register vec4_t r;r.x=a.x+b.x;r.y=a.y+b.y;r.z=a.z+b.z;r.w=a.w+b.w;return r;}

vec1_t vec1_sub_vec1(const vec1_t a,const vec1_t b){register vec1_t r;r.x=a.x-b.x;return r;}
vec2_t vec2_sub_vec2(const vec2_t a,const vec2_t b){register vec2_t r;r.x=a.x-b.x;r.y=a.y-b.y;return r;}
vec3_t vec3_sub_vec3(const vec3_t a,const vec3_t b){register vec3_t r;r.x=a.x-b.x;r.y=a.y-b.y;r.z=a.z-b.z;return r;}
vec4_t vec4_sub_vec4(const vec4_t a,const vec4_t b){register vec4_t r;r.x=a.x-b.x;r.y=a.y-b.y;r.z=a.z-b.z;r.w=a.w-b.w;return r;}

vec1_t vec1_mul_vec1(const vec1_t a,const vec1_t b){register vec1_t r;r.x=a.x*b.x;return r;}
vec2_t vec2_mul_vec2(const vec2_t a,const vec2_t b){register vec2_t r;r.x=a.x*b.x;r.y=a.y*b.y;return r;}
vec3_t vec3_mul_vec3(const vec3_t a,const vec3_t b){register vec3_t r;r.x=a.x*b.x;r.y=a.y*b.y;r.z=a.z*b.z;return r;}
vec4_t vec4_mul_vec4(const vec4_t a,const vec4_t b){register vec4_t r;r.x=a.x*b.x;r.y=a.y*b.y;r.z=a.z*b.z;r.w=a.w*b.w;return r;}

float vec1_mag(const vec1_t v){return sqrtf(v.x*v.x);}
float vec2_mag(const vec2_t v){return sqrtf(v.x*v.x+v.y*v.y);}
float vec3_mag(const vec3_t v){return sqrtf(v.x*v.x+v.y*v.y+v.z*v.z);}
float vec4_mag(const vec4_t v){return sqrtf(v.x*v.x+v.y*v.y+v.z*v.z+v.w*v.w);}

float vec1_dot(const vec1_t a,const vec1_t b){return a.x*b.x;}
float vec2_dot(const vec2_t a,const vec2_t b){return a.x*b.x+a.y*b.y;}
float vec3_dot(const vec3_t a,const vec3_t b){return a.x*b.x+a.y*b.y+a.z*b.z;}
float vec4_dot(const vec4_t a,const vec4_t b){return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w;}

vec1_t vec1_normalize(const vec1_t v)
{
	register vec1_t r;
	register float l=sqrtf(v.x*v.x);
	r.x=v.x/l;
	return r;
}
vec2_t vec2_normalize(const vec2_t v)
{
	register vec2_t r;
	register float l=sqrtf(v.x*v.x+v.y*v.y);
	r.x=v.x/l;
	r.y=v.y/l;
	return r;
}
vec3_t vec3_normalize(const vec3_t v)
{
	register vec3_t r;
	register float l=sqrtf(v.x*v.x+v.y*v.y+v.z*v.z);
	r.x=v.x/l;
	r.y=v.y/l;
	r.z=v.z/l;
	return r;
}
vec4_t vec4_normalize(const vec4_t v)
{
	register vec4_t r;
	register float l=sqrtf(v.x*v.x+v.y*v.y+v.z*v.z+v.w*v.w);
	r.x=v.x/l;
	r.y=v.y/l;
	r.z=v.z/l;
	r.w=v.w/l;
	return r;
}
vec1_t vec1_cross(const vec1_t a,const vec1_t b)
{
	register vec1_t r;
	r.x=a.x*0-0*b.x;
	return r;
}
vec2_t vec2_cross(const vec2_t a,const vec2_t b)
{
	register vec2_t r;
	r.x=a.x*b.x-a.y*b.y;
	r.y=a.x*b.y+a.y*b.x;
	return r;
}
vec3_t vec3_cross(const vec3_t a,const vec3_t b)
{
	register vec3_t r;
	r.x=a.y*b.z-b.y*a.z;
	r.y=a.z*b.x-b.z*a.x;
	r.z=a.x*b.y-b.x*a.y;
	return r;
}
vec4_t vec4_cross(const vec4_t a,const vec4_t b)
{
	register vec4_t r;
	r.x=a.w*b.x+a.x*b.w+a.y*b.z-a.z*b.y;
	r.y=a.w*b.y-a.x*b.z+a.y*b.w+a.z*b.x;
	r.z=a.w*b.z+a.x*b.y-a.y*b.x+a.z*b.w;
	r.w=a.w*b.w-a.x*b.x-a.y*b.y-a.z*b.z;
	return r;
}

int scalar_dump(const float s,int err)
{
	char buff[16];
	return write(err,buff,sprintf(buff,"%3.3f",s));
}
int vec1_dump(const vec1_t v,int err)
{
	char buff[16];
	return write(err,buff,sprintf(buff,"(%3.3f)",v.x));
}
int vec2_dump(const vec2_t v,int err)
{
	char buff[32];
	return write(err,buff,sprintf(buff,"(%3.3f,%3.3f)",v.x,v.y));
}
int vec3_dump(const vec3_t v,int err)
{
	char buff[48];
	return write(err,buff,sprintf(buff,"(%3.3f,%3.3f,%3.3f)",v.x,v.y,v.z));
}
int vec4_dump(const vec4_t v,int err)
{
	char buff[64];
	return write(err,buff,sprintf(buff,"(%3.3f,%3.3f,%3.3f,%3.3f)",v.x,v.y,v.z,v.w));
}
