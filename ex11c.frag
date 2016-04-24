//  Set the fragment color

uniform vec2 dim;
uniform sampler2D Tex0;    //  Day time texture 0
uniform sampler2D Tex1;    //  Day time texture 1

void main()
{
    vec4 color1 = texture2D(Tex0,gl_FragCoord.xy/(dim*vec2(2,2)));
    vec4 color2 = texture2D(Tex1,gl_FragCoord.xy/(dim*vec2(2,2)));
    gl_FragColor = mix(color1, color2, 0.5);
}
