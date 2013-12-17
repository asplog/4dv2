#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include"vector.h"
#include"sprite.h"
#include"gl.h"
int sprite_lcd_init(sprite_t*self,int dump)
{
	{
		bzero(self,sizeof(sprite_t));
		sprite_init(self,dump);
	}
	{
		struct
		{
			vec4_t	ambient[9][7][5];
			vec4_t	diffuse[9][7][5];
			vec4_t	normal[9][7][5];
			vec4_t	vertex[9][7][5];
		}
		buffer={0};
		register const float r=0.1f;
		register unsigned int h,H;
		#pragma omp parallel for
		for(h=0,H=7;h<H;++h)
		{
			register unsigned int w,W;
			for(w=0,W=5;w<W;++w)
			{
				register const float nr=1/sqrtf(3.0);
				register const float cx=  0.1f*w-0.2f;
				register const float cy=-(0.1f*h-0.3f);
				register const float cz=  0.0f;
				
				buffer.vertex[0][h][w].x=cx;
				buffer.vertex[0][h][w].y=cy;
				buffer.vertex[0][h][w].z=cz;
				buffer.vertex[0][h][w].w=1;
				
				buffer.normal[0][h][w].x=0;
				buffer.normal[0][h][w].y=0;
				buffer.normal[0][h][w].z=0;
				buffer.normal[0][h][w].w=1;
				
				buffer.ambient[0][h][w].r=0;
				buffer.ambient[0][h][w].g=0;
				buffer.ambient[0][h][w].b=0;
				buffer.ambient[0][h][w].a=1;
				
				buffer.diffuse[0][h][w].r=0;
				buffer.diffuse[0][h][w].g=0;
				buffer.diffuse[0][h][w].b=0;
				buffer.diffuse[0][h][w].a=1;
			
				register unsigned int v,V;
				for(v=0,V=8;v<V;++v)
				{
					buffer.vertex[v+1][h][w].x=cx+((v&0x1)?-r:r);
					buffer.vertex[v+1][h][w].y=cy+((v&0x2)?-r:r);
					buffer.vertex[v+1][h][w].z=cz+((v&0x4)?-r:r);
					buffer.vertex[v+1][h][w].w=1;
					
					buffer.normal[v+1][h][w].x=((v&0x1)?-nr:nr);
					buffer.normal[v+1][h][w].y=((v&0x2)?-nr:nr);
					buffer.normal[v+1][h][w].z=((v&0x4)?-nr:nr);
					buffer.normal[v+1][h][w].w=1;

				}
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER,self->buffer[0]);
		glBufferData(GL_ARRAY_BUFFER,sizeof(buffer),&buffer,GL_DYNAMIC_DRAW);
		self->vertex.stride=sizeof(vec4_t),self->vertex.offset=sizeof(vec4_t)*5*7*9*3;
		self->normal.stride=sizeof(vec4_t),self->normal.offset=sizeof(vec4_t)*5*7*9*2;
		self->ambient.stride=sizeof(vec4_t),self->ambient.offset=sizeof(vec4_t)*5*7*9*1;
		self->diffuse.stride=sizeof(vec4_t),self->diffuse.offset=sizeof(vec4_t)*5*7*9*0;
		
	}
	{
		unsigned int indices[7][5][12][3]={0};
		register unsigned int h,H;
		#pragma omp parallel for
		for(h=0,H=7;h<H;++h)
		{
			register unsigned int w,W;
			for(w=0,W=5;w<W;++w)
			{
				register unsigned int base=h*W+w;
				
				indices[h][w][0x0][0]=base;
				indices[h][w][0x0][1]=base+35;
				indices[h][w][0x0][2]=base+105;
				
				indices[h][w][0x1][0]=base;
				indices[h][w][0x1][1]=base+105;
				indices[h][w][0x1][2]=base+140;
				
				indices[h][w][0x2][0]=base;
				indices[h][w][0x2][1]=base+140;
				indices[h][w][0x2][2]=base+70;
				
				indices[h][w][0x3][0]=base;
				indices[h][w][0x3][1]=base+70;
				indices[h][w][0x3][2]=base+35;
				
				
				indices[h][w][0x4][0]=base;
				indices[h][w][0x4][1]=base+35;
				indices[h][w][0x4][2]=base+175;
				
				indices[h][w][0x5][0]=base;
				indices[h][w][0x5][1]=base+105;
				indices[h][w][0x5][2]=base+245;
				
				indices[h][w][0x6][0]=base;
				indices[h][w][0x6][1]=base+140;
				indices[h][w][0x6][2]=base+280;
				
				indices[h][w][0x7][0]=base;
				indices[h][w][0x7][1]=base+70;
				indices[h][w][0x7][2]=base+210;
				
				
				indices[h][w][0x8][0]=base;
				indices[h][w][0x8][1]=base+175;
				indices[h][w][0x8][2]=base+210;
				
				indices[h][w][0x9][0]=base;
				indices[h][w][0x9][1]=base+210;
				indices[h][w][0x9][2]=base+280;
				
				indices[h][w][0xa][0]=base;
				indices[h][w][0xa][1]=base+280;
				indices[h][w][0xa][2]=base+245;
				
				indices[h][w][0xb][0]=base;
				indices[h][w][0xb][1]=base+245;
				indices[h][w][0xb][2]=base+175;
					
			}
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,self->buffer[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
		self->stride=GL_UNSIGNED_INT;
		self->length=(GLvoid*)(sizeof(indices)/sizeof(unsigned int));
	}

	
	
	return 0;
}
int sprite_lcd_light(sprite_t*self,unsigned char c,float*a0,float*d0,float*a1,float*d1,int dump)
{
	
	//		LSB
	//
	//	LSB	00000  -----  00000  -----  0----  -000-  0000-
	//		0----  0--0-  ----0  -----  0----  0----  0---0
	//		0000-  0--0-  ---0-  -----  0----  0----  0---0
	//		----0  -00--  --0--  -----  0----  0----  0---0
	//		----0  0--0-  --0--  -----  0----  0----  0---0
	//		0---0  0--0-  --0--  -----  0---0  0----  0---0
	//	MSB	-000-  -----  --0--  -----  00000  -000-  0000-
	//
	//		MSB
	
	unsigned int retVal=1;
	uint64_t digits=0;
	switch(c)
	{
		//				MSB				LSB
		case 'A':digits=0x0011111f1111110e;break;
		case 'B':digits=0x000f11110f11110f;break;
		case 'C':digits=0x000e11010101110e;break;
		case 'D':digits=0x000f11111111110f;break;
		case 'E':digits=0x001f01010f01011f;break;
		case 'F':digits=0x000101010701011f;break;
		case 'G':digits=0x000e11190101110e;break;
		case 'H':digits=0x001111111f111111;break;
		case 'I':digits=0x001f04040404041f;break;
		case 'J':digits=0x000e11111010101f;break;
		case 'K':digits=0x0011090503050911;break;
		case 'L':digits=0x001f110101010101;break;
		case 'M':digits=0x0011111111151b11;break;
		case 'N':digits=0x0019191515131311;break;
		case 'O':digits=0x000e11111111110e;break;
		case 'P':digits=0x000101010f11110f;break;
		case 'Q':digits=0x000e19151111110e;break;
		case 'R':digits=0x001109050f11110f;break;
		case 'S':digits=0x000e11100e01110e;break;
		case 'T':digits=0x000404040404041f;break;
		case 'U':digits=0x000e111111111111;break;
		case 'V':digits=0x00040a1111111111;break;
		case 'W':digits=0x001b151511111111;break;
		case 'X':digits=0x0011110a040a1111;break;
		case 'Y':digits=0x00040404040a1111;break;
		case 'Z':digits=0x001f01020408101f;break;
			
		case 'a':digits=0x000e090e08070000;break;
		case 'b':digits=0x0007090907010101;break;
		case 'c':digits=0x0006090109060000;break;
		case 'd':digits=0x000e09090e080808;break;
		case 'e':digits=0x000e010f09060000;break;
		case 'f':digits=0x00020202020f020c;break;
		case 'g':digits=0x0007080e09090e00;break;
		case 'h':digits=0x0009090907010101;break;
		case 'i':digits=0x0004040404040004;break;
		case 'j':digits=0x0003040404040004;break;
		case 'k':digits=0x0000090503050900;break;
		case 'l':digits=0x0004040404040404;break;
		case 'm':digits=0x0000151515150f00;break;
		case 'n':digits=0x0000090909090700;break;
		case 'o':digits=0x0000060909090600;break;
		case 'p':digits=0x0001010709090700;break;
		case 'q':digits=0x0008080e09090e00;break;
		case 'r':digits=0x0001010101010e00;break;
		case 's':digits=0x0006090402090600;break;
		case 't':digits=0x000e0202020f0202;break;
		case 'u':digits=0x00000e0909090900;break;
		case 'v':digits=0x0000040a11111100;break;
		case 'w':digits=0x00000e1515111100;break;
		case 'x':digits=0x0000090906090900;break;
		case 'y':digits=0x000007080e090900;break;
		case 'z':digits=0x00000f0102040f00;break;
			
		case '0':digits=0x000e11131519110e;break;
		case '1':digits=0x0010101010141810;break;
		case '2':digits=0x001f02040810110e;break;
		case '3':digits=0x000e11100c10110e;break;
		case '4':digits=0x0008081f090a0c08;break;
		case '5':digits=0x000e1110100f011f;break;
		case '6':digits=0x000e11110f01020c;break;
		case '7':digits=0x000404040408101f;break;
		case '8':digits=0x000e11110e11110e;break;
		case '9':digits=0x000e11101e11110e;break;
			
		case '.':digits=0x0004000000000000;break;
		case ',':digits=0x0002040600000000;break;
			
		case '?':digits=0x000400040811110e;break;
		case '!':digits=0x0004040404040004;break;
		case ':':digits=0x0000000200020000;break;
			
		case '+':digits=0x000004041f040400;break;
		case '-':digits=0x000000001f000000;break;
		case '*':digits=0x000040150e150400;break;
		case '/':digits=0x0000010204081000;break;
			
		case '"':digits=0x0000000000000c0c;break;
		case '#':digits=0x000a0a1f0a1f0a0a;break;
			
		case '>':digits=0x0002040810080402;break;
		case '=':digits=0x0000001f001f0000;break;
		case '<':digits=0x0008040201020408;break;
			
		case ' ':digits=0x0000000000000000;break;
			
		default:
			retVal=0;
			break;
	}
	if(retVal)
	{
		vec4_t diffuse[7][5]={0};
		vec4_t ambient[7][5]={0};
		register unsigned int h,H;
		#pragma omp parallel for
		for(h=0,H=7;h<H;++h)
		{
			register unsigned int w,W;
			for(w=0,W=5;w<W;++w)
			{
				ambient[h][w]=*(vec4_t*)(digits&1?a1:a0);
				diffuse[h][w]=*(vec4_t*)(digits&1?d1:d0);
				digits=digits>>1;
			}
			digits=digits>>3;//8bits align
		}
		glBindBuffer(GL_ARRAY_BUFFER,self->buffer[0]);
		glBufferSubData(GL_ARRAY_BUFFER,self->ambient.offset,sizeof(ambient),&ambient);
		glBufferSubData(GL_ARRAY_BUFFER,self->diffuse.offset,sizeof(diffuse),&diffuse);
	}

	return retVal;
}
