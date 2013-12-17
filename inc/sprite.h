//
//  sprite.h
//  P4
//
//  Created by Kota Nakano on 16/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//

#ifndef P4_sprite_h
#define P4_sprite_h
#include"vector.h"
typedef enum
{
	sprite_type_triangles,
	sprite_type_triangle_strip,
	sprite_type_lines,
	sprite_type_line_strip,
}
sprite_type;
typedef struct
{
	unsigned int	buffer[2];
	unsigned int	length;
	void*			stride;
	struct
	{
		unsigned int offset;
		unsigned int stride;
	}
	vertex,normal,ambient,diffuse;
}
sprite_t;
int sprite_init(sprite_t*,int);
int sprite_bind(sprite_t*,int);
int sprite_edit(sprite_t*,int,int,void*,int);
int sprite_draw(sprite_t*,sprite_type,int);
int sprite_vertices(sprite_t*,float*,unsigned int,int);
int sprite_indices(sprite_t*,unsigned int*,unsigned int,int);
int sprite_term(sprite_t*,int);

int sprite_vertex(sprite_t*,unsigned int,unsigned int,int);
int sprite_normal(sprite_t*,unsigned int,unsigned int,int);
int sprite_ambient(sprite_t*,unsigned int,unsigned int,int);
int sprite_diffuse(sprite_t*,unsigned int,unsigned int,int);

typedef struct
{
	vec4_t	vertex;
	vec4_t	normal;
	vec4_t	ambient;
	vec4_t	diffuse;
}
vertex_t;
void vertex_auto_normal_triangle(vertex_t*,unsigned int);
void vertex_auto_normal_sphere(vertex_t*,unsigned int);

#endif
