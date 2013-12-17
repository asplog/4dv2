//
//  cg.h
//  P4
//
//  Created by Kota Nakano on 16/11/13.
//  Copyright (c) 2013 Kota Nakano. All rights reserved.
//

#ifndef P4_cg_h
#define P4_cg_h
typedef struct
{
	union{float x;float r;float h;};
}
vec1_t;
typedef struct
{
	union{float x;float r;float h;};
	union{float y;float g;float s;};
}
vec2_t;
typedef struct
{
	union{float x;float r;float h;};
	union{float y;float g;float s;};
	union{float z;float b;float v;};
}
vec3_t;
typedef struct
{
	union{float x;float r;float h;};
	union{float y;float g;float s;};
	union{float z;float b;float v;};
	union{float w;float a;};
}
vec4_t;
inline vec1_t vec1_pack(const float);
inline vec2_t vec2_pack(const float,const float);
inline vec3_t vec3_pack(const float,const float,const float);
inline vec4_t vec4_pack(const float,const float,const float,const float);

vec1_t vec1_copy(const float*);
vec2_t vec2_copy(const float*);
vec3_t vec3_copy(const float*);
vec4_t vec4_copy(const float*);

vec1_t vec1_shift(const vec1_t,int);
vec2_t vec2_shift(const vec2_t,int);
vec3_t vec3_shift(const vec3_t,int);
vec4_t vec4_shift(const vec4_t,int);

inline vec1_t vec1_vec1(const vec1_t);
inline vec1_t vec1_vec2(const vec2_t);
inline vec1_t vec1_vec3(const vec3_t);
inline vec1_t vec1_vec4(const vec4_t);

inline vec2_t vec2_vec1(const vec1_t,const float);
inline vec2_t vec2_vec2(const vec2_t);
inline vec2_t vec2_vec3(const vec3_t);
inline vec2_t vec2_vec4(const vec4_t);

inline vec3_t vec3_vec1(const vec1_t,const float,const float);
inline vec3_t vec3_vec2(const vec2_t,const float);
inline vec3_t vec3_vec3(const vec3_t);
inline vec3_t vec3_vec4(const vec4_t);

inline vec4_t vec4_vec1(const vec1_t,const float,const float,const float);
inline vec4_t vec4_vec2(const vec2_t,const float,const float);
inline vec4_t vec4_vec3(const vec3_t,const float);
inline vec4_t vec4_vec4(const vec4_t);

vec1_t vec1_add_scalar(const vec1_t,const float);
vec2_t vec2_add_scalar(const vec2_t,const float);
vec3_t vec3_add_scalar(const vec3_t,const float);
vec4_t vec4_add_scalar(const vec4_t,const float);

vec1_t scalar_add_vec1(const float,const vec1_t);
vec2_t scalar_add_vec2(const float,const vec2_t);
vec3_t scalar_add_vec3(const float,const vec3_t);
vec4_t scalar_add_vec4(const float,const vec4_t);

vec1_t vec1_mul_scalar(const vec1_t,const float);
vec2_t vec2_mul_scalar(const vec2_t,const float);
vec3_t vec3_mul_scalar(const vec3_t,const float);
vec4_t vec4_mul_scalar(const vec4_t,const float);

vec1_t scalar_mul_vec1(const float,const vec1_t);
vec2_t scalar_mul_vec2(const float,const vec2_t);
vec3_t scalar_mul_vec3(const float,const vec3_t);
vec4_t scalar_mul_vec4(const float,const vec4_t);

vec1_t vec1_add_vec1(const vec1_t,const vec1_t);
vec2_t vec2_add_vec2(const vec2_t,const vec2_t);
vec3_t vec3_add_vec3(const vec3_t,const vec3_t);
vec4_t vec4_add_vec4(const vec4_t,const vec4_t);

vec1_t vec1_sub_vec1(const vec1_t,const vec1_t);
vec2_t vec2_sub_vec2(const vec2_t,const vec2_t);
vec3_t vec3_sub_vec3(const vec3_t,const vec3_t);
vec4_t vec4_sub_vec4(const vec4_t,const vec4_t);

vec1_t vec1_mul_vec1(const vec1_t,const vec1_t);
vec2_t vec2_mul_vec2(const vec2_t,const vec2_t);
vec3_t vec3_mul_vec3(const vec3_t,const vec3_t);
vec4_t vec4_mul_vec4(const vec4_t,const vec4_t);

float vec1_mag(const vec1_t);
float vec2_mag(const vec2_t);
float vec3_mag(const vec3_t);
float vec4_mag(const vec4_t);

inline float vec1_dot(const vec1_t,const vec1_t);
inline float vec2_dot(const vec2_t,const vec2_t);
inline float vec3_dot(const vec3_t,const vec3_t);
inline float vec4_dot(const vec4_t,const vec4_t);

vec1_t vec1_normalize(const vec1_t);
vec2_t vec2_normalize(const vec2_t);
vec3_t vec3_normalize(const vec3_t);
vec4_t vec4_normalize(const vec4_t);

inline vec1_t vec1_cross(const vec1_t,const vec1_t);
inline vec2_t vec2_cross(const vec2_t,const vec2_t);
inline vec3_t vec3_cross(const vec3_t,const vec3_t);
inline vec4_t vec4_cross(const vec4_t,const vec4_t);

inline int scalar_dump(const float,int);
inline int vec1_dump(const vec1_t,int);
inline  int vec2_dump(const vec2_t,int);
inline int vec3_dump(const vec3_t,int);
inline int vec4_dump(const vec4_t,int);

#endif
