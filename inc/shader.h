//
//  shader.h
//  P4
//
//  Created by Kota Nakano on 16/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//

#ifndef P4_shader_h
#define P4_shader_h
typedef enum
{
	shader_type_vertex,
	shader_type_geometry,
	shader_type_fragment,
}
shader_type_t;
const char*shader_source_vertex_default;
const char*shader_source_fragment_default;
typedef struct
{
	unsigned int	program;
	unsigned int	shaders[3];
}
shader_t;
int shader_init(shader_t*,int dump);
int shader_load(shader_t*,shader_type_t,const char*,size_t,int dump);
int shader_link(shader_t*,int dump);
int shader_bind(const shader_t*,int dump);
int shader_getAttribLoc(const shader_t*,const char*,int dump);
int shader_getUniformLoc(const shader_t*,const char*,int dump);
int shader_term(shader_t*,int dump);

extern unsigned int shader_projection;
extern unsigned int shader_perspective;
extern unsigned int shader_modelview;
extern unsigned int shader_reshape;

extern unsigned int shader_light_position;
extern unsigned int shader_camera_position;

extern unsigned int shader_ambient_const;
extern unsigned int shader_ambient_drive;
extern unsigned int shader_ambient_image;
extern unsigned int shader_ambient_coord;

extern unsigned int shader_diffuse_const;
extern unsigned int shader_diffuse_drive;
extern unsigned int shader_diffuse_image;
extern unsigned int shader_diffuse_coord;

extern unsigned int shader_vertex;
extern unsigned int shader_normal;
extern unsigned int shader_ambient;
extern unsigned int shader_diffuse;

#endif
