#extension GL_EXT_geometry_shader4 : enable

uniform float base;
uniform float start;
uniform float scale;
varying in vec3 vNoiseDir[];

#define PI 3.14159265358979323846

void main()
{
    //float width = 100.0/scale;
    float width = 30.0;
    
    vec3 mg = vec3(0.0);
    for(int i = 0; i < gl_VerticesIn; ++i){
        mg += gl_PositionIn[i].xyz;
    }
    mg /= float(gl_VerticesIn);
    
    vec4 mgm = gl_ModelViewMatrix*vec4(mg, 1.0);
    for(int i = 0; i < gl_VerticesIn; ++i){
        gl_FrontColor = gl_FrontColorIn[i];
        
        vec4 p = gl_PositionIn[i];
        vec3 su = p.xyz - mg.xyz;
        vec3 add = vec3(0.0);
        float bias1 = 0.0;
        if (mgm.y-base > start && mgm.y-base <= 50.0+start) {
            bias1 = sin((mgm.y-base-start)/50.0*90.0/180.0*PI);
            add = vNoiseDir[0]*width*bias1;
        }else if (mgm.y-base > 50.0+start) {
            bias1 = 1.0;
            add = vNoiseDir[0]*width;
        }
        
        vec4 mgm2 = gl_ModelViewMatrix*vec4(mg.xyz+add, 1.0);
        
        if(mgm2.y > 50.0+start && mgm2.y <= 150.0+start) {
            float bias2 = sin((90.0+(mgm2.y-50.0-start)/100.0*90.0)/180.0*PI);
            vec4 mv1 = gl_ModelViewMatrix * vec4(vNoiseDir[0]*width, 0.0);
            vec4 mv2 = gl_ModelViewMatrix * vec4(mg, 0.0);
            vec4 mv3 = gl_ModelViewMatrix * vec4(su, 1.0);
            
            gl_Position = gl_ProjectionMatrix*(vec4( vec3(0.0, mv1.y, 0.0)*bias1 + vec3(mv1.x, 0.0, mv1.z)*bias1*(bias2*0.98+0.02) + vec3(0.0, mv2.y, 0.0) + vec3(mv2.x, 0.0, mv2.z)*(bias2*0.98+0.02) + mv3.xyz, 1.0));
        }else if(mgm2.y > 150.0+start) {
            vec4 mv1 = gl_ModelViewMatrix * vec4(vNoiseDir[0]*width, 0.0);
            vec4 mv2 = gl_ModelViewMatrix * vec4(mg, 0.0);
            vec4 mv3 = gl_ModelViewMatrix * vec4(su, 1.0);
            
            gl_Position = gl_ProjectionMatrix*(vec4( vec3(0.0, mv1.y, 0.0)*bias1 + vec3(mv1.x, 0.0, mv1.z)*0.02 + vec3(0.0, mv2.y, 0.0) + vec3(mv2.x, 0.0, mv2.z)*0.02 + mv3.xyz, 1.0));
        }else{
            gl_Position = gl_ModelViewProjectionMatrix*vec4(mg + add + su, 1.0);
        }

        EmitVertex();
    }
    EndPrimitive();
}