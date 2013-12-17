//
//  string.c
//  P4
//
//  Created by Kota Nakano on 18/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//
#include"string.h"
int string_split(char*p,char d,char**r,unsigned int length)
{
	register int result=1;
	*r++=p;
	while(*p++)
	{
		if(*p==d)
		{
			*p=0;
			*r++=++p;
			if(++result<length)continue;
			else break;
		}
	}
	return result;
}
int string_split_block_unsafe(register char*p,register char b,register char e,char**r)
{
	register int result=0;
	register unsigned int count=0;
LOOP:
	if(!*p)goto DONE;
	else if(*p==b&&!count++){*p=0;*r++=p+1;}
	else if(*p==e&&!--count){*p=0;++result;}
	++p;
	goto LOOP;
DONE:
	if(count)result=-1;
	return result;
}
int string_unquote(register const char*s,register char*d)
{
	register int result=0;
	register int state=0;
	do switch(*s)
	{
		case '\\':
			if(state)
			switch(s[1])
			{
				case 0:break;
				case '\\':d[result++]='\\';++s;break;
				case 't':d[result++]='\t';++s;break;
				case '"':d[result++]='"';++s;break;
				default:
					 if('0'<=s[1]&&s[1]<='9')
					 {
						register unsigned char value=s[1]-'0';
						d[result++]=value;
					 }
					 else d[result++]='\\';
						++s;
			}
			++s;
			break;
		case ' ':
			if(state)d[result++]=*s;
			++s;
			break;
		case '"':
			state=!state;
			++s;
			break;
		default:
			d[result++]=*s++;
	}
	while(*s);
	d[result]=0;
	return result;
}
int string_split_unsafe(register char*p,register char d,register char**r)
{
	
	register int result=0;
	/*
	do if(*p==d)
	{
		while(*p==d)*p++=0;
		r[result++]=p;
	}
	while(*++p);
	*/
	//while(*++p==d);
	*r++=p,++result;
	while(*p++)
		if(*p==d)*p=0,*r++=++p,++result;
	return result;
	/*
	*r++=p;
LOOP:
	goto*(!*++p?&&DONE:*p^d?&&LOOP:&&NEXT);
NEXT:
	*p=0;
	*r++=++p,++result;
	goto LOOP;
DONE:
	return result;
	*/
}
