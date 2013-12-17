//
//  matrix.h
//  P4
//
//  Created by Kota Nakano on 17/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//

#ifndef P4_matrix_h
#define P4_matrix_h
#include"vector.h"
typedef struct
{
	union
	{
		struct
		{
			vec1_t X;
		};
		struct
		{
			vec1_t C[0];
		};
		vec1_t	V[1];
		float	s[1][1];
	};
}mat1_t;
typedef struct
{
	union
	{
		struct
		{
			vec2_t X;
			vec2_t Y;
		};
		vec2_t	V[2];
		float	s[2][2];
	};
}mat2_t;
typedef struct
{
	union
	{
		struct
		{
			vec3_t X;
			vec3_t Y;
			vec3_t Z;
		};
		vec3_t	V[3];
		float	s[3][3];
	};
}mat3_t;
typedef struct
{
	union
	{
		struct
		{
			vec4_t X;
			vec4_t Y;
			vec4_t Z;
			vec4_t W;
		};
		vec4_t	V[4];
		float	s[4][4];
	};
}mat4_t;
#endif
