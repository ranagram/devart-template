#version 120

attribute vec3 noiseDir;
varying vec3 vNoiseDir;

void main()
{
    vec3 P = vec3(gl_ModelViewMatrix * gl_Vertex);
    vec3 N = normalize(gl_NormalMatrix * gl_Normal);
    vec3 L = normalize(gl_LightSource[0].position.xyz - P);
    float dotNL = dot(N, L);
    
    vec4 ambient = gl_FrontLightProduct[0].ambient;
    vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(0.0, dotNL);
    vec3 V = normalize(-P);
    vec3 H = normalize(L + V);
    float dotNH = pow(max(dot(N, H), 0.0), gl_FrontMaterial.shininess);
    vec4 specular = gl_FrontLightProduct[0].specular * abs(dotNH);
    
    gl_FrontColor = ambient + diffuse + specular;
    
    gl_Position = gl_Vertex;
    
    vNoiseDir = noiseDir;
}
