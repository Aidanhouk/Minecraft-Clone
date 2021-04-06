#ifndef SHADER_DATA_H
#define SHADER_DATA_H

struct ShaderData{
    bool fxaa;

    bool bloom;

    bool motionblur;
    
    bool aniso;
};

extern ShaderData g_ShaderSettings;

#endif