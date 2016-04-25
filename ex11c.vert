uniform vec3 cameraPos;
varying vec3 toCameraVector;
varying vec3 View;
varying vec3 Light;

void main()
{
   //  Vertex location in modelview coordinates
   vec4 P = gl_ModelViewMatrix * gl_Vertex;
   //  Light position
   Light  = vec3(1000,10,0) - P.xyz;
   //  Eye position
   View  = -P.xyz;
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   toCameraVector = cameraPos - gl_Position.xyz;
   gl_TexCoord[0] = gl_MultiTexCoord0;
}
