void main()
{
   vec4 vert = gl_Vertex;
   float move = length(gl_Vertex.xy);
   vert.z += move;
   vec3 vertex = vec3(gl_Vertex);
   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_FrontColor = vec4(vertex, 1.0);
   gl_Position = gl_ModelViewProjectionMatrix * vert;
}
