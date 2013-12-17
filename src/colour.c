//
//  colour.c
//  P4
//
//  Created by Kota Nakano on 17/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//

#include <stdio.h>
#include<math.h>
#include "colour.h"
vec4_t hsv2rgb(const vec4_t arg)
{
	register const float	h=arg.x<0.0f?360.0f-fmod(-arg.x,360.0f):fmod(arg.x,360.0f);
	register const float	s=arg.y;
	register const float	v=arg.z;
	register const int		ti=(int)(h/60.0f);
	register const float	f=h/60.0-(float)ti;
	register const float	l=v*(1.0f-s);
	register const float	m=v*(1.0f-f*s);
	register const float	n=v*(1.0f-(1.0f-f)*s);
	register vec4_t r={0,0,0,arg.w};
	if(s)
	switch(ti)
	{
		case 0:r.r=v,r.g=n,r.b=l;break;
		case 1:r.r=m,r.g=v,r.b=l;break;
		case 2:r.r=l,r.g=v,r.b=n;break;
		case 3:r.r=l,r.g=m,r.b=v;break;
		case 4:r.r=n,r.g=l,r.b=v;break;
		case 5:r.r=v,r.g=l,r.b=m;break;
	}
	else r.r=v,r.g=v,r.z=v;
	return r;
}
vec4_t rgb2hsv(const vec4_t arg)
{
	register const float r=arg.x;
	register const float g=arg.y;
	register const float b=arg.z;
	register float M=0;//(r<g&&b<g)?g:(r<b&&g<b)?b:r;
	register float m=1;//(r>g&&b>g)?g:(r>b&&g>b)?b:r;
	register vec4_t v;
	if(M<r)M=r;if(M<g)M=g;if(M<b)M=b;
	if(m>r)m=r;if(m>g)m=g;if(m>b)m=b;
	v.a=arg.w;
	v.v=M;
	v.s=v.v?(M-m):0;
	v.h=v.s?
	M==r?0x00+0x3c*(g-b)/(M-m):
	M==g?0x78+0x3c*(b-r)/(M-m):
	M==b?0xf0+0x3c*(r-g)/(M-m):
	0:0;
	return v;
}
