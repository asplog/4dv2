//
//  hs.c
//  P4
//
//  Created by Kota Nakano on 16/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//
#include<stdio.h>
#include"gl.h"
#include"hs.h"
#include"shader.h"
#include"vector.h"
#include"colour.h"
#include"poly.h"
#include"line.h"
#include"projet.h"
#include"lcd.h"
#include"bill.h"
#include"sphere.h"
#include<strings.h>
#include<math.h>
int hs_init(hs_t*self,unsigned int name,short w,short h,int dump)
{
	register int result=0;
	register unsigned int k,K,j,J;
	bzero((void*)self,sizeof(hs_t));
	
	if(dump)
	{
		char buf[1024];
		result+=write(dump,buf,sprintf(buf,"HS INIT: BEGIN WITH %d\n",glGetError()));
	}

	self->name=name;
	self->flag=hs_BLEND_MODE|hs_DRAW_FILL|hs_DRAW_EDGE|hs_DRAW_NAME|hs_DRAW_DOTS|hs_ROTATION_UNWRAP;

	self->root[0]=w;
	self->root[1]=h;

	self->view[0]=0;
	self->view[1]=0.2;
	self->view[2]=0.8;
	self->view[3]=0.8;

	self->lines.width=2.4;
	
	self->lines.ambient[0]=0.5;
	self->lines.ambient[1]=0.5;
	self->lines.ambient[2]=0;
	self->lines.ambient[3]=1;
	
	self->lines.diffuse[0]=0.5;
	self->lines.diffuse[1]=0.5;
	self->lines.diffuse[2]=0;
	self->lines.diffuse[3]=0;
	
	self->alpha.rad=0.6f;
	self->alpha.decay=0.7f;
	self->alpha.accel=0.3f;
	self->alpha.shift=0.1f;
	
	self->theta.rad=0.6f;
	self->theta.decay=0.7f;
	self->theta.accel=0.3f;
	self->theta.shift=0.1f;
	
	result+=sprite_init_tetrahedron(self->poly+0,dump);
	result+=sprite_init_pentahedron(self->poly+1,dump);
	result+=sprite_init_octahedron(self->poly+2,dump);
	result+=sprite_init_icosahedron(self->poly+3,dump);
	result+=sprite_init_line(&self->line,dump);
	result+=sprite_lcd_init(&self->text,dump);
	result+=sprite_bill_init(&self->bill,dump);
	result+=sprite_sphere_init(&self->dots,dump);

	for(j=0,J=3;j<J;++j)
	{
		for(k=0,K=4;k<K;++k)
		{
			const unsigned int index=k*J+j;
			const unsigned int text[]={'C','C#','D','D#','E','F','F#','G','G#','A','A#','B'};
			//const unsigned int text[]={'C','C','D','D','E','F','F','G','G','A','A','B'};
			//const unsigned int text[]={'C','C','D','D','E','F','F','G','G','A','A','B'};
			//const unsigned int text[]={'0','1','2','3','4','5','6','7','8','9','a','b'};
		
			self->dict[(j*K+k*J)%12]=index;
		
			self->shape[index].refs=0;
			self->shape[index].flag=hs_DRAW_FILL|hs_DRAW_EDGE;

			self->label[index].refs=text[(j*K+k*J)%12];
			self->label[index].flag=hs_DRAW_FILL;
		
			self->shape[index].modelview[0][0]=1;
			self->shape[index].modelview[1][1]=1;
			self->shape[index].modelview[2][2]=1;
			self->shape[index].modelview[3][3]=1;

			self->label[index].modelview[0][0]=1;
			self->label[index].modelview[1][1]=1;
			self->label[index].modelview[2][2]=1;
			self->label[index].modelview[3][3]=1;
		
			self->shape[index].reshape[0][0]=0.1f;
			self->shape[index].reshape[1][1]=0.1f;
			self->shape[index].reshape[2][2]=0.1f;
			self->shape[index].reshape[3][3]=1;
	
			self->label[index].reshape[0][0]=0.2f;
			self->label[index].reshape[1][1]=0.2f;
			self->label[index].reshape[2][2]=0.2f;
			self->label[index].reshape[3][3]=1;
		
			self->label[index].reshape[3][1]=0.2f;
		
			self->shape[index].width=2.4;
			self->label[index].width=0.1;
		
			*((vec4_t*)self->shape[index].edge.ambient)=hsv2rgb(vec4_pack(30+120*(j),0.5,1.0,1.0));
			*((vec4_t*)self->shape[index].edge.diffuse)=hsv2rgb(vec4_pack(30+120*(j),0.5,1.0,0.0));
			*((vec4_t*)self->shape[index].fill.ambient)=hsv2rgb(vec4_pack(30+120*(j),1.0,1.0,1.0));
			*((vec4_t*)self->shape[index].fill.diffuse)=hsv2rgb(vec4_pack(30+120*(j),1.0,1.0,0.0));
			
			*((vec4_t*)self->label[index].unlit.ambient)=hsv2rgb(vec4_pack(30+120*(j),1.0,0.1,1.0));
			*((vec4_t*)self->label[index].unlit.diffuse)=hsv2rgb(vec4_pack(30+120*(j),0.0,0.0,0.0));
			*((vec4_t*)self->label[index].light.ambient)=hsv2rgb(vec4_pack(30+120*(j),1.0,0.5,1.0));
			*((vec4_t*)self->label[index].light.diffuse)=hsv2rgb(vec4_pack(30+120*(j),1.0,0.5,0.0));
		}
	}
	
	self->camera.fovy=100;
	self->camera.aspect=self->root[0]*self->view[2]/self->root[1]*self->view[3];
	self->camera.zmin=1.0f;
	self->camera.zmax=4.0f;
	
	result+=hs_perspective(self,self->camera.fovy,self->camera.aspect,self->camera.zmin,self->camera.zmax,dump);

	self->camera.from[0]=0,self->camera.from[1]=0.25,self->camera.from[2]=1.96;
	self->camera.to[0]=0,self->camera.to[1]=0,self->camera.to[2]=0;
	self->camera.up[0]=0,self->camera.up[1]=1,self->camera.up[2]=0;

	result+=hs_lookat(self,
			self->camera.from[0],self->camera.from[1],self->camera.from[2],
			self->camera.to[0],self->camera.to[1],self->camera.to[2],
			self->camera.up[0],self->camera.up[1],self->camera.up[2],	
			dump);

	self->light.from[0]=0,self->light.from[1]=4,self->light.from[2]=0;
	self->light.to[0]=0,self->light.to[1]=0,self->light.to[2]=0;
	self->light.fovy=360;

	
	self->geodes.flags=0x1f;
	self->geodes.attrib[0].resol=240;
	*((vec4_t*)self->geodes.attrib[0].scale)=vec4_pack(0.01,0.01,0.01,1);
	*((vec4_t*)self->geodes.attrib[0].ambient)=vec4_pack(0.5,0.5,0.5,0.75);
	*((vec4_t*)self->geodes.attrib[0].diffuse)=vec4_pack(1,0,1,0);
	self->geodes.attrib[1].resol=360;
	*((vec4_t*)self->geodes.attrib[1].scale)=vec4_pack(0.01,0.01,0.01,1);
	*((vec4_t*)self->geodes.attrib[1].ambient)=vec4_pack(0.5,0.5,0.5,0.75);
	*((vec4_t*)self->geodes.attrib[1].diffuse)=vec4_pack(0,1,1,0);
	self->geodes.attrib[2].resol=108;
	*((vec4_t*)self->geodes.attrib[2].scale)=vec4_pack(0.01,0.01,0.01,1);
	*((vec4_t*)self->geodes.attrib[2].ambient)=vec4_pack(0.5,0.5,0.5,0.75);
	*((vec4_t*)self->geodes.attrib[2].diffuse)=vec4_pack(1,0,0,0);
	self->geodes.attrib[3].resol=144;
	*((vec4_t*)self->geodes.attrib[3].scale)=vec4_pack(0.01,0.01,0.01,1);
	*((vec4_t*)self->geodes.attrib[3].ambient)=vec4_pack(0.5,0.5,0.5,0.75);
	*((vec4_t*)self->geodes.attrib[3].diffuse)=vec4_pack(0,1,1,0);
	self->geodes.attrib[4].resol=180;
	*((vec4_t*)self->geodes.attrib[4].scale)=vec4_pack(0.01,0.01,0.01,1);
	*((vec4_t*)self->geodes.attrib[4].ambient)=vec4_pack(0.5,0.5,0.5,0.75);
	*((vec4_t*)self->geodes.attrib[4].diffuse)=vec4_pack(1,1,0,0);

	result+=hs_light(self,self->light.from[0],self->light.from[1],self->light.from[2],dump);
	
	if(dump)
	{
		char buf[1024];
		result+=write(dump,buf,sprintf(buf,"HS INIT: END WITH %d\n",glGetError()));
	}
	
	return result;
}
int hs_draw(hs_t*self,int dump)
{
	glViewport(self->view[0]*self->root[0],self->view[1]*self->root[1],self->view[2]*self->root[0],self->view[3]*self->root[1]);
	{
		float E[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
		
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		
		glUniform1i(shader_diffuse_drive,0);
		glUniform1i(shader_diffuse_image,0);
		glUniform1i(shader_ambient_drive,0);
		glUniform1i(shader_ambient_image,0);
		
		glUniformMatrix4fv(shader_perspective,1,0,*E);
		glUniformMatrix4fv(shader_projection,1,0,*E);
		glUniformMatrix4fv(shader_modelview,1,0,*E);
		glUniformMatrix4fv(shader_reshape,1,0,*E);
		
		glUniform4fv(shader_ambient_const,1,self->back);
		glUniform4fv(shader_diffuse_const,1,self->back);
		
		glBindBuffer(GL_ARRAY_BUFFER,self->bill.buffer[0]);
		glVertexAttribPointer(shader_vertex,4,GL_FLOAT,0,self->bill.vertex.stride,(GLvoid*)(self->bill.vertex.offset));
		glVertexAttribPointer(shader_normal,4,GL_FLOAT,0,self->bill.normal.stride,(GLvoid*)self->bill.normal.offset);
		glVertexAttribPointer(shader_ambient,4,GL_FLOAT,0,self->bill.ambient.stride,(GLvoid*)self->bill.ambient.offset);
		glVertexAttribPointer(shader_diffuse,4,GL_FLOAT,0,self->bill.diffuse.stride,(GLvoid*)self->bill.diffuse.offset);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->bill.buffer[1]);
		glDrawElements(GL_TRIANGLES,self->bill.length,(GLsizei)self->bill.stride,0);
	}
	glClear(self->flag&hs_DEPTH_CLEAR?GL_DEPTH_BUFFER_BIT:0);
	(self->flag&hs_DEPTH_MODE?glEnable:glDisable)(GL_DEPTH_TEST);
	(self->flag&hs_BLEND_MODE?glEnable:glDisable)(GL_BLEND);
	glStencilMask(self->name);
	{
		glUniform4fv(shader_light_position,1,self->light.from);
		glUniform4fv(shader_camera_position,1,self->camera.from);
		glUniformMatrix4fv(shader_projection,1,0,*self->proj);
		glUniformMatrix4fv(shader_perspective,1,0,*self->pers);
	}
	{
		register unsigned int a,A;
		for(a=0,A=4;a<A;++a)
		{
			glBindBuffer(GL_ARRAY_BUFFER,self->poly[a].buffer[0]);
			glVertexAttribPointer(shader_vertex,4,GL_FLOAT,0,self->poly[a].vertex.stride,(GLvoid*)self->poly[a].vertex.offset);
			glVertexAttribPointer(shader_normal,4,GL_FLOAT,0,self->poly[a].normal.stride,(GLvoid*)self->poly[a].normal.offset);
			glVertexAttribPointer(shader_ambient,4,GL_FLOAT,0,self->poly[a].ambient.stride,(GLvoid*)self->poly[a].ambient.	offset);
			glVertexAttribPointer(shader_diffuse,4,GL_FLOAT,0,self->poly[a].diffuse.stride,(GLvoid*)self->poly[a].diffuse.offset);
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->poly[a].buffer[1]);
			register unsigned int t,T;
			for(t=0,T=3;t<T;++t)
			{
				register unsigned int index=a*T+t;
				
				glUniformMatrix4fv(shader_modelview,1,0,*self->shape[index].modelview);
				glUniformMatrix4fv(shader_reshape,1,0,*self->shape[index].reshape);
				
				if(self->flag&self->shape[index].flag&hs_DRAW_FILL)
				{
					glUniform4fv(shader_ambient_const,1,self->shape[index].fill.ambient);
					glUniform4fv(shader_diffuse_const,1,self->shape[index].fill.diffuse);
					
					glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
					glDrawElements(GL_TRIANGLES,self->poly[a].length,(GLsizei)self->poly[a].stride,0);
				}
				
				if(self->flag&self->shape[index].flag&hs_DRAW_EDGE)
				{
					glUniform4fv(shader_ambient_const,1,self->shape[index].edge.ambient);
					glUniform4fv(shader_diffuse_const,1,self->shape[index].edge.diffuse);
					
					glLineWidth(self->shape[index].width);
					
					glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
					glDrawElements(GL_TRIANGLES,self->poly[a].length,(GLsizei)self->poly[a].stride,0);
				}
			}
		}
	}
	if(self->flag&hs_DRAW_LINE)
	{
		const float e[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
		
		glLineWidth(self->lines.width);
		glUniform4fv(shader_ambient_const,1,self->lines.ambient);
		glUniform4fv(shader_diffuse_const,1,self->lines.diffuse);
		
		glUniformMatrix4fv(shader_modelview,1,0,*e);
		glUniformMatrix4fv(shader_reshape,1,0,*e);
		
		glBindBuffer(GL_ARRAY_BUFFER,self->line.buffer[0]);
		glVertexAttribPointer(shader_vertex,4,GL_FLOAT,0,self->line.vertex.stride,(GLvoid*)self->line.vertex.offset);
		glVertexAttribPointer(shader_normal,4,GL_FLOAT,0,self->line.normal.stride,(GLvoid*)self->line.normal.offset);
		glVertexAttribPointer(shader_ambient,4,GL_FLOAT,0,self->line.ambient.stride,(GLvoid*)self->line.ambient.offset);
		glVertexAttribPointer(shader_diffuse,4,GL_FLOAT,0,self->line.diffuse.stride,(GLvoid*)self->line.diffuse.offset);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->line.buffer[1]);
		glDrawElements(GL_LINES,self->line.length,(GLsizei)self->line.stride,0);
	}
	if(self->flag&hs_DRAW_DOTS)
	{
		float R[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
		float M[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
		
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		
		glBindBuffer(GL_ARRAY_BUFFER,self->dots.buffer[0]);
		glVertexAttribPointer(shader_vertex,4,GL_FLOAT,0,self->dots.vertex.stride,(GLvoid*)self->dots.vertex.offset);
		glVertexAttribPointer(shader_normal,4,GL_FLOAT,0,self->dots.normal.stride,(GLvoid*)self->dots.normal.offset);
		glVertexAttribPointer(shader_ambient,4,GL_FLOAT,0,self->dots.ambient.stride,(GLvoid*)self->dots.ambient.offset);
		glVertexAttribPointer(shader_diffuse,4,GL_FLOAT,0,self->dots.diffuse.stride,(GLvoid*)self->dots.diffuse.offset);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->dots.buffer[1]);
		

		if(self->geodes.flags&1<<0)//+1
		{
			register unsigned int n,N;
			R[0][0]=self->geodes.attrib[0].scale[0];
			R[1][1]=self->geodes.attrib[0].scale[1];
			R[2][2]=self->geodes.attrib[0].scale[2];
			glUniformMatrix4fv(shader_reshape,1,0,*R);
			glUniform4fv(shader_ambient_const,1,self->geodes.attrib[0].ambient);
			glUniform4fv(shader_diffuse_const,1,self->geodes.attrib[0].diffuse);	
			for(n=0,N=self->geodes.attrib[0].resol;n<N;++n)
			{
				register const float ar=2*M_PI*(self->alpha.cur/360.0f+3*n/(float)N);
				register const float tr=2*M_PI*(self->theta.cur/360.0f-4*n/(float)N);
				register const float ac=cosf(ar);
				register const float as=sinf(ar);			
				register const float tc=cosf(tr);
				register const float ts=sinf(tr);
				M[3][0]=self->alpha.rad*ac+self->theta.rad*tc;
				M[3][1]=self->theta.rad*ts;
				M[3][2]=self->alpha.rad*as;
				glUniformMatrix4fv(shader_modelview,1,0,*M);
				glDrawElements(GL_TRIANGLES,self->dots.length,(GLsizei)self->dots.stride,0);
			}
		}
		if(self->geodes.flags&1<<1)//+2
		{
			register unsigned int n,N;
			R[0][0]=self->geodes.attrib[1].scale[0];
			R[1][1]=self->geodes.attrib[1].scale[1];
			R[2][2]=self->geodes.attrib[1].scale[2];
			glUniformMatrix4fv(shader_reshape,1,0,*R);
			glUniform4fv(shader_ambient_const,1,self->geodes.attrib[1].ambient);
			glUniform4fv(shader_diffuse_const,1,self->geodes.attrib[1].diffuse);	
			for(n=0,N=self->geodes.attrib[1].resol;n<N;++n)//+2
			{
				{
					register const float ar=2*M_PI*(self->alpha.cur/360.0f+1/4.0+6*n/(float)N);
					register const float tr=2*M_PI*(self->theta.cur/360.0f+4*n/(float)N);
					register const float ac=cosf(ar);
					register const float as=sinf(ar);			
					register const float tc=cosf(tr);
					register const float ts=sinf(tr);
					M[3][0]=self->alpha.rad*ac+self->theta.rad*tc;
					M[3][1]=self->theta.rad*ts;
					M[3][2]=self->alpha.rad*as;
					glUniformMatrix4fv(shader_modelview,1,0,*M);
					glDrawElements(GL_TRIANGLES,self->dots.length,(GLsizei)self->dots.stride,0);
				}
				{
					register const float ar=2*M_PI*(self->alpha.cur/360.0f+6*n/(float)N);
					register const float tr=2*M_PI*(self->theta.cur/360.0f+4*n/(float)N);
					register const float ac=cosf(ar);
					register const float as=sinf(ar);			
					register const float tc=cosf(tr);
					register const float ts=sinf(tr);
					M[3][0]=self->alpha.rad*ac+self->theta.rad*tc;
					M[3][1]=self->theta.rad*ts;
					M[3][2]=self->alpha.rad*as;
					glUniformMatrix4fv(shader_modelview,1,0,*M);
					glDrawElements(GL_TRIANGLES,self->dots.length,(GLsizei)self->dots.stride,0);
				}
			}
		}	
		if(self->geodes.flags&1<<2)
		{
			register unsigned int n,N;
			R[0][0]=self->geodes.attrib[2].scale[0];
			R[1][1]=self->geodes.attrib[2].scale[1];
			R[2][2]=self->geodes.attrib[2].scale[2];
			glUniformMatrix4fv(shader_reshape,1,0,*R);
			glUniform4fv(shader_ambient_const,1,self->geodes.attrib[2].ambient);
			glUniform4fv(shader_diffuse_const,1,self->geodes.attrib[2].diffuse);	
			for(n=0,N=4;n<4;++n)
			{
				register float ar=M_PI*(self->alpha.cur/180.0f+n/(N*0.5f));
				register float ac=cosf(ar);
				register float as=sinf(ar);
				register unsigned int k,K;
				for(k=0,K=self->geodes.attrib[2].resol/N;k<K;++k)
				{
					register float tr=M_PI*(self->theta.cur/180.0f+k/(K*0.5f));
					register float tc=cosf(tr);
					register float ts=sinf(tr);
					M[3][0]=self->alpha.rad*ac+self->theta.rad*tc;
					M[3][1]=self->theta.rad*ts;
					M[3][2]=self->alpha.rad*as;
					glUniformMatrix4fv(shader_modelview,1,0,*M);
					glDrawElements(GL_TRIANGLES,self->dots.length,(GLsizei)self->dots.stride,0);
				}
			}
		}
		if(self->geodes.flags&1<<3)
		{
			register unsigned int n,N;
			R[0][0]=self->geodes.attrib[3].scale[0];
			R[1][1]=self->geodes.attrib[3].scale[1];
			R[2][2]=self->geodes.attrib[3].scale[2];
			glUniformMatrix4fv(shader_reshape,1,0,*R);
			glUniform4fv(shader_ambient_const,1,self->geodes.attrib[3].ambient);
			glUniform4fv(shader_diffuse_const,1,self->geodes.attrib[3].diffuse);	
			for(n=0,N=3;n<N;++n)
			{
				register float tr=M_PI*(self->theta.cur/180.0f+n/(N*0.5f));
				register float tc=cosf(tr);
				register float ts=sinf(tr);
				register unsigned int k,K;
				for(k=0,K=self->geodes.attrib[3].resol/N;k<K;++k)
				{
					register float ar=M_PI*(self->alpha.cur/180.0f+k/(K*0.5f));
					register float ac=cosf(ar);
					register float as=sinf(ar);
					M[3][0]=self->alpha.rad*ac+self->theta.rad*tc;
					M[3][1]=self->theta.rad*ts;
					M[3][2]=self->alpha.rad*as;
					glUniformMatrix4fv(shader_modelview,1,0,*M);
					glDrawElements(GL_TRIANGLES,self->dots.length,(GLsizei)self->dots.stride,0);
				}
			}
		}
		if(self->geodes.flags&1<<4)//+5
		{
			register unsigned int n,N;
			R[0][0]=self->geodes.attrib[4].scale[0];
			R[1][1]=self->geodes.attrib[4].scale[1];
			R[2][2]=self->geodes.attrib[4].scale[2];
			glUniformMatrix4fv(shader_reshape,1,0,*R);
			glUniform4fv(shader_ambient_const,1,self->geodes.attrib[4].ambient);
			glUniform4fv(shader_diffuse_const,1,self->geodes.attrib[4].diffuse);
			for(n=0,N=self->geodes.attrib[4].resol;n<N;++n)//+5
			{
				register const float ar=2*M_PI*(self->alpha.cur/360.0f+3*n/(float)N);
				register const float tr=2*M_PI*(self->theta.cur/360.0f+4*n/(float)N);
				register const float ac=cosf(ar);
				register const float as=sinf(ar);			
				register const float tc=cosf(tr);
				register const float ts=sinf(tr);
				M[3][0]=self->alpha.rad*ac+self->theta.rad*tc;
				M[3][1]=self->theta.rad*ts;
				M[3][2]=self->alpha.rad*as;
				glUniformMatrix4fv(shader_modelview,1,0,*M);
				glDrawElements(GL_TRIANGLES,self->dots.length,(GLsizei)self->dots.stride,0);
			}
		}
	}
		
#if 0
	if(0)
	{
		const float e[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
		glUniform4fv(shader_ambient_const,1,self->lines.ambient);
		glUniform4fv(shader_diffuse_const,1,self->lines.diffuse);
		{	
			const float e[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{1,0,0,1}};
			glUniformMatrix4fv(shader_modelview,1,0,*e);
		}
		{
			const float e[4][4]={{0.1,0,0,0},{0,.1,0,0},{0,0,.1,0},{0,0,0,1}};
			glUniformMatrix4fv(shader_reshape,1,0,*e);
		}
		glBindBuffer(GL_ARRAY_BUFFER,self->goal.buffer[0]);
		glVertexAttribPointer(shader_vertex,4,GL_FLOAT,0,self->goal.vertex.stride,(GLvoid*)self->goal.vertex.offset);
		glVertexAttribPointer(shader_normal,4,GL_FLOAT,0,self->goal.normal.stride,(GLvoid*)self->goal.normal.offset);
		glVertexAttribPointer(shader_ambient,4,GL_FLOAT,0,self->goal.ambient.stride,(GLvoid*)self->goal.ambient.offset);
		glVertexAttribPointer(shader_diffuse,4,GL_FLOAT,0,self->goal.diffuse.stride,(GLvoid*)self->goal.diffuse.offset);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->goal.buffer[1]);
		glDrawElements(GL_TRIANGLES,self->goal.length,(GLsizei)self->goal.stride,0);

	}
#endif
	if(self->flag&hs_DRAW_NAME)
	{
		const float E[4]={1,1,1,1};
		
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		
		glBindBuffer(GL_ARRAY_BUFFER,self->text.buffer[0]);
		glVertexAttribPointer(shader_vertex,4,GL_FLOAT,0,self->text.vertex.stride,(GLvoid*)self->text.vertex.offset);
		glVertexAttribPointer(shader_normal,4,GL_FLOAT,0,self->text.normal.stride,(GLvoid*)self->text.normal.offset);
		glVertexAttribPointer(shader_ambient,4,GL_FLOAT,0,self->text.ambient.stride,(GLvoid*)self->text.ambient.offset);
		glVertexAttribPointer(shader_diffuse,4,GL_FLOAT,0,self->text.diffuse.stride,(GLvoid*)self->text.diffuse.offset);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->text.buffer[1]);
		
		glUniform4fv(shader_ambient_const,1,E);
		glUniform4fv(shader_diffuse_const,1,E);
		{
			register unsigned int a,A;
			for(a=0,A=4;a<A;++a)
			{
				register unsigned int t,T;
				for(t=0,T=3;t<T;++t)
				{
					register unsigned int index=a*T+t,k,K;
					for(k=0,K=4;k<K;++k)
					{
						if(sprite_lcd_light(&self->text,((unsigned char*)&self->label[index].refs)[k],
											self->label[index].unlit.ambient,
											self->label[index].unlit.diffuse,
											self->label[index].light.ambient,
											self->label[index].light.diffuse,
											0))
						{
							float R[4][4];
							*((mat4_t*)R)=*(mat4_t*)self->label[index].reshape;
							R[3][0]=-R[0][0]*k*0.6;
							glUniformMatrix4fv(shader_modelview,1,0,*self->label[index].modelview);
							glUniformMatrix4fv(shader_reshape,1,0,*R);
							glDrawElements(GL_TRIANGLES,self->text.length,(GLsizei)self->text.stride,0);
						}
					}
				}
			}
		}
	}
	return 0;
}
int hs_perspective(hs_t*self,float fovy,float aspect,float zmin,float zmax,int dump)
{
	*((mat4_t*)self->proj)=mat4_perspective(fovy,aspect,zmin,zmax);
	return 0;
}
int hs_lookat(hs_t*self,float ex,float ey,float ez,float ax,float ay,float az,float ux,float uy,float uz,int dump)
{
	register vec3_t e={ex,ey,ez};
	register vec3_t l={ax,ay,az};
	register vec3_t u=vec3_normalize(vec3_pack(ux,uy,uz));
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
int hs_update(hs_t*self,int dump)
{
	++self->frame;
	register int result=0;
	register unsigned int a,A;
	float hypershpere[12][4];
	{
		self->alpha.vel*=self->alpha.decay;
		self->alpha.vel+=self->alpha.accel*(self->alpha.dst-self->alpha.cur);
		self->alpha.cur+=self->alpha.shift*self->alpha.vel;

		self->theta.vel*=self->theta.decay;
		self->theta.vel+=self->theta.accel*(self->theta.dst-self->theta.cur);
		self->theta.cur+=self->theta.shift*self->theta.vel;
	}
	for(a=0,A=4;a<A;++a)
	{
		register unsigned int t,T;
		register float alpha=2.0f*M_PI*(0.25f+a/(float)A+self->alpha.cur/360.0f);
		for(t=0,T=3;t<T;++t)
		{
			register unsigned int index=a*T+t;
			register float theta=2.0f*M_PI*(t/(float)T+self->theta.cur/360.0f);
			
			hypershpere[index][0]=self->alpha.rad*cosf(alpha)+self->theta.rad*cosf(theta);
			hypershpere[index][1]=self->theta.rad*sinf(theta);
			hypershpere[index][2]=self->alpha.rad*sinf(alpha);
			hypershpere[index][3]=1;
			
			self->shape[index].modelview[3][0]=hypershpere[index][0];
			self->shape[index].modelview[3][1]=hypershpere[index][1];
			self->shape[index].modelview[3][2]=hypershpere[index][2];
			
			self->label[index].modelview[3][0]=hypershpere[index][0];
			self->label[index].modelview[3][1]=hypershpere[index][1];
			self->label[index].modelview[3][2]=hypershpere[index][2];
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER,self->line.buffer[0]);
	glBufferSubData(GL_ARRAY_BUFFER,self->line.vertex.offset,sizeof(hypershpere),hypershpere);
	glBufferSubData(GL_ARRAY_BUFFER,self->line.normal.offset,sizeof(hypershpere),hypershpere);
	return result;
}
int hs_light(hs_t*self,float x,float y,float z,int dump)
{
	self->light.from[0]=x;
	self->light.from[1]=y;
	self->light.from[2]=z;
	self->light.from[3]=1;
	return 0;
}
int hs_view(hs_t*self,float x,float y,float w,float h,int dump)
{
	register int result=0;
	self->view[0]=x;
	self->view[1]=y;
	self->view[2]=w;
	self->view[3]=h;
	self->camera.aspect=self->root[0]*self->view[2]/self->root[1]*self->view[3];	
	result+=hs_perspective(self,self->camera.fovy,self->camera.aspect,self->camera.zmin,self->camera.zmax,dump);
	return result;
}
int hs_root(hs_t*self,short w,short h,int dump)
{
	register int result=0;
	self->root[0]=w;
	self->root[1]=h;
	self->camera.aspect=self->root[0]*self->view[2]/self->root[1]*self->view[3];	
	result+=hs_perspective(self,self->camera.fovy,self->camera.aspect,self->camera.zmin,self->camera.zmax,dump);
	return result;
}
int hs_back(hs_t*self,float r,float g,float b,float a,int dump)
{
	register int result=0;
	self->back[0]=r;
	self->back[1]=g;
	self->back[2]=b;
	self->back[3]=a;
	return result;
}
int hs_term(hs_t*self,int dump)
{
	int result=~0;
	return result;
}
