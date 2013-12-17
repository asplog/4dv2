//
//  projet.c
//  P4
//
//  Created by Kota Nakano on 18/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//
#include<math.h>
#include"projet.h"
mat4_t mat4_ortho(float l,float r,float b,float t,float n,float f)
{
	float M[4][4]=
	{
		{2/(r-l),0,0,0},
		{0,2/(t-b),0,0},
		{0,0,-2/(f-n),0},
		{-(r+l)/(r-l),-(t+b)/(t-b),(f+n)/(f-n),1}
	};
	return*(mat4_t*)M;
}
mat4_t mat4_frustum(float l,float r,float b,float t,float n,float f)
{
	float M[4][4]=
	{
		{2*n/(r-l),		0,				0,				 0},
		{0,				2*n/(t-b),		0,				 0},
		{0,				0,				-(f+n)/(f-n),	-1},
		{(r+l)/(r-l),	(t+b)/(t-b),	-2*(f*n)/(f-n),	 0}
	};
	return*(mat4_t*)M;
}
mat4_t mat4_perspective(float fovy,float aspect,float n,float f)
{
	register float v=n*tanf(M_PI*fovy/360.0f);
	return mat4_frustum(-v*aspect, v*aspect,-v, v,n,f);
}
mat4_t mat4_look(float ex,float ey,float ez,float ax,float ay,float az,float ux,float uy,float uz)
{
	register vec3_t e={ex,ey,ez};
	register vec3_t l={0,0,0};
	register vec3_t u={0,1,0};
	register vec3_t z=vec3_normalize(vec3_sub_vec3(e,l));
	register vec3_t x=vec3_normalize(vec3_cross(u,z));
	register vec3_t y=vec3_normalize(vec3_cross(z,x));
	float M[4][4]=
	{
		{x.x,	y.x,	z.x,	0},
		{x.y,	y.y,	z.y,	0},
		{x.z,	y.z,	z.z,	0},
		{-vec3_dot(x,e),	-vec3_dot(y,e),	-vec3_dot(z,e),	1}
	};
	return*(mat4_t*)*M;
}
