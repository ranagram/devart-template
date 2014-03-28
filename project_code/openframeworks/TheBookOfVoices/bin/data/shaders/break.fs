uniform float fogStart;
uniform float fogEnd;

void main()
{
    const float LOG2 = 1.442695;
    float z = gl_FragCoord.z/ gl_FragCoord.w;
    
    float fogFactor = (fogEnd - z) / (fogEnd - fogStart);
    
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    
    gl_FragColor = mix(vec4(0.2, 0.2, 0.2, 1.0), gl_Color, fogFactor);
    
    // gl_FragColor = gl_Color;
}