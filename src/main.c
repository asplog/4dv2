//
//  main.c
//  P4
//
//  Created by Kota Nakano on 16/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//
#include"gl.h"
#include"texture.h"
#include"shader.h"
#include"hs.h"
#include"hd.h"
#include"hp.h"
#include"sock.h"
#include<unistd.h>
#include<GL/freeglut.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
const char*_CPORT_="--cport";
const char*_DPORT_="--dport";
const char*_EPORT_="--eport";
const char*_WIDTH_="--width";
const int _WIDTH_DEFAULT_=960;
const char*_HEIGHT_="--height";
const int _HEIGHT_DEFAULT_=540;
const char*_FPS_="--fps";
const int _FPS_DEFAULT_=60;
const char*_FULLSCREEN_="--fullscreen";
const char*_VERBOSE_="--verbose";
const char*_HELP_="--help";
const char*_SIGNAL_DICT_="--dict";
const char*_SIGNAL_DICT_DEFAULT_="data/electrode_set_standard_cartesian.txt";
const char*_SIGNAL_ORDER_="--order";
const char*_SIGNAL_ORDER_DEFAULT_="AF3 F7 F3 FC5 T7 P7 O1 O2 P8 T8 FC6 F4 F8 AF4";
const char*_HEAD_MESH_="--head";
const char*_HEAD_MESH_DEFAULT_="data/head.xml";
const char*_FACE_MESH_="--face";
const char*_FACE_MESH_DEFAULT_="data/face.xml";
const char*_VERTEX_SHADER_="--vsh";
const char*_VERTEX_SHADER_DEFAULT_="data/main.vsh";
const char*_FRAGMENT_SHADER_="--fsh";
const char*_FRAGMENT_SHADER_DEFAULT_="data/main.fsh";
shader_t shader;
hs_t	hs;
hd_t	hd;
hp_t	hp;
void init(int dump,const char*vshp,const char*fshp)
{
	int	fd[2]={open(vshp,O_RDONLY),open(fshp,O_RDONLY)};
	const char cache[2][16384]={shader_source_vertex_default,shader_source_fragment_default};
	glewInit();
	
	if(fd[0]<0||1>read(fd[0],cache[0],sizeof(cache[0]))&&dump)write(dump,"FSH error",10);
	if(fd[1]<1||1>read(fd[1],cache[1],sizeof(cache[1]))&&dump)write(dump,"VSH error",10);

	shader_init(&shader,dump);
	shader_load(&shader,shader_type_vertex,		cache[0],0,dump);
	shader_load(&shader,shader_type_fragment,	cache[1],0,dump);
	shader_link(&shader,0);
	shader_bind(&shader,0);
	
	shader_vertex=shader_getAttribLoc(&shader,"aVertex",0);
	shader_normal=shader_getAttribLoc(&shader,"aNormal",0);
	shader_ambient=shader_getAttribLoc(&shader,"aAmbient",0);
	shader_diffuse=shader_getAttribLoc(&shader,"aDiffuse",0);
	
	glEnableVertexAttribArray(shader_vertex);
	glEnableVertexAttribArray(shader_normal);
	glEnableVertexAttribArray(shader_ambient);
	glEnableVertexAttribArray(shader_diffuse);

	shader_projection=shader_getUniformLoc(&shader,"uProjection",0);
	shader_perspective=shader_getUniformLoc(&shader,"uPerspective",0);
	shader_modelview=shader_getUniformLoc(&shader,"uModelview",0);
	shader_reshape=shader_getUniformLoc(&shader,"uReshape",0);
	
	shader_light_position=shader_getUniformLoc(&shader,"uLight",0);
	shader_camera_position=shader_getUniformLoc(&shader,"uCamera",0);
	
	shader_ambient_const=shader_getUniformLoc(&shader,"uAmbient",dump);
	shader_ambient_drive=shader_getUniformLoc(&shader,"sAmbient",dump);
	shader_ambient_image=shader_getUniformLoc(&shader,"iAmbient",dump);
	shader_ambient_coord=shader_getUniformLoc(&shader,"mAmbient",dump);
	
	shader_diffuse_const=shader_getUniformLoc(&shader,"uDiffuse",dump);
	shader_diffuse_drive=shader_getUniformLoc(&shader,"sDiffuse",dump);
	shader_diffuse_image=shader_getUniformLoc(&shader,"iDiffuse",dump);
	shader_diffuse_coord=shader_getUniformLoc(&shader,"mDiffuse",dump);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
}
void redp(int arg)
{
	int dump=0;
	hs_update(&hs,dump);
	hd_update(&hd,dump);
	hp_update(&hp,dump);
	glutPostRedisplay();
}
void disp()
{
	int dump=0;
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	hs_draw(&hs,dump);
	hd_draw(&hd,dump);
	hp_draw(&hp,dump);
	glutSwapBuffers();
	glutTimerFunc(1000/60,redp,0);
}
void keyf(int k,int t,int x,int y)
{
	switch(k)
	{
		case 't':hs.theta.dst+=120;break;
		case 'T':hs.theta.dst-=120;break;
		case 'a':hs.alpha.dst+=90;break;
		case 'A':hs.alpha.dst-=90;break;
	}
}
void keys(int k,int x,int y)
{
	switch(k)
	{
		case GLUT_KEY_UP:	hp.value[1].dst+=1.0f;break;
		case GLUT_KEY_DOWN:	hp.value[1].dst-=1.0f;break;
		case GLUT_KEY_LEFT:	hp.value[0].dst-=1.0f;break;
		case GLUT_KEY_RIGHT:	hp.value[0].dst+=1.0f;break;
	}
}
void resize(int w,int h)
{
	hs_root(&hs,w,h,0);
	hd_root(&hd,w,h,0);
	hp_root(&hp,w,h,0);
}
void term()
{

}
int usage(const char*type)
{
	fprintf(stderr,"Usage: 4d\n");
	if(!type)
	{
		fprintf(stderr,"\t%s <NUMBER>: recieve udp for hypersphere via port <NUMBER>\n",_CPORT_);
		fprintf(stderr,"\t%s <NUMBER>: recieve udp for eeg head via port <NUMBER>\n",_DPORT_);
		fprintf(stderr,"\t%s <FILE>: eeg positioning dictionary, default: %s\n",_SIGNAL_DICT_,_SIGNAL_DICT_DEFAULT_);
		fprintf(stderr,"\t%s <STRING>: eeg positioning order, default: %s\n",_SIGNAL_ORDER_,_SIGNAL_ORDER_DEFAULT_);
		fprintf(stderr,"\t%s <FILE>: load face mesh file, default: %s\n",_FACE_MESH_,_FACE_MESH_DEFAULT_);
		fprintf(stderr,"\t%s <FILE>: load head mesh file, default: %s\n",_HEAD_MESH_,_HEAD_MESH_DEFAULT_);
		fprintf(stderr,"\t%s <FILE>: load vertex shader file, default: %s\n",_VERTEX_SHADER_,_VERTEX_SHADER_DEFAULT_);
		fprintf(stderr,"\t%s <FILE>: load fragment shader file default: %s\n",_FRAGMENT_SHADER_,_FRAGMENT_SHADER_DEFAULT_);
		fprintf(stderr,"\t%s: verbose mode\n",_VERBOSE_);
		fprintf(stderr,"\t%s: display this message\n",_HELP_);
	}
	else if(!strcmp(type,_CPORT_))hs_handler_help();
	else if(!strcmp(type,_DPORT_))hd_handler_help();
	else if(!strcmp(type,_EPORT_))hp_handler_help();
	else fprintf(stderr,"\t%s: invalid args.\n\tplease input just %s\n",_HELP_,_HELP_);
	return 0;
}
int main(int argc,char**argv)
{
	int dump=0,k;
	int cdrive=0;
	int ddrive=0;
	int edrive=0;
	int work=1;
	int W=_WIDTH_DEFAULT_;
	int H=_HEIGHT_DEFAULT_;
	int FPS=_FPS_DEFAULT_;
	const char*SIGNAL_DICT=_SIGNAL_DICT_DEFAULT_;
	const char*SIGNAL_ORDER=_SIGNAL_ORDER_DEFAULT_;
	const char*HEAD_MESH=_HEAD_MESH_DEFAULT_;
	const char*FACE_MESH=_FACE_MESH_DEFAULT_;
	const char*VERTEX_SHADER=_VERTEX_SHADER_DEFAULT_;
	const char*FRAGMENT_SHADER=_FRAGMENT_SHADER_DEFAULT_;

	for(k=1;k<argc;++k)
	{
		if(!argv[k]);
		else if(!strcmp(argv[k],_CPORT_)&&sscanf(argv[k+1],"%d",&cdrive))++k;
		else if(!strcmp(argv[k],_DPORT_)&&sscanf(argv[k+1],"%d",&ddrive))++k;
		else if(!strcmp(argv[k],_EPORT_)&&sscanf(argv[k+1],"%d",&edrive))++k;
		else if(!strcmp(argv[k],_WIDTH_)&&sscanf(argv[k+1],"%d",&W))++k;
		else if(!strcmp(argv[k],_HEIGHT_)&&sscanf(argv[k+1],"%d",&H))++k;
		else if(!strcmp(argv[k],_FPS_)&&sscanf(argv[k+1],"%d",&FPS))++k;
		else if(!strcmp(argv[k],_SIGNAL_DICT_)&&k+1<argc&&*argv[k+1]^'-')SIGNAL_DICT=argv[++k];
		else if(!strcmp(argv[k],_SIGNAL_ORDER_)&&k+1<argc&&*argv[k+1]^'-')SIGNAL_ORDER=argv[++k];
		else if(!strcmp(argv[k],_FACE_MESH_)&&k+1<argc&&*argv[k+1]^'-')FACE_MESH=argv[++k];
		else if(!strcmp(argv[k],_HEAD_MESH_)&&k+1<argc&&*argv[k+1]^'-')HEAD_MESH=argv[++k];
		else if(!strcmp(argv[k],_VERTEX_SHADER_)&&k+1<argc&&*argv[k+1]^'-')VERTEX_SHADER=argv[++k];
		else if(!strcmp(argv[k],_FRAGMENT_SHADER_)&&k+1<argc&&*argv[k+1]^'-')FRAGMENT_SHADER=argv[++k];
		else if(!strcmp(argv[k],_VERBOSE_))dump=2;
		else if(!strcmp(argv[k],_HELP_))work=usage(k+1<argc?argv[k+1]:0);
	}
	if(work)
	{
		glutInit(&argc,argv);
		glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE|GLUT_STENCIL);
		glutInitWindowSize(W,H);
		glutCreateWindow(argv[0]);
		glutDisplayFunc(disp);
		glutKeyboardFunc(keyf);
		glutSpecialFunc(keys);
		glutReshapeFunc(resize);

		init(dump,VERTEX_SHADER,FRAGMENT_SHADER);	

		hs_init(&hs,1,W,H,dump);
		hs_back(&hs,0,0,0,1,dump);
		hs_view(&hs,0,0.2,0.8,0.8,dump);	

		hd_init(&hd,2,W,H,FACE_MESH,HEAD_MESH,dump);
		hd_back(&hd,0,0,0,1,dump);
		hd_view(&hd,0.7,0,0.3,0.3,dump);	
		hd_eeg_dict(&hd,SIGNAL_DICT,dump);
		hd_eeg_order(&hd,SIGNAL_ORDER,dump);
	
		hp_init(&hp,3,W,H,dump);
		//hp_back(&hp,0.0,0.0,0.0,1,dump);
		hp_view(&hp,0.7,0.3,0.3,0.3,dump);
		hp_view(&hp,0,0,1,1,dump);
		hp_lookat(&hp, 0, 0, 1.0, 0, 0, 0,0,1,0,dump);

		if(cdrive)sock_gen(cdrive,&hs,hs_handler,dump);
		if(ddrive)sock_gen(ddrive,&hd,hd_handler,dump);
		if(edrive)sock_gen(edrive,&hp,hp_handler,dump);
		glutMainLoop();
		term();
	}
	return 0;
}

