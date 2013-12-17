//
//  shader.c
//  P4
//
//  Created by Kota Nakano on 16/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//
#include<stdio.h>
#include<string.h>
#include"gl.h"
#include"shader.h"
int shader_init(shader_t*self,int err)
{
	self->program=glCreateProgram();
	return glGetError();
}
int shader_load(shader_t*self,shader_type_t type,const char*text,size_t length,int dump)
{
	int result;
	unsigned int shader=glCreateShader
	(
		type==shader_type_vertex?GL_VERTEX_SHADER:
		type==shader_type_geometry?GL_GEOMETRY_SHADER_EXT:
		type==shader_type_fragment?GL_FRAGMENT_SHADER:
		0
	);
	if(!(result=glGetError()))
	{
		if(!length)length=strlen(text);
		glShaderSource(shader,1,&text,&length);
		glCompileShader(shader);
		glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&result);
		if(1<result)
		{
			char*log=(char*)malloc(result);
			glGetShaderInfoLog(shader,result,&result,log);
			if(0<dump)result=write(dump,
								type==shader_type_vertex?"VSH:":
								type==shader_type_geometry?"GSH:":
								type==shader_type_fragment?"FSH:":
								"NOR:",4)+write(dump,log,result);
			free(log);
		}
		else glAttachShader(self->program,shader);
	}
	return result;
}
int shader_link(shader_t*self,int dump)
{
	register char buff[16];
	int result=~0;
	glLinkProgram(self->program);
	glValidateProgram(self->program);
	glGetProgramiv(self->program,GL_INFO_LOG_LENGTH,&result);
	if(result)
	{
		char*log=(char*)malloc(result);
		glGetProgramInfoLog(self->program,result,&result,log);
		if(dump)result=write(dump,log,result);
		free(log);
	}
	return result;
}
int shader_bind(const shader_t*self,int dump)
{
	char buff[16];
	register int result=-1;
	glUseProgram(self->program);
	if((result=glGetError()))
		glUseProgram(0);
	return 0<dump?write(dump,buff,sprintf(buff,"%x",result)):result;
}
int shader_getAttribLoc(const shader_t*self,const char*name,int dump)
{
	return glGetAttribLocation(self->program,name);
}
int shader_getUniformLoc(const shader_t*self,const char*name,int dump)
{
	if(0<dump)
	{
		char buff[64];
		write(dump,buff,sprintf(buff,"%s:%d\n",name,glGetUniformLocation(self->program,name)));
	}
	return glGetUniformLocation(self->program,name);
}
int shader_term(shader_t*self,int err)
{
	glUseProgram(0);
	if(self->program)glDeleteProgram(self->program);
	return 0;
}
 unsigned int shader_projection;
 unsigned int shader_perspective;
 unsigned int shader_modelview;
 unsigned int shader_reshape;

 unsigned int shader_fog_level;
 unsigned int shader_light_position;
 unsigned int shader_camera_position;

 unsigned int shader_ambient_const;
 unsigned int shader_ambient_drive;
 unsigned int shader_ambient_image;
 unsigned int shader_ambient_coord;

 unsigned int shader_diffuse_const;
 unsigned int shader_diffuse_drive;
 unsigned int shader_diffuse_image;
 unsigned int shader_diffuse_coord;

 unsigned int shader_vertex;
 unsigned int shader_normal;
 unsigned int shader_ambient;
 unsigned int shader_diffuse;
const char*shader_source_vertex_default="		\n\
uniform mat4 uProjection;						\n\
uniform mat4 uPerspective;						\n\
uniform mat4 uModelview;						\n\
uniform mat4 uReshape;							\n\
\n\
attribute vec4	aVertex;						\n\
attribute vec4	aNormal;						\n\
attribute vec4	aDiffuse;						\n\
attribute vec4	aAmbient;						\n\
\n\
varying	vec4	vVertex;						\n\
varying	vec4	vNormal;						\n\
varying	vec4	vDiffuse;						\n\
varying	vec4	vAmbient;						\n\
\n\
void main()											\n\
{													\n\
	vVertex=uModelview*uReshape*aVertex;			\n\
	vNormal=uReshape*aNormal;						\n\
	gl_Position=uProjection*uPerspective*vVertex;	\n\
	vAmbient=aAmbient;								\n\
	vDiffuse=aDiffuse;								\n\
}													\n\
";
const char*shader_source_fragment_default="	\n\
uniform vec4	uLight;													\n\
uniform	vec4	uCamera;												\n\
\n\
uniform int			sAmbient;											\n\
uniform	vec4		uAmbient;											\n\
uniform mat4		mAmbient;											\n\
uniform	sampler2D	iAmbient;											\n\
\n\
uniform int			sDiffuse;											\n\
uniform	vec4		uDiffuse;											\n\
uniform mat4		mDiffuse;											\n\
uniform	sampler2D	iDiffuse;											\n\
\n\
varying	vec4	vVertex;												\n\
varying	vec4	vNormal;												\n\
varying	vec4	vAmbient;												\n\
varying	vec4	vDiffuse;												\n\
\n\
void main()																\n\
{																		\n\
	vec3	dst=vVertex.xyz-uCamera.xyz;									\n\
	vec4	ambient=vAmbient*(0<sAmbient?texture2D(iAmbient,(mAmbient*vVertex).xy):uAmbient);	\n\
	vec4	diffuse=vDiffuse*(0<sDiffuse?texture2D(iDiffuse,(mDiffuse*vVertex).xy):uDiffuse);	\n\
	float	np=(dot(normalize(vNormal.xyz),normalize(uLight.xyz-vVertex.xyz)));		\n\
	float	vp=length(vVertex.xyz-uCamera.xyz);									\n\
	gl_FragColor=(ambient+np*diffuse);									\n\
	gl_FragColor.a=gl_FragColor.a/exp(vp*0.5);\n\
}																		\n\
";
