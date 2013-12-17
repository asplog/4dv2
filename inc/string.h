//
//  string.h
//  P4
//
//  Created by Kota Nakano on 18/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//

#ifndef P4_string_h
#define P4_string_h
int string_split(char*,char,char**,unsigned int);
int string_split_unsafe(char*p,char d,char**r);
int string_split_block_unsafe(char*p,char,char,char**r);
int string_unquote(const char*,char*);
#endif
