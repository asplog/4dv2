//
//  mesh.c
//  P4
//
//  Created by Kota Nakano on 18/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//
#include<libxml/parser.h>
#include<stdlib.h>
#include"gl.h"
#include"sprite.h"
int sprite_mesh_init(sprite_t*self,const char*path,float*x,float*X,float*y,float*Y,float*z,float*Z,int dump)
{
	char buff[256];
	register int result=0;
	vertex_t*vbuf;
	unsigned int vlen;
	unsigned int*ibuf;
	unsigned int ilen;
	xmlDocPtr doc;
	doc=xmlParseFile(path);
	if(doc)
	{
		xmlNodePtr root=xmlDocGetRootElement(doc);
		while(root)
		{
			if(!xmlStrcmp(root->name,(xmlChar*)"mesh"))
			{
				xmlNodePtr mesh=root->xmlChildrenNode;
				while(mesh)
				{
					if(!xmlStrcmp(mesh->name,(xmlChar*)"submeshes"))
					{
						xmlNodePtr submeshes=mesh->xmlChildrenNode;
						while(submeshes)
						{
							if(!xmlStrcmp(submeshes->name,(xmlChar*)"submesh"))
							{
								xmlNodePtr submesh=submeshes->xmlChildrenNode;
								while(submesh)
								{
									if(!xmlStrcmp(submesh->name,(xmlChar*)"faces"))
									{
										register unsigned int index=0;
										xmlNodePtr faces=submesh->xmlChildrenNode;
										
										
										if((ilen=3*atoi((char*)xmlGetProp(submesh,(xmlChar*)"count")))&&(ibuf=(unsigned int*)malloc(ilen*sizeof(GLuint))))
											while(faces)
											{
												if(!xmlStrcmp(faces->name,(xmlChar*)"face"))
												{
													ibuf[index++]=atoi((char*)xmlGetProp(faces,(xmlChar*)"v1"));
													ibuf[index++]=atoi((char*)xmlGetProp(faces,(xmlChar*)"v2"));
													ibuf[index++]=atoi((char*)xmlGetProp(faces,(xmlChar*)"v3"));
												}
												faces=faces->next;
											}
									}
									if(!xmlStrcmp(submesh->name,(xmlChar*)"geometry"))
									{
										register unsigned int index=0;
										xmlNodePtr geometry=submesh->xmlChildrenNode;
										if((vlen=atoi((char*)xmlGetProp(submesh,(xmlChar*)"vertexcount")))&&(vbuf=(vertex_t*)malloc(vlen*sizeof(vertex_t))))
											while(geometry)
											{
												if(!xmlStrcmp(geometry->name,(xmlChar*)"vertexbuffer"))
												{
													xmlNodePtr vertexbuffer=geometry->xmlChildrenNode;
													while(vertexbuffer)
													{
														if(!xmlStrcmp(vertexbuffer->name,(xmlChar*)"vertex"))
														{
															xmlNodePtr vertex=vertexbuffer->xmlChildrenNode;
															
															vbuf[index].ambient.r=0.5;
															vbuf[index].ambient.g=0.5;
															vbuf[index].ambient.b=0.5;
															vbuf[index].ambient.a=1;
															
															vbuf[index].diffuse.r=0.5;
															vbuf[index].diffuse.g=0.5;
															vbuf[index].diffuse.b=0.5;
															vbuf[index].diffuse.a=0;
															
															while(vertex)
															{
																if(!xmlStrcmp(vertex->name,(xmlChar*)"position"))
																{
																	register float tx=atof((char*)xmlGetProp(vertex,(xmlChar*)"x"));
																	register float ty=atof((char*)xmlGetProp(vertex,(xmlChar*)"y"));
																	register float tz=atof((char*)xmlGetProp(vertex,(xmlChar*)"z"));
																	vbuf[index].vertex.x=tx;if(x&&tx<*x)*x=tx;if(X&&*X<tx)*X=tx;
																	vbuf[index].vertex.y=ty;if(y&&ty<*y)*y=ty;if(Y&&*Y<ty)*Y=ty;
																	vbuf[index].vertex.z=tz;if(z&&tz<*z)*z=tz;if(Z&&*Z<tz)*Z=tz;
																	vbuf[index].vertex.w=1;
																	
																	
																}
																if(!xmlStrcmp(vertex->name,(xmlChar*)"normal"))
																{
																	vbuf[index].normal.x=atof((char*)xmlGetProp(vertex,(xmlChar*)"x"));
																	vbuf[index].normal.y=atof((char*)xmlGetProp(vertex,(xmlChar*)"y"));
																	vbuf[index].normal.z=atof((char*)xmlGetProp(vertex,(xmlChar*)"z"));
																	vbuf[index].normal.w=1;
																}
																vertex=vertex->next;
															}
															
															++index;
														}
														vertexbuffer=vertexbuffer->next;
													}
												}
												geometry=geometry->next;
											}
									}
									submesh=submesh->next;
								}
							}
							submeshes=submeshes->next;
						}
					}
					mesh=mesh->next;
				}
			}
			root=root->next;
		}
		xmlFreeDoc(doc);
	}
	/*
	if(dump)
		for(int i=0;i<vlen;++i)
			write(dump,buff,sprintf(buff,"%f\n",vbuf[i].x));
			fprintf(stderr,"%f\n",vBuf.buffer.fp[i]);
	if(dump)
		for(int i=0;i<iBuf.length;++i)
			fprintf(stderr,"%d\n",iBuf.buffer.lp[i]);
	*/
	glGenBuffers(2,self->buffer);
	
	glBindBuffer(GL_ARRAY_BUFFER,self->buffer[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_t)*vlen,vbuf,GL_STATIC_DRAW);
	self->vertex.stride=sizeof(vertex_t),self->vertex.offset=sizeof(vec4_t)*0;
	self->normal.stride=sizeof(vertex_t),self->normal.offset=sizeof(vec4_t)*1;
	self->ambient.stride=sizeof(vertex_t),self->ambient.offset=sizeof(vec4_t)*2;
	self->diffuse.stride=sizeof(vertex_t),self->diffuse.offset=sizeof(vec4_t)*3;
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->buffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*ilen,ibuf,GL_STATIC_DRAW);
	self->stride=GL_UNSIGNED_INT;
	self->length=ilen;
	
	free(ibuf);
	free(vbuf);

	return result;
}
