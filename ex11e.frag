//  Set the fragment color

uniform vec2 dim;
uniform float movement;
uniform sampler2D Tex0;
uniform sampler2D Tex1;
uniform sampler2D waterDUDV;
uniform sampler2D waterNormal;
uniform float reflection;
varying vec3 View;
varying vec3 Light;

const float waveStrength = 0.03;
const float shineDamper = 3.0;
const float reflectivity = 0.6;

void main()
{
    vec2 distortedTexCoords = texture2D(waterDUDV, vec2(gl_TexCoord[0].s + movement, gl_TexCoord[0].t)).rg*0.1;
    distortedTexCoords = gl_TexCoord[0].st + vec2(distortedTexCoords.x, distortedTexCoords.y+movement);
    vec2 distortion = (texture2D(waterDUDV, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;
    vec2 texCoord = gl_FragCoord.xy/dim + distortion;
    texCoord = clamp(texCoord, 0.001, 0.999);
    vec4 color1 = texture2D(Tex0,texCoord);
    vec4 color2 = texture2D(Tex1,texCoord);
    vec4 Color = mix(color1, color2, reflection);

    gl_FragColor = mix(Color, vec4(0,0.3,0.5,1), 0.2);
}
