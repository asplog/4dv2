#include"hs.h"
#include"hd.h"
#include"hp.h"
#include"string.h"
#include"colour.h"
#include"handler.h"
#include"matrix.h"
#include"projet.h"
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<strings.h>
static inline void parseMatrix(register char**token,float*t)
{
	register char*p=*token++;
	do switch(*p)
	{
		case '0':if(*token)t[0x0]=atof(*token++);break;
		case '1':if(*token)t[0x1]=atof(*token++);break;
		case '2':if(*token)t[0x2]=atof(*token++);break;
		case '3':if(*token)t[0x3]=atof(*token++);break;
		case '4':if(*token)t[0x4]=atof(*token++);break;
		case '5':if(*token)t[0x5]=atof(*token++);break;
		case '6':if(*token)t[0x6]=atof(*token++);break;
		case '7':if(*token)t[0x7]=atof(*token++);break;
		case '8':if(*token)t[0x8]=atof(*token++);break;
		case '9':if(*token)t[0x9]=atof(*token++);break;
		case 'a':if(*token)t[0xa]=atof(*token++);break;
		case 'b':if(*token)t[0xb]=atof(*token++);break;
		case 'c':if(*token)t[0xc]=atof(*token++);break;
		case 'd':if(*token)t[0xd]=atof(*token++);break;
		case 'e':if(*token)t[0xe]=atof(*token++);break;
		case 'f':if(*token)t[0xf]=atof(*token++);break;
	}
	while(*++p);
	return;
}
static inline void parseScale(register char**token,float*t)
{
	register char*p=*token++;
	do switch(*p)
	{
		case 'x':if(*token)t[0x0]=atof(*token++);break;
		case 'y':if(*token)t[0x5]=atof(*token++);break;
		case 'z':if(*token)t[0xa]=atof(*token++);break;
		case 'w':if(*token)t[0xf]=atof(*token++);break;
	}
	while(*++p);
	return;
}
static inline void parsePosit(register char**token,float*t)
{
	register char*p=*token++;
	do switch(*p)
	{
		case 'x':if(*token)t[0]=atof(*token++);break;
		case 'y':if(*token)t[1]=atof(*token++);break;
		case 'z':if(*token)t[2]=atof(*token++);break;
		case 'w':if(*token)t[3]=atof(*token++);break;
	}
	while(*++p);
	return;
}
static inline void parseVport(register char**token,float*t)
{
	register char*p=*token++;
	do switch(*p)
	{
		case 'x':if(*token)t[0]=atof(*token++);break;
		case 'y':if(*token)t[1]=atof(*token++);break;
		case 'w':if(*token)t[2]=atof(*token++);break;
		case 'h':if(*token)t[3]=atof(*token++);break;
	}
	while(*++p);
	return;
}
static inline void parseColor(register char**token,float*c)
{
	register char*p=*token++;
	do switch(*p)
	{
		case 'h':if(*token){vec4_t hsva=rgb2hsv(*(vec4_t*)c);hsva.h=atof(*token++);*((vec4_t*)c)=hsv2rgb(hsva);}break;
		case 's':if(*token){vec4_t hsva=rgb2hsv(*(vec4_t*)c);hsva.s=atof(*token++);*((vec4_t*)c)=hsv2rgb(hsva);}break;
		case 'v':if(*token){vec4_t hsva=rgb2hsv(*(vec4_t*)c);hsva.v=atof(*token++);*((vec4_t*)c)=hsv2rgb(hsva);}break;
		case 'r':if(*token)c[0]=atof(*token++);break;
		case 'g':if(*token)c[1]=atof(*token++);break;
		case 'b':if(*token)c[2]=atof(*token++);break;
		case 'a':if(*token)c[3]=atof(*token++);break;
	}
	while(*++p);
	return;
}
int hs_handler_help()
{
	fprintf(stderr,"message format:\n");
	fprintf(stderr,"\t%s <TARGET> (SUBTARGET)... <VALUE> ...:\n",_SET_);
	fprintf(stderr,"\t\tchange target value\n");
	
	fprintf(stderr,"\t\t\t%s %s <NUMBER>: set alpha value for rotation\n",_SET_,_ALPHA_);
	fprintf(stderr,"\t\t\t%s %s <NUMBER>: set theta value for rotatoin\n",_SET_,_THETA_);
	
	fprintf(stderr,"\t\t\t%s %s xywh <START_X:0~1> <START_Y:0~1> <WIDTH:0~1> <HEIGHT:0~1>: set viewport area\n",_SET_,_VIEW_);
	fprintf(stderr,"\t\t\t%s %s (rgb|hsv) <R:0~1/H:0~360> <G:0~1/S:0~1> <B:0~1/V:0~1>: set background color\n",_SET_,_BACK_);
	
	fprintf(stderr,"\t\t\tset camera up xyz <X> <Y> <Z>: set camera upper direction\n");
	fprintf(stderr,"\t\t\tset camera from xyz <X> <Y> <Z>: set camera position\n");
	fprintf(stderr,"\t\t\tset camera to xyz <X> <Y> <Z>: set camera target\n");
	
	fprintf(stderr,"\t\t\tset light from xyz <X> <Y> <Z>: set light position\n");
	
	fprintf(stderr,"\t\t\tset common fill (0|1): set common flag for fill rendering\n");
	fprintf(stderr,"\t\t\tset common edge (0|1): set common flag for edge rendering\n");
	fprintf(stderr,"\t\t\tset common line (0|1): set common flag for line rendering\n");
	fprintf(stderr,"\t\t\tset common dots (0|1): set common flag for dots rendering\n");
	
	fprintf(stderr,"\t\t\tset note <NOTE_NUMBER> shape shape <NUMBER>: set object type for note <NOTE_NUMBER>\n");
	fprintf(stderr,"\t\t\tset note <NOTE_NUMBER> shape size xyz <X> <Y> <Z>: set object color for note <NOTE_NUMBER>\n");
	fprintf(stderr,"\t\t\tset note <NOTE_NUMBER> shape fill ambient (hsv|rgb) <R/H> <G/S> <B/V>:\n\t\t\t\t set surface ambient color for note <NOTE_NUMBER> object with rgb or hsv format\n");
	fprintf(stderr,"\t\t\tset note <NOTE_NUMBER> shape fill diffuse (hsv|rgb) <R/H> <G/S> <B/V>:\n\t\t\t\t set surface diffuse color for note <NOTE_NUMBER> object with rgb or hsv format\n");
	fprintf(stderr,"\t\t\tset note <NOTE_NUMBER> shape edge ambient (hsv|rgb) <R/H> <G/S> <B/V>:\n\t\t\t\t set edge ambient color for note <NOTE_NUMBER> object with rgb or hsv format\n");
	fprintf(stderr,"\t\t\tset note <NOTE_NUMBER> shape edge diffuse (hsv|rgb) <R/H> <G/S> <B/V>:\n\t\t\t\t set edge diffuse color for note <NOTE_NUMBER> object with rgb or hsv format\n");

	fprintf(stderr,"\t\t\tset note <NOTE_NUMBER> label label <NUMBER>: set label text for note <NOTE_NUMBER>\n");
	fprintf(stderr,"\t\t\tset note <NOTE_NUMBER> label size xyz <X> <Y> <Z>: set object color for note <NOTE_NUMBER>\n");
	fprintf(stderr,"\t\t\tset note <NOTE_NUMBER> label fill ambient (hsv|rgb) <R/H> <G/S> <B/V>:\n\t\t\t\t set surface ambient color for note <NOTE_NUMBER> label with rgb or hsv format\n");
	fprintf(stderr,"\t\t\tset note <NOTE_NUMBER> label fill diffuse (hsv|rgb) <R/H> <G/S> <B/V>:\n\t\t\t\t set surface diffuse color for note <NOTE_NUMBER> label with rgb or hsv format\n");
	fprintf(stderr,"\t\t\tset note <NOTE_NUMBER> label edge ambient (hsv|rgb) <R/H> <G/S> <B/V>:\n\t\t\t\t set edge ambient color for note <NOTE_NUMBER> label with rgb or hsv format\n");
	fprintf(stderr,"\t\t\tset note <NOTE_NUMBER> label edge diffuse (hsv|rgb) <R/H> <G/S> <B/V>:\n\t\t\t\t set edge diffuse color for note <NOTE_NUMBER> label with rgb or hsv format\n");

	fprintf(stderr,"\t\t\tset dots <DOTS_NUMBER> up (0|1): set flag for geodesic drawing\n");
	fprintf(stderr,"\t\t\tset dots <DOTS_NUMBER> resolution <NUMBER>: set the number of dots\n");
	fprintf(stderr,"\t\t\tset dots <DOTS_NUMBER> ambient (hsv|rgb) <R/H> <G/S> <B/V>:\n\t\t\t\t set ambient color for geodesic <DOTS_NUMBER> drawing with rgb or hsv format\n");
	fprintf(stderr,"\t\t\tset dots <DOTS_NUMBER> diffuse (hsv|rgb) <R/H> <G/S> <B/V>:\n\t\t\t\t set diffuse color for geodesic <DOTS_NUMBER> drawing with rgb or hsv format\n");
	return 0;
}
int hs_handler(hs_t*self,const char*buffer,unsigned int length,int dump)
{
	char*token[64]={0};
	int count=string_split_unsafe(memcpy(malloc(length),buffer,length),' ',token);
	if(!token[0]);
	else if(!strcmp(token[0],_SET_))
	{
		if(!token[1]);
		else if(!strcmp(token[1],_BACK_)&&token[2])parseColor(token+2,self->back);
		else if(!strcmp(token[1],_VIEW_)&&token[2])parseVport(token+2,self->view);
		else if(!strcmp(token[1],_ALPHA_)&&token[2])
		{
			register float dst=atof(token[2]);
			if(self->flag&hs_ROTATION_UNWRAP)
			{
				while( 180.0f<dst-self->alpha.cur)dst-=360.0f;
				while(dst-self->alpha.cur<-180.0f)dst+=360.0f;
			}
			self->alpha.dst=dst;
		}
		else if(!strcmp(token[1],_THETA_)&&token[2])
		{
			register float dst=atof(token[2]);
			if(self->flag&hs_ROTATION_UNWRAP)
			{
				while( 180.0f<dst-self->theta.cur)dst-=360.0f;
				while(dst-self->theta.cur<-180.0f)dst+=360.0f;
			}
			self->theta.dst=dst;	
		}
		else if(!strcmp(token[1],_COMMON_))
		{
			if(!token[2]);
			else if(!strcmp(token[2],_FILL_)&&token[3])self->flag=atoi(token[3])?self->flag|hs_DRAW_FILL:self->flag&~hs_DRAW_FILL;
			else if(!strcmp(token[2],_EDGE_)&&token[3])self->flag=atoi(token[3])?self->flag|hs_DRAW_EDGE:self->flag&~hs_DRAW_EDGE;
			else if(!strcmp(token[2],_LINE_)&&token[3])self->flag=atoi(token[3])?self->flag|hs_DRAW_LINE:self->flag&~hs_DRAW_LINE;
			else if(!strcmp(token[2],_DOTS_)&&token[3])self->flag=atoi(token[3])?self->flag|hs_DRAW_DOTS:self->flag&~hs_DRAW_DOTS;
			else if(!strcmp(token[2],_NAME_)&&token[3])self->flag=atoi(token[3])?self->flag|hs_DRAW_NAME:self->flag&~hs_DRAW_NAME;
		}
		else if(!strcmp(token[1],_LIGHT_))
		{
			if(!token[2]);
			else if(!strcmp(token[2],_FROM_)&&token[3])parsePosit(token+3,self->light.from);
			else if(!strcmp(token[2],_TO_)&&token[3])parsePosit(token+3,self->light.to);
			else if(!strcmp(token[2],_FOVY_)&&token[3])self->light.fovy=atof(token[3]);
		}
		else if(!strcmp(token[1],_CAMERA_))
		{
			if(!token[2]);
			else if(!strcmp(token[2],_FROM_)&&token[3])parsePosit(token+3,self->camera.from);
			else if(!strcmp(token[2],_TO_)&&token[3])parsePosit(token+3,self->camera.to);
			else if(!strcmp(token[2],_UP_)&&token[3])parsePosit(token+3,self->camera.up);
			else if(!strcmp(token[2],_FOVY_)&&token[3])self->camera.fovy=atof(token[3]);
			else if(!strcmp(token[2],_ASPECT_)&&token[3])self->camera.aspect=atof(token[3]);
			else if(!strcmp(token[2],_ZMIN_)&&token[3])self->camera.zmin=atof(token[3]);
			else if(!strcmp(token[2],_ZMAX_)&&token[3])self->camera.zmax=atof(token[3]);
			hs_lookat(self,
					self->camera.from[0],self->camera.from[1],self->camera.from[2],
					self->camera.to[0],self->camera.to[1],self->camera.to[2],
					self->camera.up[0],self->camera.up[1],self->camera.up[2],dump);
			hs_perspective(self,self->camera.fovy,self->camera.aspect,self->camera.zmin,self->camera.zmax,dump);
		
		}
		else if(!strcmp(token[1],_DOTS_))
		{
			if(!token[2]);
			else
			{
				unsigned int index=atoi(token[2])%5;
				unsigned int value=0;
				if(!token[3]);
				else if(!strcmp(token[3],_UP_)&&token[4])self->geodes.flags=atoi(token[4])?self->geodes.flags|(1<<index):self->geodes.flags&~(1<<index);	
				else if(!strcmp(token[3],_AMBIENT_)&&token[4])parseColor(token+4,self->geodes.attrib[index].ambient);
				else if(!strcmp(token[3],_DIFFUSE_)&&token[4])parseColor(token+4,self->geodes.attrib[index].diffuse);
				//else if(!strcmp(token[3],_SIZE_)&&token[4])parseScale(token
				else if(!strcmp(token[3],_RESO_)&&token[4])self->geodes.attrib[index].resol=atoi(token[4]);
			}
		}
		else if(!strcmp(token[1],_NOTE_))
		{
			if(!token[2]);
			else
			{
				unsigned int note=self->dict[atoi(token[2])%12];
				if(!token[3]);
				else if(!strcmp(token[3],_SHAPE_))
				{
					if(!token[4]);
					else if(!strcmp(token[4],_SHAPE_)&&token[5])self->shape[note].refs=atoi(token[5])%4;
					else if(!strcmp(token[4],_SIZE_)&&token[5])parseScale(token+5,*self->shape[note].reshape);
					else if(!strcmp(token[4],_MATRIX_)&&token[5])parseMatrix(token+5,*self->shape[note].reshape);
					else if(!strcmp(token[4],_FILL_))
					{
						if(!token[5]);
						else if(!strcmp(token[5],_AMBIENT_)&&token[6])parseColor(token+6,self->shape[note].fill.ambient);
						else if(!strcmp(token[5],_DIFFUSE_)&&token[6])parseColor(token+6,self->shape[note].fill.diffuse);
					}
					else if(!strcmp(token[4],_EDGE_))
					{
						if(!token[5]);
						else if(!strcmp(token[5],_AMBIENT_)&&token[6])parseColor(token+6,self->shape[note].edge.ambient);
						else if(!strcmp(token[5],_DIFFUSE_)&&token[6])parseColor(token+6,self->shape[note].edge.diffuse);
					}
				}
				else if(!strcmp(token[3],_LABEL_))
				{
					if(!token[4]);
					else if(!strcmp(token[4],_LABEL_)&&token[5])self->shape[note].refs=*(unsigned int*)token[5];
					else if(!strcmp(token[4],_SIZE_)&&token[5])parseScale(token+5,*self->label[note].reshape);
					else if(!strcmp(token[4],_MATRIX_)&&token[5])parseMatrix(token+5,*self->label[note].reshape);
					else if(!strcmp(token[4],_LIGHT_))
					{
						if(!token[5]);
						else if(!strcmp(token[5],_AMBIENT_)&&token[6])parseColor(token+6,self->label[note].light.ambient);
						else if(!strcmp(token[5],_DIFFUSE_)&&token[6])parseColor(token+6,self->label[note].light.diffuse);
					}
					else if(!strcmp(token[4],_UNLIT_))
					{
						if(!token[5]);
						else if(!strcmp(token[5],_AMBIENT_)&&token[6])parseColor(token+6,self->label[note].unlit.ambient);
						else if(!strcmp(token[5],_DIFFUSE_)&&token[6])parseColor(token+6,self->label[note].unlit.diffuse);
					}
				}
			}	
		}
	}
	free(*token);
	return 0;
}
int hd_handler_help()
{
	fprintf(stderr,"message format:\n");
	fprintf(stderr,"\tmap <NUMBER> <NUMBER> ...:\n");
	fprintf(stderr,"\t\tchange brain map:\n");
	fprintf(stderr,"\tset <TARGET> (SUBTARGET)... <VALUE> ...:\n");
	fprintf(stderr,"\t\tchange target value\n");
	fprintf(stderr,"\t\t\t%s %s <NUMBER>: set bias value for brain coloring\n",_SET_,_BIAS_);
	fprintf(stderr,"\t\t\t%s %s <NUMBER>: set gain value for brain coloring\n",_SET_,_GAIN_);	
	fprintf(stderr,"\t\t\t%s %s xywh <START_X:0~1> <START_Y:0~1> <WIDTH:0~1> <HEIGHT:0~1>: set viewport area\n",_SET_,_VIEW_);
	fprintf(stderr,"\t\t\t%s %s (rgb|hsv) <R:0~1/H:0~360> <G:0~1/S:0~1> <B:0~1/V:0~1>: set background color\n",_SET_,_BACK_);
	fprintf(stderr,"\t\t\tset camera up xyz <X> <Y> <Z>: set camera upper direction\n");
	fprintf(stderr,"\t\t\tset camera from xyz <X> <Y> <Z>: set camera position\n");
	fprintf(stderr,"\t\t\tset camera to xyz <X> <Y> <Z>: set camera target\n");
	fprintf(stderr,"\t\t\tset light from xyz <X> <Y> <Z>: set light position\n");
	return 0;
}
int hd_handler(hd_t*self,const char*buffer,unsigned int length,int dump)
{	
	if(dump)write(dump,buffer,length);
	char*token[256]={0};
	int count=string_split_unsafe(memcpy(malloc(length),buffer,length),' ',token);
	if(!token[0]);
	else if(!strcmp(token[0],_SET_))
	{
		if(!token[1]);
		else if(!strcmp(token[1],_BACK_)&&token[2])parseColor(token+2,self->back);
		else if(!strcmp(token[1],_VIEW_)&&token[2])parseVport(token+2,self->view);
		else if(!strcmp(token[1],_BIAS_)&&token[2])self->eeg.bias=atof(token[2]);
		else if(!strcmp(token[1],_GAIN_)&&token[2])self->eeg.gain=atof(token[2]);
		else if(!strcmp(token[1],_LIGHT_))
		{
			if(!token[2]);
			else if(!strcmp(token[2],_FROM_)&&token[3])parsePosit(token+3,self->light.from);
			else if(!strcmp(token[2],_TO_)&&token[3])parsePosit(token+3,self->light.to);
			else if(!strcmp(token[2],_FOVY_)&&token[3])self->light.fovy=atof(token[3]);
		}
		else if(!strcmp(token[1],_CAMERA_))
		{
			if(!token[2]);
			else if(!strcmp(token[2],_FROM_)&&token[3])parsePosit(token+3,self->camera.from);
			else if(!strcmp(token[2],_TO_)&&token[3])parsePosit(token+3,self->camera.to);
			else if(!strcmp(token[2],_UP_)&&token[3])parsePosit(token+3,self->camera.up);
			hd_lookat(self,
					self->camera.from[0],self->camera.from[1],self->camera.from[2],
					self->camera.to[0],self->camera.to[1],self->camera.to[2],
					self->camera.up[0],self->camera.up[1],self->camera.up[2],dump);
		}
	}
	else if(!strcmp(token[0],_MAP_))
	{
		float	value[256]={0};
		register unsigned int index=0,length=self->eeg.itemLength;;
		for(index=0;index<length;++index)value[index]=token[index]?atof(token[index+1]):0;
		register unsigned int w,h,k;
		const register unsigned int W=self->texture.w,H=self->texture.h,K=length;
		register vec4_t*data=(vec4_t*)self->texture.cache;
		bzero(data,sizeof(vec4_t)*W*H);
		for(h=0;h<H;++h)
		{
			register float fy=h/(float)H;
			for(w=0;w<W;++w)
			{
				register float fx=w/(float)W;
				{
					register unsigned int index=h*W+w;
					register float hue=self->eeg.bias;
					for(k=0;k<K;++k)
					{
						register float tx=fx-(self->eeg.dict[self->eeg.order[k]].x*0.5+0.5);
						register float ty=fy-(self->eeg.dict[self->eeg.order[k]].y*0.5+0.5);
						hue-=self->eeg.gain*value[k]/expf(32*tx*tx+32*ty*ty);
					}
					data[index]=hsv2rgb(vec4_pack(hue,1,1,1));
				}
			}	
		}
	}
	return 0;
}
int hp_handler_help()
{
	fprintf(stderr,"message format:\n");
	fprintf(stderr,"\tset x [NUMBER]:	change place on x-axis\n");
	fprintf(stderr,"\tset y [NUMBER]:	change place on y-axis\n");
	fprintf(stderr,"\tset z [NUMBER]:	change place on z-axis\n");
	fprintf(stderr,"\tset [TARGET] ([SUBTARGET])... ([FORMAT]) [VALUES] ...:\n");
	fprintf(stderr,"\t\tchange configuration for rendering\n");
	fprintf(stderr,"\t\t\t%s %s xywh <START_X:0~1> <START_Y:0~1> <WIDTH:0~1> <HEIGHT:0~1>: set viewport area\n",_SET_,_VIEW_);
	fprintf(stderr,"\t\t\t%s %s (rgb|hsv) <R:0~1/H:0~360> <G:0~1/S:0~1> <B:0~1/V:0~1>: set background color\n",_SET_,_BACK_);
	fprintf(stderr,"\t\t\t%s %s %s xyz <X> <Y> <Z>: set camera upper direction\n",_SET_,_CAMERA_,_UP_);
	fprintf(stderr,"\t\t\t%s %s %s xyz <X> <Y> <Z>: set camera position\n",_SET_,_CAMERA_,_FROM_);
	fprintf(stderr,"\t\t\t%s %s %s xyz <X> <Y> <Z>: set camera target\n",_SET_,_CAMERA_,_TO_);
	fprintf(stderr,"\t\t\t%s %s %s xyz <X> <Y> <Z>: set light position\n",_SET_,_LIGHT_,_FROM_);
	fprintf(stderr,"\t%s %s [NUMBER:0~1] ([SUBTARGETs])... ([FORMAT]) [VALUEs] ...:\n",_SET_,_AXIS_);
	fprintf(stderr,"\t\tchange configuration for [NUMBER]'s axis\n");
	fprintf(stderr,"\t\t\t%s %s %s [NUMBER] %s\n",_SET_,_AXIS_,_RANGE_,_MIN_);
	fprintf(stderr,"\t\t\t\tchange maximum-range for the axis\n");
	fprintf(stderr,"\t\t\t%s %s %s [NUMBER] %s\n",_SET_,_AXIS_,_RANGE_,_MIN_);
	fprintf(stderr,"\t\t\t\tchange minimum-range for the axis\n");
	fprintf(stderr,"\t\t\t%s %s %s [NUMBER] %s\n",_SET_,_AXIS_,_SHAPE_,_SIZE_);
	fprintf(stderr,"\t\t\t\tchange size of the axis's line object\n");
	fprintf(stderr,"\t\t\t%s %s %s [NUMBER] %s\n",_SET_,_AXIS_,_SHAPE_,_AMBIENT_);
	fprintf(stderr,"\t\t\t\tchange ambient colour of the axis's line object\n");
	fprintf(stderr,"\t\t\t%s %s %s [NUMBER] %s\n",_SET_,_AXIS_,_SHAPE_,_DIFFUSE_);
	fprintf(stderr,"\t\t\t\tchange diffuse colour of the axis's line object\n");
	fprintf(stderr,"\t\t\t%s %s %s [NUMBER] %s\n",_SET_,_AXIS_,_LABEL_,_LABEL_);
	fprintf(stderr,"\t\t\t\tchange label on the axis\n");
	fprintf(stderr,"\t\t\t%s %s %s [NUMBER] %s\n",_SET_,_AXIS_,_LABEL_,_SIZE_);
	fprintf(stderr,"\t\t\t\tchange size of label on the axis-line\n");
	fprintf(stderr,"\t\t\t%s %s %s [NUMBER] %s\n",_SET_,_AXIS_,_LABEL_,_LIGHT_,_AMBIENT_);
	fprintf(stderr,"\t\t\t\tchange ambient colour for turning-on LCD on the axis-line\n");
	fprintf(stderr,"\t\t\t%s %s %s [NUMBER] %s\n",_SET_,_AXIS_,_LABEL_,_LIGHT_,_DIFFUSE_);
	fprintf(stderr,"\t\t\t\tchange diffuse colour for turning-on LCD on the axis-line\n");
	fprintf(stderr,"\t\t\t%s %s %s [NUMBER] %s\n",_SET_,_AXIS_,_LABEL_,_LIGHT_,_AMBIENT_);
	fprintf(stderr,"\t\t\t\tchange ambient colour for turning-off LCD on the axis-line\n");
	fprintf(stderr,"\t\t\t%s %s %s [NUMBER] %s\n",_SET_,_AXIS_,_LABEL_,_LIGHT_,_DIFFUSE_);
	fprintf(stderr,"\t\t\t\tchange diffuse colour for turning-off LCD on the axis-line\n");
	return 0;
}
int hp_handler(hp_t*self,const char*buffer,unsigned int length,int dump)
{
	char*token[64]={0};
	int count=string_split_unsafe(memcpy(malloc(length),buffer,length),' ',token);
	if(!token[0]);
	else if(!strcmp(token[0],_SET_))
	{
		if(!token[1]);
		else if(!strcmp(token[1],_BACK_)&&token[2])parseColor(token+2,self->back);
		else if(!strcmp(token[1],_VIEW_)&&token[2])parseVport(token+2,self->view);
		else if(!strcmp(token[1],_X_)&&token[2])self->value[0].dst=atof(token[2]);
		else if(!strcmp(token[1],_Y_)&&token[2])self->value[1].dst=atof(token[2]);
		else if(!strcmp(token[1],_Z_)&&token[2])self->value[2].dst=atof(token[2]);
		else if(!strcmp(token[1],_COMMON_))
		{
		//	if(!token[2]);
		//	else if(!strcmp(token[2],_FILL_)&&token[3])self->flag=atoi(token[3])?self->flag|hs_DRAW_FILL:self->flag&~hs_DRAW_FILL;
		}
		else if(!strcmp(token[1],_LIGHT_))
		{
			if(!token[2]);
			else if(!strcmp(token[2],_FROM_)&&token[3])parsePosit(token+3,self->light.from);
			else if(!strcmp(token[2],_TO_)&&token[3])parsePosit(token+3,self->light.to);
			else if(!strcmp(token[2],_FOVY_)&&token[3])self->light.fovy=atof(token[3]);
		}
		else if(!strcmp(token[1],_CAMERA_))
		{
			if(!token[2]);
			else if(!strcmp(token[2],_FROM_)&&token[3])parsePosit(token+3,self->camera.from);
			else if(!strcmp(token[2],_TO_)&&token[3])parsePosit(token+3,self->camera.to);
			else if(!strcmp(token[2],_UP_)&&token[3])parsePosit(token+3,self->camera.up);
			*((mat4_t*)self->pinhole.matrix)=mat4_look
			(
				self->camera.from[0],self->camera.from[1],self->camera.from[2],
				self->camera.to[0],self->camera.to[1],self->camera.to[2],
				self->camera.up[0],self->camera.up[1],self->camera.up[2]
			);
		}
		else if(!strcmp(token[1],_AXIS_))
		{
			if(!token[2]);
			else
			{
				register unsigned int index=atoi(token[2])%3;
				if(!token[3]);
				else if(!strcmp(token[3],_RANGE_))
				{
					if(!token[4]);
					else if(!strcmp(token[4],_MIN_)&&token[5])self->value[index].min=atof(token[5]);
					else if(!strcmp(token[4],_MAX_)&&token[5])self->value[index].max=atof(token[5]);
					hp_reshape(self,dump);
				}
				else if(!strcmp(token[3],_SHAPE_))
				{
					if(!token[4]);
					else if(!strcmp(token[4],_SIZE_)&&token[5])self->axis[index].scale=atof(token[5]);
					else if(!strcmp(token[4],_AMBIENT_)&&token[5])parseColor(token+5,self->axis[index].ambient);
					else if(!strcmp(token[4],_DIFFUSE_)&&token[5])parseColor(token+5,self->axis[index].diffuse);	
				}
				else if(!strcmp(token[3],_LABEL_))
				{
					if(!token[4]);
					else if(!strcmp(token[4],_SIZE_)&&token[5])self->label[index].scale=atof(token[5]);
					else if(!strcmp(token[4],_LABEL_))strcpy(self->label[index].label,token[5]?token[5]:"");
					else if(!strcmp(token[4],_LIGHT_))
					{
						if(!token[5]);
						else if(!strcmp(token[5],_AMBIENT_)&&token[5])parseColor(token+5,self->label[index].p.ambient);
						else if(!strcmp(token[5],_DIFFUSE_)&&token[5])parseColor(token+5,self->label[index].p.diffuse);
					}
					else if(!strcmp(token[4],_UNLIT_))
					{
						if(!token[5]);
						else if(!strcmp(token[5],_AMBIENT_)&&token[5])parseColor(token+5,self->label[index].n.ambient);
						else if(!strcmp(token[5],_DIFFUSE_)&&token[5])parseColor(token+5,self->label[index].n.diffuse);	
					}
				}
			}
		}
	}
}
