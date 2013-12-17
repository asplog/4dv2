uniform mat4 uProjection;						
uniform mat4 uPerspective;						
uniform mat4 uModelview;						
uniform mat4 uReshape;							

attribute vec4	aVertex;						
attribute vec4	aNormal;						
attribute vec4	aDiffuse;						
attribute vec4	aAmbient;						

varying	vec4	vVertex;						
varying	vec4	vNormal;						
varying	vec4	vDiffuse;						
varying	vec4	vAmbient;						

void main()											
{													
	vVertex=uModelview*uReshape*aVertex;			
	vNormal=uReshape*aNormal;						
	gl_Position=uProjection*uPerspective*vVertex;	
	vAmbient=aAmbient;								
	vDiffuse=aDiffuse;								
}													
