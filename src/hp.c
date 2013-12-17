//
//  head.c
//  P4
//
//  Created by Kota Nakano on 16/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"shader.h"
#include"gl.h"
#include"hp.h"
#include"lcd.h"
#include"bill.h"
#include"vector.h"
#include"projet.h"
#include"colour.h"
#include"string.h"
#include"sphere.h"
#include"cylinder.h"
#include<string.h>
#include<strings.h>
#include<math.h>
#include<memory.h>
int hp_init(hp_t*self,unsigned int name,short w,short h,int dump)
{
	register unsigned int k,K;
	register int result=0;
	register int index=0;
	bzero(self,sizeof(self));
	if(dump)
	{
		char buf[1024];
		result+=write(dump,buf,sprintf(buf,"HP INIT: BEGN WITH %d\n",glGetError()));
	}

	self->flag=hp_BLEND_MODE;
	self->name=name;

	self->root[0]=w;
	self->root[1]=h;
	
	self->view[0]=0;
	self->view[1]=0;
	self->view[2]=1;
	self->view[3]=1;

	self->light.from[0]=0;
	self->light.from[1]=4;
	self->light.from[2]=0;
	self->light.from[3]=1;
	
	self->light.to[0]=0;
	self->light.to[1]=0;
	self->light.to[2]=0;
	self->light.to[3]=1;

	self->pinhole.matrix[0][0]=1;
	self->pinhole.matrix[1][1]=1;
	self->pinhole.matrix[2][2]=1;
	self->pinhole.matrix[3][3]=1;

	self->project.matrix[0][0]=1;
	self->project.matrix[1][1]=1;
	self->project.matrix[2][2]=1;
	self->project.matrix[3][3]=1;

	result+=sprite_bill_init(&self->bill,dump);
	result+=sprite_cylinder_init_x(self->cylinder+0,-1<<16, 1<<16,1.0f,24,dump);
	result+=sprite_cylinder_init_y(self->cylinder+1,-1<<16, 1<<16,1.0f,24,dump);
	result+=sprite_cylinder_init_z(self->cylinder+2,-1<<16, 1<<16,1.0f,24,dump);
	result+=sprite_sphere_init(&self->sphere,dump);
	result+=sprite_lcd_init(&self->string,dump);

	for(k=0,K=3;k<K;++k)
	{
		self->value[k].decay=0.7f;
		self->value[k].accel=0.3f;
		self->value[k].shift=0.1f;
		self->value[k].max= 3.0f;
		self->value[k].min=-3.0f;
	}
	
	self->object[0].ambient[0]=0;
	self->object[0].ambient[1]=0;
	self->object[0].ambient[2]=0;
	self->object[0].ambient[3]=1;
	
	self->object[0].diffuse[0]=0.7f;
	self->object[0].diffuse[1]=0.8f;
	self->object[0].diffuse[2]=0.9f;
	self->object[0].diffuse[3]=0;
	
	self->object[0].scale=0.5f;

	for(k=0,K=3;k<K;++k)
	{
		self->axis[k].ambient[0]=0.8;
		self->axis[k].ambient[1]=0.8;
		self->axis[k].ambient[2]=0.8;
		self->axis[k].ambient[3]=1.0;
		
		self->axis[k].diffuse[0]=0.0;
		self->axis[k].diffuse[1]=0.0;
		self->axis[k].diffuse[2]=0.0;
		self->axis[k].diffuse[3]=0.0;
	
		self->axis[k].width=0.1f;
	}

	for(k=0,K=3;k<K;++k)
	{
		self->label[k].label[0]='X'+k;
		self->label[k].label[1]='-';
		self->label[k].label[2]='A';
		self->label[k].label[3]='X';
		self->label[k].label[4]='I';
		self->label[k].label[5]='S';
		self->label[k].label[6]='\0';

		self->label[k].scale=0.5f;
		
		self->label[k].n.ambient[0]=0;self->label[k].n.diffuse[0]=0;
		self->label[k].n.ambient[1]=0;self->label[k].n.diffuse[1]=0;
		self->label[k].n.ambient[2]=0;self->label[k].n.diffuse[2]=0;
		self->label[k].n.ambient[3]=0;self->label[k].n.diffuse[3]=0;
		
		self->label[k].p.ambient[0]=1;self->label[k].p.diffuse[0]=1;
		self->label[k].p.ambient[1]=1;self->label[k].p.diffuse[1]=1;
		self->label[k].p.ambient[2]=1;self->label[k].p.diffuse[2]=1;
		self->label[k].p.ambient[3]=1;self->label[k].p.diffuse[3]=1;
	}
	
	if(dump)
	{
		char buf[1024];
		result+=write(dump,buf,sprintf(buf,"HP INIT: END WITH %d\n",glGetError()));
	}
	hp_reshape(self,dump);
	return result;
}
int hp_draw(hp_t*self,int dump)
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
	glClear(self->flag&hp_DEPTH_CLEAR?GL_DEPTH_BUFFER_BIT:0);
	(self->flag&hp_DEPTH_MODE?glEnable:glDisable)(GL_DEPTH_TEST);
	(self->flag&hp_BLEND_MODE?glEnable:glDisable)(GL_BLEND);
	glStencilMask(self->name);
	{
		glUniform1f(shader_fog_level,self->fog);
		glUniform4fv(shader_light_position,1,self->light.from);
		glUniform4fv(shader_camera_position,1,self->camera.from);
		glUniformMatrix4fv(shader_projection,1,0,*self->project.matrix);
		glUniformMatrix4fv(shader_perspective,1,0,*self->pinhole.matrix);
	}
	{
		register unsigned int k,K;
		
		const float M[4][4]=
		{
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,0},
			{0,0,0,1},
		};
		glUniformMatrix4fv(shader_modelview,1,0,*M);
		
		for(k=0,K=3;k<K;++k)
		{
			const float R[4][4]=
			{
				{self->axis[k].width,0,0,0},
				{0,self->axis[k].width,0,0},
				{0,0,self->axis[k].width,0},
				{0,0,0,1},
			};
			glUniformMatrix4fv(shader_reshape,1,0,*R);

			glUniform4fv(shader_ambient_const,1,self->axis[k].ambient);
			glUniform4fv(shader_diffuse_const,1,self->axis[k].diffuse);

			glBindBuffer(GL_ARRAY_BUFFER,self->cylinder[k].buffer[0]);
			glVertexAttribPointer(shader_vertex,4,GL_FLOAT,0,self->cylinder[k].vertex.stride,(GLvoid*)self->cylinder[k].vertex.offset);
			glVertexAttribPointer(shader_normal,4,GL_FLOAT,0,self->cylinder[k].normal.stride,(GLvoid*)self->cylinder[k].normal.offset);
			glVertexAttribPointer(shader_ambient,4,GL_FLOAT,0,self->cylinder[k].ambient.stride,(GLvoid*)self->cylinder[k].ambient.offset);
			glVertexAttribPointer(shader_diffuse,4,GL_FLOAT,0,self->cylinder[k].diffuse.stride,(GLvoid*)self->cylinder[k].diffuse.offset);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->cylinder[k].buffer[1]);
			glDrawElements(GL_TRIANGLES,self->cylinder[k].length,(GLsizei)self->cylinder[k].stride,0);
		}
	}
	{
		register unsigned int k,K;
				
		const float M[4][4]=
		{
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,0},
			{self->value[0].cur,self->value[1].cur,self->value[2].cur,1},
		};
		
		const float R[4][4]=
		{
			{self->object[0].scale,0,0,0},
			{0,self->object[0].scale,0,0},
			{0,0,self->object[0].scale,0},
			{0,0,0,1.0f},
		};

		glUniform4fv(shader_ambient_const,1,self->object[0].ambient);
		glUniform4fv(shader_diffuse_const,1,self->object[0].diffuse);
	
		glUniformMatrix4fv(shader_modelview,1,0,*M);
		glUniformMatrix4fv(shader_reshape,1,0,*R);

		glBindBuffer(GL_ARRAY_BUFFER,self->sphere.buffer[0]);
		glVertexAttribPointer(shader_vertex,4,GL_FLOAT,0,self->sphere.vertex.stride,(GLvoid*)self->sphere.vertex.offset);
		glVertexAttribPointer(shader_normal,4,GL_FLOAT,0,self->sphere.normal.stride,(GLvoid*)self->sphere.normal.offset);
		glVertexAttribPointer(shader_ambient,4,GL_FLOAT,0,self->sphere.ambient.stride,(GLvoid*)self->sphere.ambient.offset);
		glVertexAttribPointer(shader_diffuse,4,GL_FLOAT,0,self->sphere.diffuse.stride,(GLvoid*)self->sphere.diffuse.offset);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->sphere.buffer[1]);
		glDrawElements(GL_TRIANGLES,self->sphere.length,(GLsizei)self->sphere.stride,0);

	}
	{
		register unsigned int k,K;
		register const char*p=0;
		const float a[4]={1,1,1,1};
		const float d[4]={1,1,1,1};
		const float M[3][4][4]=
		{
			{
				{1,0,0,0},
				{0,1,0,0},
				{0,0,1,0},
				{0,0,0,1}
			},
			{
				{0,1,0,0},
				{-1,0,0,0},
				{0,0,1,0},
				{0,0,0,1}
			},
			{
				{0,0,-1,0},
				{0,1,0,0},
				{1,0,0,0},
				{0,0,0,1}
			},
		};
		glUniform4fv(shader_ambient_const,1,a);
		glUniform4fv(shader_diffuse_const,1,d);	

		for(k=0,K=3;k<K;++k)
		{
			char buff[64];
			register const char*p=self->label[k].label;
			glUniformMatrix4fv(shader_modelview,1,0,*M[k]);
			while(*p)
			{
				if(sprite_lcd_light(&self->string,*p,self->label[k].n.ambient,self->label[k].n.diffuse,self->label[k].p.ambient,self->label[k].p.diffuse,0))
				{
					const float r[4][4]=
					{
						{self->label[k].scale,0,0,0},
						{0,self->label[k].scale,0,0},
						{0,0,self->label[k].scale,0},
						{self->value[k].max+self->label[k].scale+0.6f*self->label[k].scale*(p-self->label[k].label),-self->label[k].scale,0,1}
					};
					glUniformMatrix4fv(shader_reshape,1,0,*r);

					glBindBuffer(GL_ARRAY_BUFFER,self->string.buffer[0]);
					glVertexAttribPointer(shader_vertex,4,GL_FLOAT,0,self->string.vertex.stride,(GLvoid*)self->string.vertex.offset);
					glVertexAttribPointer(shader_normal,4,GL_FLOAT,0,self->string.normal.stride,(GLvoid*)self->string.normal.offset);
					glVertexAttribPointer(shader_ambient,4,GL_FLOAT,0,self->string.ambient.stride,(GLvoid*)self->string.ambient.offset);
					glVertexAttribPointer(shader_diffuse,4,GL_FLOAT,0,self->string.diffuse.stride,(GLvoid*)self->string.diffuse.offset);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->string.buffer[1]);
					glDrawElements(GL_TRIANGLES,self->string.length,(GLsizei)self->string.stride,0);			
				}
				++p;
			}
			sprintf(buff,"%2.2f",self->value[k].max);
			p=buff;
			while(*p)
			{
				const float r[4][4]=
				{
					{self->label[k].scale,0,0,0},
					{0,self->label[k].scale,0,0},
					{0,0,self->label[k].scale,0},
					{self->value[k].max+self->label[k].scale+0.6f*self->label[k].scale*(p-buff),0,0,1}
				};
				glUniformMatrix4fv(shader_reshape,1,0,*r);

				sprite_lcd_light(&self->string,*p,
						self->label[k].n.ambient,
						self->label[k].n.diffuse,
						self->label[k].p.ambient,
						self->label[k].p.diffuse,0);

				glBindBuffer(GL_ARRAY_BUFFER,self->string.buffer[0]);
				glVertexAttribPointer(shader_vertex,4,GL_FLOAT,0,self->string.vertex.stride,(GLvoid*)self->string.vertex.offset);
				glVertexAttribPointer(shader_normal,4,GL_FLOAT,0,self->string.normal.stride,(GLvoid*)self->string.normal.offset);
				glVertexAttribPointer(shader_ambient,4,GL_FLOAT,0,self->string.ambient.stride,(GLvoid*)self->string.ambient.offset);
				glVertexAttribPointer(shader_diffuse,4,GL_FLOAT,0,self->string.diffuse.stride,(GLvoid*)self->string.diffuse.offset);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->string.buffer[1]);
				glDrawElements(GL_TRIANGLES,self->string.length,(GLsizei)self->string.stride,0);			
				++p;
			}
			sprintf(buff,"%2.2f",self->value[k].min);
			p=buff;
			while(*p)
			{
				const float r[4][4]=
				{
					{self->label[k].scale,0,0,0},
					{0,self->label[k].scale,0,0},
					{0,0,self->label[k].scale,0},
					{self->value[k].min-2.5f*self->label[k].scale+0.6f*self->label[k].scale*(p-buff),0,0,1}
				};
				glUniformMatrix4fv(shader_reshape,1,0,*r);

				sprite_lcd_light(&self->string,*p,
						self->label[k].n.ambient,
						self->label[k].n.diffuse,
						self->label[k].p.ambient,
						self->label[k].p.diffuse,0);

				glBindBuffer(GL_ARRAY_BUFFER,self->string.buffer[0]);
				glVertexAttribPointer(shader_vertex,4,GL_FLOAT,0,self->string.vertex.stride,(GLvoid*)self->string.vertex.offset);
				glVertexAttribPointer(shader_normal,4,GL_FLOAT,0,self->string.normal.stride,(GLvoid*)self->string.normal.offset);
				glVertexAttribPointer(shader_ambient,4,GL_FLOAT,0,self->string.ambient.stride,(GLvoid*)self->string.ambient.offset);
				glVertexAttribPointer(shader_diffuse,4,GL_FLOAT,0,self->string.diffuse.stride,(GLvoid*)self->string.diffuse.offset);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->string.buffer[1]);
				glDrawElements(GL_TRIANGLES,self->string.length,(GLsizei)self->string.stride,0);			
				++p;
			}
		}	
	}
	return 0;
}
int hp_back(hp_t*self,float r,float g,float b,float a,int dump)
{
	register int result=0;
	self->back[0]=r;
	self->back[1]=g;
	self->back[2]=b;
	self->back[3]=a;
	return result;
}
int hp_view(hp_t*self,float x,float y,float w,float h,int dump)
{
	register int result=0;
	self->view[0]=x;
	self->view[1]=y;
	self->view[2]=w;
	self->view[3]=h;
	hp_reshape(self,dump);
	return result;
}
int hp_root(hp_t*self,short w,short h,int dump)
{
	register int result=0;
	self->root[0]=w;
	self->root[1]=h;
	hp_reshape(self,dump);
	return result;
}
int hp_reshape(hp_t*self,int dump)
{
	register int result=0;
	register const float rate = sqrt(3.0f);
	register const float RW=self->root[0]*self->view[2];
	register const float RH=self->root[1]*self->view[3];
	if(RW==RH)*((mat4_t*)self->project.matrix)=mat4_ortho
		(
		self->value[0].min*rate,
		self->value[0].max*rate,
		self->value[1].min*rate, 
		self->value[1].max*rate,
		self->value[2].min*rate, 
		self->value[2].max*rate
		);
	else if(RW<RH)*((mat4_t*)self->project.matrix)=mat4_ortho
		(
		self->value[0].min*rate,
		self->value[0].max*rate,
		self->value[1].min*rate*RH/RW, 
		self->value[1].max*rate*RH/RW,
		self->value[2].min*rate, 
		self->value[2].max*rate	
		);
	else if(RW>RH)*((mat4_t*)self->project.matrix)=mat4_ortho
		(
		self->value[0].min*rate*RW/RH,
		self->value[0].max*rate*RW/RH,
		self->value[1].min*rate, 
		self->value[1].max*rate,
		self->value[2].min*rate, 
		self->value[2].max*rate	
		);
	if(dump)
	{
		register unsigned int r,R;
		for(r=0,R=4;r<R;++r)
		{
			register unsigned int c,C;
			for(c=0,C=4;c<C;++c)
			{
				char buff[16];
				result+=write(dump,buff,sprintf(buff,"%f%c",self->project.matrix[r][c],c==C-1?'\n':','));
			}
		}
	}
	else result=-1;
	return result;
}
int hp_update(hp_t*self,int dump)
{
	register unsigned int k,K;
	++self->frame;
	for(k=0,K=3;k<K;++k)
	{
		if(self->value[k].max<self->value[k].dst)self->value[k].dst=self->value[k].max+(self->value[k].dst-self->value[k].max)*self->value[k].decay;
		if(self->value[k].dst<self->value[k].min)self->value[k].dst=self->value[k].min+(self->value[k].dst-self->value[k].min)*self->value[k].decay;

		self->value[k].vel*=self->value[k].decay;
		self->value[k].vel+=(self->value[k].dst-self->value[k].cur)*self->value[k].accel;
		self->value[k].cur+=(self->value[k].vel*self->value[k].shift);
	}
	return 0;
}
int hp_lookat(hp_t*self,float ex,float ey,float ez,float ax,float ay,float az,float ux,float uy,float uz,int dump)
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
		{-vec3_dot(x,e),		-vec3_dot(y,e),			-vec3_dot(z,e),		1}
	};
	*((mat4_t*)self->pinhole.matrix)=*((mat4_t*)*M);
	return 0;
}

/*
int hd_light(hd_t*self,float x,float y,float z,int dump)
{
	self->light.from[0]=x;
	self->light.from[1]=y;
	self->light.from[2]=z;
	self->light.from[3]=1;
	return 0;
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
int hd_ortho(hd_t*self,float l,float r,float b,float t,float zmin,float zmax,int dump)
{
	*(mat4_t*)self->proj=mat4_ortho(l,r,b,t,zmin,zmax);
	return 0;
}
int hd_eeg_dict(hd_t*self,const char*file,int dump)
{
	unsigned int result=0;
	const char mess[]="Invalid data format";
	const char beg='[';
	const char end=']';
	const char del=' ';
	FILE*fp=fopen(file,"rb");
	if(fp)
	{
		char buff[16384];
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
						else if(dump)write(dump,mess,sizeof(mess));
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
*/
