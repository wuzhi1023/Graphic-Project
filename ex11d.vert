uniform float movement;
void main()
{
   vec4 vert = gl_Vertex;
//   float move = length(texture2D(noiseMap, vec2(gl_Vertex.x+movement, gl_Vertex.y)));
   float move = length(gl_Vertex.xy);
   if(gl_Vertex.x>1.0) vert.z += 0.01*(0.5+gl_Vertex.x)*sin(movement+gl_Vertex.x);
   else vert.z += 0.004*(0.5+gl_Vertex.x)*sin(movement+gl_Vertex.x);
//   vec3 vertex = vec3(gl_Vertex);
   gl_TexCoord[0] = gl_MultiTexCoord0;
//   gl_FrontColor = vec4(vertex, 1.0);
   gl_FrontColor = gl_Color;
   gl_Position = gl_ModelViewProjectionMatrix * vert;
}
