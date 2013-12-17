//
//  head.c
//  P4
//
//  Created by Kota Nakano on 16/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//
#include<stdio.h>
#include<stdlib.h>
#include"shader.h"
#include"gl.h"
#include"hd.h"
#include"bill.h"
#include"mesh.h"
#include"vector.h"
#include"projet.h"
#include"colour.h"
#include"string.h"
#include<string.h>
#include<math.h>
const rate=4.0;
int hd_init(hd_t*self,unsigned int name,short w,short h,const char*face_path,const char*head_path,int dump)
{
	register int result=0;
	register int index=0;

	if(dump)
	{
		char buf[1024];
		result+=write(dump,buf,sprintf(buf,"HD INIT: BEGIN WITH %d\n",glGetError()));
	}

	self->flag=hd_DEPTH_MODE;
	self->name=name;
	self->root[0]=w;
	self->root[1]=h;
	
	self->view[0]=0;
	self->view[1]=0;
	self->view[2]=1;
	self->view[3]=1;

	result+=sprite_bill_init(&self->bill,dump);
	result+=sprite_mesh_init(&self->head,head_path,&self->xrange.min,&self->xrange.max,0,0,&self->zrange.min,&self->zrange.max,dump);
	result+=sprite_mesh_init(&self->face,face_path,&self->xrange.min,&self->xrange.max,0,0,&self->zrange.min,&self->zrange.max,dump);
	
	self->texture.w=32;
	self->texture.h=32;
	self->texture.cache=malloc(self->texture.w*self->texture.h*4*sizeof(float));
	self->texture.engine=1;
	
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1,&self->texture.buffer);
	glBindTexture(GL_TEXTURE_2D,self->texture.buffer);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,self->texture.w,self->texture.h,0,GL_RGBA,GL_FLOAT,self->texture.cache);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	

	self->light.from[0]= 5;
	self->light.from[1]= 10;
	self->light.from[2]= 10;
	self->light.from[3]=1;
	
	self->camera.from[0]=0;
	self->camera.from[1]=0;
	self->camera.from[2]=1;
	self->camera.from[3]=0;

	self->camera.to[0]=0;
	self->camera.to[1]=0;
	self->camera.to[2]=0;
	self->camera.to[3]=1;

	self->pers[0][0]=1;
	self->pers[1][1]=1;
	self->pers[2][2]=1;
	self->pers[3][3]=1;

	self->proj[0][0]=1;
	self->proj[1][1]=1;
	self->proj[2][2]=1;
	self->proj[3][3]=1;
	
	self->camera.from[0]=  0;self->camera.to[0]=0;self->camera.up[0]=0;
	self->camera.from[1]=1.2;self->camera.to[1]=0;self->camera.up[1]=1;
	self->camera.from[2]=1.2;self->camera.to[2]=0;self->camera.up[2]=0;
	self->camera.from[3]=1.0;self->camera.to[3]=1;

	self->light.from[0]= 0;
	self->light.from[1]=10;
	self->light.from[2]= 0;
	self->light.from[3]= 1;

	result+=hd_ortho(self,-4.8,4.8,-2.7, 2.7,-3.6, 3.6,dump);
	result+=hd_lookat(self,
			self->camera.from[0],self->camera.from[1],self->camera.from[2],
			self->camera.to[0],self->camera.to[1],self->camera.to[2],
			self->camera.up[0],self->camera.up[1],self->camera.up[2],
			dump);
	self->eeg.gain=-15;
	self->eeg.bias=240;
	
	if(dump)
	{
		char buf[1024];
		result+=write(dump,buf,sprintf(buf,"HD INIT: END WITH %d\n",glGetError()));
	}
	
	return result;
}
int hd_draw(hd_t*self,int dump)
{
	glViewport(self->root[0]*self->view[0],self->root[1]*self->view[1],self->root[0]*self->view[2],self->root[1]*self->view[3]);
	{
		float E[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
		
		glStencilMask(0);

		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_TEXTURE_2D);
		
		glUniformMatrix4fv(shader_perspective,1,0,*E);
		glUniformMatrix4fv(shader_projection,1,0,*E);
		glUniformMatrix4fv(shader_modelview,1,0,*E);
		glUniformMatrix4fv(shader_reshape,1,0,*E);
			
		glUniform4fv(shader_ambient_const,1,self->back);
		glUniform4fv(shader_diffuse_const,1,self->back);
			
		glBindBuffer(GL_ARRAY_BUFFER,self->bill.buffer[0]);
		glVertexAttribPointer(shader_vertex,4,GL_FLOAT,0,self->bill.vertex.stride,(GLvoid*)self->bill.vertex.offset);
		glVertexAttribPointer(shader_normal,4,GL_FLOAT,0,self->bill.normal.stride,(GLvoid*)self->bill.normal.offset);
		glVertexAttribPointer(shader_ambient,4,GL_FLOAT,0,self->bill.ambient.stride,(GLvoid*)self->bill.ambient.offset);
		glVertexAttribPointer(shader_diffuse,4,GL_FLOAT,0,self->bill.diffuse.stride,(GLvoid*)self->bill.diffuse.offset);
			
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->bill.buffer[1]);
		glDrawElements(GL_TRIANGLES,self->bill.length,(int)self->bill.stride,0);
	}
	glClear(self->flag&hd_DEPTH_CLEAR?GL_DEPTH_BUFFER_BIT:0);
	(self->flag&hd_DEPTH_MODE?glEnable:glDisable)(GL_DEPTH_TEST);
	(self->flag&hd_BLEND_MODE?glEnable:glDisable)(GL_BLEND);
	glStencilMask(self->name);
	{
		glUniform1f(shader_fog_level,self->fog);
		glUniform4fv(shader_light_position,1,self->light.from);
		glUniform4fv(shader_camera_position,1,self->camera.from);
		glUniformMatrix4fv(shader_projection,1,0,*self->proj);
		glUniformMatrix4fv(shader_perspective,1,0,*self->pers);
	}
	{
		float e[4]={1,1,1,1};
		float z[4]={0,0,0,0};
				
		glUniform4fv(shader_ambient_const,1,z);
		glUniform4fv(shader_diffuse_const,1,z);

		if(1)
		{
			glActiveTexture(GL_TEXTURE0+self->texture.engine);
			glBindTexture(GL_TEXTURE_2D,self->texture.buffer);
			glTexSubImage2D(GL_TEXTURE_2D,0,0,0,self->texture.w,self->texture.h,GL_RGBA,GL_FLOAT,self->texture.cache);
			glUniform1i(shader_diffuse_drive,1);
			glUniform1i(shader_diffuse_image,self->texture.engine);
			float xrange=(self->xrange.max-self->xrange.min);
			float zrange=(self->zrange.max-self->zrange.min);
			float M[4][4]=
			{
				{1/xrange,0.0,0.0,-self->xrange.min/xrange},
				{0.0,0.0,1/zrange,-self->zrange.min/zrange},
				{0.0,0.0,0.0,0.0},
				{0.0,0.0,0.0,1.0},
			};
			glUniformMatrix4fv(shader_diffuse_coord,1,1,*M);
		}	
		glBindBuffer(GL_ARRAY_BUFFER,self->head.buffer[0]);
		glVertexAttribPointer(shader_vertex,4,GL_FLOAT,0,self->head.vertex.stride,(GLvoid*)self->head.vertex.offset);
		glVertexAttribPointer(shader_normal,4,GL_FLOAT,0,self->head.normal.stride,(GLvoid*)self->head.normal.offset);
		glVertexAttribPointer(shader_ambient,4,GL_FLOAT,0,self->head.ambient.stride,(GLvoid*)self->head.ambient.offset);
		glVertexAttribPointer(shader_diffuse,4,GL_FLOAT,0,self->head.diffuse.stride,(GLvoid*)self->head.diffuse.offset);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->head.buffer[1]);
		glDrawElements(GL_TRIANGLES,self->head.length,(GLsizei)self->head.stride,0);
		
		glUniform1i(shader_ambient_drive,0);
		glUniform4fv(shader_ambient_const,1,e);
		
		glUniform1i(shader_diffuse_drive,0);
		glUniform4fv(shader_diffuse_const,1,e);
		
		glDisable(GL_TEXTURE_2D);
		glBindBuffer(GL_ARRAY_BUFFER,self->face.buffer[0]);
		glVertexAttribPointer(shader_vertex,4,GL_FLOAT,0,self->face.vertex.stride,(GLvoid*)self->face.vertex.offset);
		glVertexAttribPointer(shader_normal,4,GL_FLOAT,0,self->face.normal.stride,(GLvoid*)self->face.normal.offset);
		glVertexAttribPointer(shader_ambient,4,GL_FLOAT,0,self->face.ambient.stride,(GLvoid*)self->face.ambient.offset);
		glVertexAttribPointer(shader_diffuse,4,GL_FLOAT,0,self->face.diffuse.stride,(GLvoid*)self->face.diffuse.offset);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->face.buffer[1]);
		glDrawElements(GL_TRIANGLES,self->face.length,(GLsizei)self->face.stride,0);


	}

	return 0;
}
int hd_update(hd_t*self,int dump)
{
	++self->frame;
	return 0;
}
int hd_light(hd_t*self,float x,float y,float z,int dump)
{
	self->light.from[0]=x;
	self->light.from[1]=y;
	self->light.from[2]=z;
	self->light.from[3]=1;
	return 0;
}
int hd_view(hd_t*self,float x,float y,float w,float h,int dump)
{
	register int result=0;
	self->view[0]=x;
	self->view[1]=y;
	self->view[2]=w;
	self->view[3]=h;
	register float RW=self->root[0]*self->view[2];
	register float RH=self->root[1]*self->view[3];
	if(RW==RH)	result+=hd_ortho(self,-rate, rate,-rate, rate,-rate, rate,dump);
	else if(RW<RH)	result+=hd_ortho(self,-rate*sqrtf(RH/RW), rate*sqrtf(RH/RW),-rate*sqrtf(RW/RH), rate*sqrtf(RW/RH),-rate, rate,dump);
	else if(RH<RW)	result+=hd_ortho(self,-rate*sqrtf(RW/RH), rate*sqrtf(RW/RH),-rate*sqrtf(RH/RW), rate*sqrtf(RH/RW),-rate, rate,dump);
	return result;
}
int hd_root(hd_t*self,short w,short h,int dump)
{
	register int result=0;
	self->root[0]=w;
	self->root[1]=h;
	register float RW=self->root[0]*self->view[2];
	register float RH=self->root[1]*self->view[3];
	if(RW==RH)	result+=hd_ortho(self,-rate, rate,-rate, rate,-rate, rate,dump);
	else if(RW<RH)	result+=hd_ortho(self,-rate*sqrtf(RH/RW), rate*sqrtf(RH/RW),-rate*sqrtf(RW/RH), rate*sqrtf(RW/RH),-rate, rate,dump);
	else if(RH<RW)	result+=hd_ortho(self,-rate*sqrtf(RW/RH), rate*sqrtf(RW/RH),-rate*sqrtf(RH/RW), rate*sqrtf(RH/RW),-rate, rate,dump);
	return result;
}
int hd_back(hd_t*self,float r,float g,float b,float a,int dump)
{
	register int result=0;
	self->back[0]=r;
	self->back[1]=g;
	self->back[2]=b;
	self->back[3]=a;
	return result;
}
int hd_ortho(hd_t*self,float l,float r,float b,float t,float zmin,float zmax,int dump)
{
	*(mat4_t*)self->proj=mat4_ortho(l,r,b,t,zmin,zmax);
	return 0;
}
int hd_lookat(hd_t*self,float ex,float ey,float ez,float ax,float ay,float az,float ux,float uy,float uz,int dump)
{
	register vec3_t e={ex,ey,ez};
	register vec3_t l={ax,ay,az};
	register vec3_t u={ux,uy,uz};
	register vec3_t z=vec3_normalize(vec3_sub_vec3(e,l));
	register vec3_t x=vec3_normalize(vec3_cross(u,z));
	register vec3_t y=vec3_normalize(vec3_cross(z,x));
	float M[4][4]=
	{
		{x.x,				y.x,				z.x,			0},
		{x.y,				y.y,				z.y,			0},
		{x.z,				y.z,				z.z,			0},
		{-vec3_dot(x,e),	-vec3_dot(y,e),		-vec3_dot(z,e),	1}
	};
	*((mat4_t*)self->pers)=*((mat4_t*)*M);
	return 0;
}
int hd_eeg_dict(hd_t*self,const char*file,int dump)
{
	unsigned int result=0;
	const char mess[]="Invalid data format: ";
	const char beg='[';
	const char end=']';
	const char del=' ';
	FILE*fp=fopen(file,"rb");
	if(fp)
	{
		char buff[16384]={0};
		fread(buff,1,sizeof(buff),fp);
		{
			char*token[1024]={0};
			if(0<string_split_block_unsafe(buff,beg,end,token))
			{
				register unsigned int k,K;
				char*name[256]={0};
				char*head[256]={0};
				if(0<string_split_block_unsafe(token[0],beg,end,head))
				for(k=0,K=(self->eeg.dictLength=string_split_unsafe(head[0],del,name));k<K;++k)
				{
					register unsigned int m,M;
					char*point[256]={0};
					if(token[k+1])
					for(m=0,M=string_split_block_unsafe(token[k+1],beg,end,point);m<M;++m)
					{
						char*coord[64]={0};
						if(0<string_split_unsafe(point[m],del,coord))
						{
							string_unquote(name[k],self->eeg.dict[k].name);	
							self->eeg.dict[k].x=coord[0]?atof(coord[0]):0;
							self->eeg.dict[k].y=coord[1]?atof(coord[1]):0;
							self->eeg.dict[k].z=coord[2]?atof(coord[2]):0;
						}
						else if(dump)result+=write(dump,mess,sizeof(mess));
					}
					else if(dump)result+=write(dump,mess,sizeof(mess));
				}
				else if(dump)result+=write(dump,mess,sizeof(mess));
			}
			else if(dump)result+=write(dump,mess,sizeof(mess));
		}
		fclose(fp);
	}
	return result;
}
int hd_eeg_order(hd_t*self,const char*refs,int dump)
{
	register unsigned k,K,m,M;
	char*buff=strcpy(malloc(strlen(refs)),refs);
	char*token[256]={0};
	self->eeg.itemLength=0;
	for(k=0,K=string_split_unsafe(buff,' ',token);k<K;++k)
		for(m=0,M=self->eeg.dictLength;m<M;++m)
			if(!strcmp(token[k],self->eeg.dict[m].name))
				self->eeg.order[self->eeg.itemLength++]=m;
	free(buff);
	return 0;
}
