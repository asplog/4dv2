uniform	vec4	uLight;
uniform	vec4	uCamera;
uniform	float	uFog;

uniform	int	sAmbient;
uniform	vec4	uAmbient;
uniform	mat4	mAmbient;
uniform	sampler2D	iAmbient;

uniform	int	sDiffuse;
uniform	vec4	uDiffuse;
uniform	mat4	mDiffuse;
uniform	sampler2D	iDiffuse;

varying	vec4	vVertex;
varying	vec4	vNormal;
varying	vec4	vAmbient;
varying	vec4	vDiffuse;

void main()
{
	vec3	dst=vVertex.xyz-uCamera.xyz;
	vec4	ambient=vAmbient*(0<sAmbient?texture2D(iAmbient,(mAmbient*vVertex).xy):uAmbient);
	vec4	diffuse=vDiffuse*(0<sDiffuse?texture2D(iDiffuse,(mDiffuse*vVertex).xy):uDiffuse);
	float	np=(dot(normalize(vNormal.xyz),normalize(uLight.xyz-vVertex.xyz)));
	float	vp=length(vVertex.xyz-uCamera.xyz);
	gl_FragColor=(ambient+(np<0.0?0.0:np)*diffuse);
	gl_FragColor.a=gl_FragColor.a/exp(vp*uFog);
}
