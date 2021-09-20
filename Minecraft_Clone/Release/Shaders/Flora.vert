#version 330

layout(location = 0) in vec3  inVertexPosition;
layout(location = 1) in vec2  inTextureCoord;
layout(location = 2) in float inCardinalLight;

out vec2 passTextureCoord;
out float passCardinalLight;
out float visibility;

//uniform mat4 projViewMatrix;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform float dayTime;
uniform float globalTime;

const float density = 0.015;
const float gradient = 10.0;

void getDirectional(float pl){
    if(pl == 0.5){ //bottom
        passCardinalLight =  0.5f; //The bottom of a block is never lit
    }else if(pl == 0.6){ //front
        if(dayTime < 1500){
            passCardinalLight =  mix(1.0f, 0.8f, dayTime/1500);
        }
        if(dayTime >= 1500 && dayTime < 22500){
            passCardinalLight = 0.8f;
        }
        if(dayTime >= 22500 && dayTime < 24000){
            passCardinalLight =  mix(0.8f, 1.0f, (dayTime-22500)/1500);
        }
    }else if(pl == 0.8){ //right
        passCardinalLight =  0.8f; //These aren't effected by either side
    }else if(pl == 0.9){ //left
        passCardinalLight =  0.8f; //These aren't effected by either side
    }else if(pl == 1.0){ //top
        if(dayTime < 6000){
            passCardinalLight =  mix(0.8f, 1.0f, dayTime/6000);
        }
        if(dayTime >= 6000 && dayTime < 12000){
            passCardinalLight =  mix(1.0f, 0.8f, (dayTime-6000)/6000);
        }
        if(dayTime >= 12000 && dayTime < 18000){
            passCardinalLight = 0.8f;
        }
        if(dayTime >= 18000 && dayTime < 24000){
            passCardinalLight = 0.8f;
        }
    }else if(pl == 0.7){ //back
        if(dayTime < 10500 || dayTime >= 13500){
            passCardinalLight = 0.8f;
        }
        if(dayTime >= 10500 && dayTime < 12000){
            passCardinalLight = mix(0.8f, 1.0f, (dayTime-10500)/1500);
        }
        if(dayTime >= 12000 && dayTime < 13500){
            passCardinalLight = mix(1.0f, 0.8f, (dayTime-12000)/1500);
        }
    }
}

vec4 getWorldPos()
{
	vec3 inVert = inVertexPosition.xyz;
	inVert.x += sin((globalTime + inVert.z + inVert.y) * 1.8f) / 15.0f;
	inVert.z -= cos((globalTime + inVert.x + inVert.y) * 1.8f) / 15.0f;
	
	return vec4(inVert, 1);
}

void main()
{
	vec4 positionRelativeToCam = viewMatrix * getWorldPos();
	gl_Position = projMatrix * positionRelativeToCam;
	passTextureCoord = inTextureCoord;
	getDirectional(inCardinalLight);

	//float distance = length(positionRelativeToCam.xyz);
	//visibility = exp(-pow((distance * density), gradient));
	//visibility = clamp(visibility, 0.0, 1.0);
}