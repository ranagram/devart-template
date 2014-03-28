#version 120

attribute vec3 bombDir;
varying vec3 vBombDir;
varying vec3 VN;

void main()
{
    gl_Position = gl_Vertex;
    
    VN = normalize(gl_NormalMatrix * gl_Normal);
    vBombDir = bombDir;
}
