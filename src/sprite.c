//
//  sprite.c
//  P4
//
//  Created by Kota Nakano on 16/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//

#include <string.h>
#include<stdio.h>
#include"gl.h"
#include"shader.h"
#include"sprite.h"
void sprite_default(sprite_t*self,int dump)
{
	glEnableVertexAttribArray(shader_vertex);
	glVertexAttribPointer(shader_vertex,4,GL_FLOAT,0,self->vertex.stride,(GLvoid*)(self->vertex.offset));
	
	glEnableVertexAttribArray(shader_normal);
	glVertexAttribPointer(shader_normal,4,GL_FLOAT,0,self->normal.stride,(GLvoid*)(self->normal.offset));
	
	glEnableVertexAttribArray(shader_ambient);
	glVertexAttribPointer(shader_ambient,4,GL_FLOAT,0,self->ambient.stride,(GLvoid*)(self->ambient.offset));

	glEnableVertexAttribArray(shader_diffuse);
	glVertexAttribPointer(shader_diffuse,4,GL_FLOAT,0,self->diffuse.stride,(GLvoid*)(self->diffuse.offset));
}
int sprite_init(sprite_t*self,int dump)
{
	int result=0;
	bzero(self,sizeof(sprite_t));
	
	glGenBuffers(2,self->buffer);
	return result;
}
int sprite_bind(sprite_t*self,int dump)
{
	glBindBuffer(GL_ARRAY_BUFFER,self->buffer[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->buffer[1]);
	return 0;
}
int sprite_edit(sprite_t*self,int offset,int length,void*buffer,int dump)
{
	glBufferSubData(GL_ARRAY_BUFFER,offset,length,buffer);
	return glGetError();
}
int sprite_draw(sprite_t*self,sprite_type type,int dump)
{
	glBindBuffer(GL_ELEMENT_ARRAY_APPLE,self->buffer[1]);
	glDrawElements
	(
		type==sprite_type_triangles?GL_TRIANGLES:
		type==sprite_type_triangle_strip?GL_TRIANGLE_STRIP:
		type==sprite_type_lines?GL_LINES:
		type==sprite_type_line_strip?GL_TRIANGLE_STRIP:
		0,
		self->length,GL_UNSIGNED_INT,0
	);
	return glGetError();
}
int sprite_vertices(sprite_t*self,float*buff,unsigned int size,int dump)
{
	glBindBuffer(GL_ARRAY_BUFFER,self->buffer[0]);
	glBufferData(GL_ARRAY_BUFFER,size,buff,GL_DYNAMIC_DRAW);
	return glGetError();
}
int sprite_indices(sprite_t*self,unsigned int*buff,unsigned int size,int dump)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->buffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,buff,GL_STATIC_DRAW);
	self->length=size/sizeof(unsigned int);
	return glGetError();
}
int sprite_vertex(sprite_t*self,unsigned int stride,unsigned int offset,int dump)
{
	self->vertex.stride=stride;
	self->vertex.offset=offset;
	return 0;
}
int sprite_normal(sprite_t*self,unsigned int stride,unsigned int offset,int dump)
{
	self->normal.stride=stride;
	self->normal.offset=offset;
	return 0;
}
int sprite_ambient(sprite_t*self,unsigned int stride,unsigned int offset,int dump)
{
	self->ambient.stride=stride;
	self->ambient.offset=offset;
	return 0;
}
int sprite_diffuse(sprite_t*self,unsigned int stride,unsigned int offset,int dump)
{
	self->diffuse.stride=stride;
	self->diffuse.offset=offset;
	return 0;
}
int sprite_term(sprite_t*self,int dump)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glDeleteBuffers(2,self->buffer);
	return glGetError();
}
void vertex_auto_normal_triangle(vertex_t*vertex,unsigned int length)
{
	register unsigned int index;
	for(index=0;index<length;index+=3)
	{
		vec4_t normal={0,0,0,1};
		*((vec3_t*)&normal)=
		vec3_cross(
				   vec3_sub_vec3(*(vec3_t*)&vertex[index+2].vertex,*(vec3_t*)&vertex[index+1].vertex),
				   vec3_sub_vec3(*(vec3_t*)&vertex[index+1].vertex,*(vec3_t*)&vertex[index+0].vertex)
				   );
		vertex[index+0].normal=normal;
		vertex[index+1].normal=normal;
		vertex[index+2].normal=normal;
	}
}
void vertex_auto_normal_sphere(vertex_t*vertex,unsigned int length)
{
	register unsigned int index;
	for(index=0;index<length;++index)
		vertex[index].normal=vec4_vec3(vec3_normalize(*(vec3_t*)&vertex[index].vertex),1);
}
