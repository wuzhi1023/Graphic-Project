uniform sampler2D Tex0;    //  Day time texture 0
uniform sampler2D Tex1;    //  Day time texture 1
uniform vec2 dim;

void main()
{
    vec2 texCoord = gl_FragCoord.xy/dim;
    vec4 color1 = texture2D(Tex0,texCoord);
    vec4 color2 = texture2D(Tex1,texCoord);
   gl_FragColor = mix(color1, color2, 0.5);
}
