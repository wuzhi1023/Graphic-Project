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

    vec3 NormalColor = texture2D(waterNormal, distortedTexCoords).xyz;
    vec3 Normal = vec3(NormalColor.r * 2.0 - 1.0, NormalColor.b, NormalColor.g *2.0 -1.0);
    vec3 N = normalize(Normal);
    vec3 L = normalize(Light);
    vec3 R = reflect(-L,N);
    vec3 V = normalize(View);
    float Id = max(dot(L,N) , 0.0);
    float Is = (Id>0.0) ? pow(max(dot(R,V),0.0) , shineDamper) : 0.0;
    vec3 specularHighlights = vec3(1,1,1) * Is * reflectivity;

    gl_FragColor = mix(Color, vec4(0,0.3,0.5,1), 0.2) + vec4(specularHighlights, 0.0);
}
